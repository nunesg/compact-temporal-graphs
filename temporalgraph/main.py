import time
import config as config_obj
import timegen
import subprocess
from os import path
from datagenerator import DataGenerator
from graphgenerator import GraphGenerator
from monitor import ProcessMonitor

BAZEL_BIN_DIR = '../bazel-bin/temporalgraph'
OUTPUT_FILENAME = "results.json"

# TIME_GENERATORS = {
#     'dummy': timegen.DummyGenerator()
# }


# def get_time_generator(timegen_type):
#     return TIME_GENERATORS.get(timegen_type, timegen.DummyGenerator())


def generate_data(config):
    V = config.V
    E = config.E
    T = config.T
    print(f"V = {V}, E = {E}, T = {T}")
    graph = GraphGenerator.gen_adjacencies(
        V, E, T)
    DataGenerator.gen(graph, V, E, T, config.datapath)


def update_with_flags(commandline, config, output_file):
    commandline = f"{commandline} --has_edge_epochs={config.has_edge_epochs}"
    commandline = f"{commandline} --neighbours_epochs={config.neighbours_epochs}"
    commandline = f"{commandline} --aggregate_epochs={config.aggregate_epochs}"
    commandline = f"{commandline} --output_file={output_file}"
    return commandline


def run_version(config, version):
    subprocess.run(
        ["bazel", "build", f"v{version}:main"])
    print(config.datapath)
    pmonitor = ProcessMonitor(
        update_with_flags(
            commandline=f"{BAZEL_BIN_DIR}/v{version}/main < {config.datapath}",
            config=config,
            output_file=f"v{version}/{OUTPUT_FILENAME}"))

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
    print(f"baseline_rss_memory:{pmonitor.rss_baseline/1024} KB")
    print(f"max_rss_memory:{pmonitor.max_rss_memory/1024} KB")
    print(f"\n=====================================")
    pmonitor.print()
    return


def main(config):
    generate_data(config)

    # I am executing "make target" here
    if config.version == "all":
        i = 0
        while path.exists(f"v{i}"):
            run_version(config, i)
            i += 1
    elif isinstance(config.version, list):
        for i in config.version:
            run_version(config, i)
    elif isinstance(config.version, int):
        run_version(config, config.version)
    else:
        raise Exception("Invalid config version")
    return


if __name__ == '__main__':
    main(config_obj)
