import time
import psutil
import subprocess
from matplotlib import pyplot


class ProcessMonitor:
    def __init__(self, command):
        self.command = command
        self.execution_state = False

    def execute(self, shell=False):
        self.rss_baseline = -1
        self.max_rss_memory = 0
        self.rss_memory_array = []

        self.p = subprocess.Popen(self.command, shell=shell)
        self.execution_state = True

    def poll(self):
        if not self.check_execution_state():
            return False

        try:

            pp = psutil.Process(self.p.pid)
            # obtain a list of the subprocess and all its descendants
            descendants = list(pp.children(recursive=True))
            descendants = descendants + [pp]

            rss_memory = 0

            # calculate and sum up the memory of the subprocess and all its descendants
            for descendant in descendants:
                try:
                    mem_info = descendant.memory_info()

                    rss_memory += mem_info.rss
                except psutil.NoSuchProcess:
                    # sometimes a subprocess descendant will have terminated between the time
                    # we obtain a list of descendants, and the time we actually poll this
                    # descendant's memory usage.
                    pass
            self.rss_memory_array.append(rss_memory)
            if self.rss_baseline == -1:
                self.rss_baseline = rss_memory
            self.max_rss_memory = max(self.max_rss_memory, rss_memory)

        except psutil.NoSuchProcess:
            return self.check_execution_state()

        return self.check_execution_state()

    def is_running(self):
        return psutil.pid_exists(self.p.pid) and self.p.poll() == None

    def check_execution_state(self):
        if not self.execution_state:
            return False
        if self.is_running():
            return True
        self.executation_state = False
        self.t1 = time.time()
        return False

    def close(self, kill=False):
        try:
            pp = psutil.Process(self.p.pid)
            if kill:
                pp.kill()
            else:
                pp.terminate()
        except psutil.NoSuchProcess:
            pass
        self.max_rss_memory = self.max_rss_memory - self.rss_baseline

    def print(self):
        pyplot.plot(self.rss_memory_array)
        pyplot.show()
