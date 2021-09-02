#include <bits/stdc++.h>
#include <sys/resource.h>

using namespace std;

int main(int argc, char *argv[]) {
  struct rusage result;
  for (int i = 0; i < 5; i++) {
    getrusage(RUSAGE_SELF, &result);
    printf("maxrss: %ld Kb\n", result.ru_maxrss);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  int *arr;
  arr = new int[10000000];
  memset(arr, 1, sizeof(int) * 10000000);
  for (int i = 0; i < 10; i++) {
    getrusage(RUSAGE_SELF, &result);
    printf("after maxrss: %ld Kb\n", result.ru_maxrss);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
  delete arr;
  for (int i = 0; i < 10; i++) {
    getrusage(RUSAGE_SELF, &result);
    printf("after delete maxrss: %ld Kb\n", result.ru_maxrss);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
  }
}