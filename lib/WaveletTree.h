#pragma once

#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"

namespace compact {
namespace lib {

class WaveletTree {
 public:
  WaveletTree() {}

  WaveletTree(const Array& values) : WaveletTree() { reset(values); }

  WaveletTree(const std::vector<uint>& values) : WaveletTree() {
    reset(values);
  }

  WaveletTree(const std::initializer_list<uint>& values)
      : WaveletTree(std::vector<uint>(values)) {}

  // TODO
  template <typename ArrayType>
  void reset(const ArrayType& values) {}

  // TODO
  uint size() const { return 0; }

  // TODO
  uint rank(uint pos, uint c) const { return 0; }

  // TODO
  uint select(uint idx, uint c) const { return 0; }

  // TODO
  std::string to_string() const { return "Wavelet Tree"; }

 private:
};

}  // namespace lib
}  // namespace compact
