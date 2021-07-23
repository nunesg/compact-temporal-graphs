import time
import config as config_obj
import timegen
import subprocess
from datagenerator import DataGenerator
from graphgenerator import GraphGenerator
from monitor import ProcessMonitor

BAZEL_BIN_DIR = '../bazel-bin/temporalgraph'

TIME_GENERATORS = {
    'dummy': timegen.DummyGenerator()
}


def get_time_generator(timegen_type):
    return TIME_GENERATORS.get(timegen_type, timegen.DummyGenerator())


def generate_data(config):
    V = config.graph_vertices
    E = config.graph_edges
    graph = GraphGenerator.gen_adjacencies(
        V, E, get_time_generator(config.time_gen))
    DataGenerator.gen(graph, V, E, config.datapath)


def run_version(config, version):
    subprocess.run(["bazel", "build", f"v{version}:main"])
    print(config.datapath)
    pmonitor = ProcessMonitor(
        f"{BAZEL_BIN_DIR}/v{version}/main < {config.datapath}")

    try:
        pmonitor.execute(shell=True)
        # poll as often as possible; otherwise the subprocess might
        # "sneak" in some extra memory usage while you aren't looking
        while pmonitor.poll():
            time.sleep(.001)

    finally:
        # make sure that we don't leave the process dangling?
        pmonitor.close()

    print(f"========== Run Version {version} ==========\n")
    print(f"return code:{pmonitor.p.returncode}")
    print(f"time:{(pmonitor.t1 - pmonitor.t0): .9f} seconds")
    print(f"max_vms_memory:{pmonitor.max_vms_memory/1024} KB")
    print(f"max_rss_memory:{pmonitor.max_rss_memory/1024} KB")
    print(f"\n=====================================")
    return


def main(config):
    generate_data(config)

    # I am executing "make target" here
    if config.version == "all":
        for i in range(2):
            run_version(config, i)
    else:
        run_version(config, int(config.version))
    return


if __name__ == '__main__':
    main(config_obj)
