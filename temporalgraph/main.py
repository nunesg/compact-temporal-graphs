import time
import psutil
import subprocess
import argparse
from monitor import ProcessMonitor

BAZEL_BIN_DIR = '../bazel-bin/temporalgraph'


def run_version(args, version):
    subprocess.run(["bazel", "build", f"v{version}:main"])
    print(args.data)
    pmonitor = ProcessMonitor(
        f"{BAZEL_BIN_DIR}/v{version}/main < {args.data}")

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


def main(args):
    # I am executing "make target" here
    if args.version == "all":
        for i in range(2):
            run_version(args, i)
    else:
        run_version(args, args.version)
    return


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("--data", help="Path to the input file")
    parser.add_argument("--version", default="all",
                        help="Specific version to execute")
    args = parser.parse_args()
    main(args)
