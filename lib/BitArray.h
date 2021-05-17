#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/FixedSizeArray.h"

namespace compact {
namespace lib {

class BitArray : public Array {
 public:
  BitArray() : array() {}

  BitArray(uint n) { resize(n); }

  BitArray(const std::vector<uint>& values) { reset(values); }

  uint size() const override { return array.size(); }

  void resize(uint n) { array.resize(n, 1); }

  template <typename ArrayType>
  void reset(const ArrayType& values) {
    array.reset(values, 1);
  }

  void assign(uint n, uint val) { array.assign(n, val, 1); }

  uint read(uint idx) const override { return array.read(idx); }

  void write(uint idx, uint val) override { array.write(idx, val); }

  std::string to_string() const { return array.to_string(); }

 private:
  FixedSizeArray array;
};

}  // namespace lib
}  // namespace compact
