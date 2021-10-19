import config as config_obj
import json
import subprocess
import time
from databasemanager import DatabaseManager
from datagenerator import DataGenerator
from graphgenerator import GraphGenerator
from monitor import ProcessMonitor
from os import path

BAZEL_BIN_DIR = '../bazel-bin/temporalgraph'
OUTPUT_FILENAME = "results.json"

# TIME_GENERATORS = {
#     'dummy': timegen.DummyGenerator()
# }


# def get_time_generator(timegen_type):
#     return TIME_GENERATORS.get(timegen_type, timegen.DummyGenerator())


def generate_data(V, E, T, datapath):
    print(f"V = {V}, E = {E}, T = {T}")
    graph = GraphGenerator.gen_adjacencies(
        V, E, T)
    DataGenerator.gen(graph, V, E, T, datapath)


def update_with_flags(commandline, config, output_file):
    commandline = f"{commandline} --has_edge_epochs={config.has_edge_epochs}"
    commandline = f"{commandline} --neighbours_epochs={config.neighbours_epochs}"
    commandline = f"{commandline} --aggregate_epochs={config.aggregate_epochs}"
    commandline = f"{commandline} --output_file={output_file}"
    return commandline


def run_version(config, version, database):
    subprocess.run(
        ["bazel", "build", f"v{version}:main"])
    print(config.datapath)
    output_file = f"v{version}/{OUTPUT_FILENAME}"
    pmonitor = ProcessMonitor(
        update_with_flags(
            commandline=f"{BAZEL_BIN_DIR}/v{version}/main < {config.datapath}",
            config=config,
            output_file=output_file))

    try:
        print("running...")
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
    # pmonitor.print()

    results = open(output_file)
    data = json.load(results)
    results.close()
    database.insert(data)
    return


def main(config):
    database_manager = DatabaseManager()
    if not isinstance(config.V, list):
        config.V = [config.V]
    if not isinstance(config.E, list):
        config.E = [config.E]
    if not isinstance(config.T, list):
        config.T = [config.T]

    print(f"V: {config.V}, E: {config.E}, T: {config.T}")

    for V in config.V:
        for E in config.E:
            for T in config.T:
                generate_data(V, E, T, config.datapath)

                # I am executing "make target" here
                if config.version == "all":
                    i = 0
                    while path.exists(f"v{i}"):
                        run_version(config, i, database_manager)
                        i += 1
                elif isinstance(config.version, list):
                    for i in config.version:
                        run_version(config, i, database_manager)
                elif isinstance(config.version, int):
                    run_version(config, config.version, database_manager)
                else:
                    raise Exception("Invalid config version")

    database_manager.close()
    return


if __name__ == '__main__':
    main(config_obj)
