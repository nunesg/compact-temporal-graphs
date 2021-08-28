#pragma once

#include <math.h>
#include <stdio.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

namespace compact {
namespace temporalgraph {

class TimeCounter {
  using clock = std::chrono::high_resolution_clock;
  using milliseconds = std::chrono::milliseconds;

 public:
  void start() { start_t = clock::now(); }

  void stop() {
    end_t = clock::now();
    add_sample();
  }

  double get_mean() {
    uint sum = std::accumulate(samples.begin(), samples.end(), 0);
    // std::cout << "sum = " << sum << std::endl;
    return (double)sum / samples.size();
  }

  double get_median() {
    uint n = samples.size();
    std::sort(samples.begin(), samples.end());
    if (n & 1) {
      return samples[n / 2];
    }
    return (samples[n / 2] + samples[(n - 1) / 2]) / 2.0;
  }

  std::vector<uint> get_samples() { return std::vector<uint>(samples); }

  uint get_max() { return *std::max_element(samples.begin(), samples.end()); }

  uint get_min() { return *std::min_element(samples.begin(), samples.end()); }

 private:
  clock::time_point start_t, end_t;
  std::vector<uint> samples;

  void add_sample() {
    milliseconds diff_ms =
        std::chrono::duration_cast<milliseconds>(end_t - start_t);
    // printf("diff = %lld\n", diff_ms.count());
    samples.push_back(diff_ms.count());
  }
};

}  // namespace temporalgraph
}  // namespace compact