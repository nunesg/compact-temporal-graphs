import time
import psutil
import subprocess
from monitor import ProcessMonitor

BAZEL_BIN_DIR = '../bazel-bin/temporalgraph'


def main():
    # I am executing "make target" here
    for i in range(2):
        subprocess.run(["bazel", "build", f"v{i}:main"])
        pmonitor = ProcessMonitor([f"{BAZEL_BIN_DIR}/v{i}/main"])

        try:
            pmonitor.execute()
            # poll as often as possible; otherwise the subprocess might
            # "sneak" in some extra memory usage while you aren't looking
            while pmonitor.poll():
                time.sleep(.001)

        finally:
            # make sure that we don't leave the process dangling?
            pmonitor.close()

        print(f"========== Run Version {i} ==========\n")
        print(f"return code:{pmonitor.p.returncode}")
        print(f"time:{(pmonitor.t1 - pmonitor.t0): .9f} seconds")
        print(f"max_vms_memory:{pmonitor.max_vms_memory/1024} KB")
        print(f"max_rss_memory:{pmonitor.max_rss_memory/1024} KB")
        print(f"\n=====================================")
    return


if __name__ == '__main__':
    main()
