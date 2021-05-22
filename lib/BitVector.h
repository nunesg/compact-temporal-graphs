#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitArray.h"

namespace compact {
namespace lib {

class BitVector : public Array {
 public:
  BitVector() : bit_stream() {}

  BitVector(uint n) { resize(n); }

  BitVector(const std::vector<uint>& values) { reset(values); }

  BitVector(const std::initializer_list<uint>& values)
      : BitVector(std::vector<uint>(values)) {}

  void resize(uint n) { bit_stream.resize(n); }

  template <typename ArrayType>
  void reset(const ArrayType& values) {
    bit_stream.reset(values);
  }

  void assign(uint n, uint val) { bit_stream.assign(n, val); }

  uint size() const override { return bit_stream.size(); }

  uint read(uint idx) const override { return bit_stream.read(idx); }

  void write(uint idx, uint val) override { bit_stream.write(idx, val); }

  // TODO
  uint rank(uint pos) const { return 0; }

  // TODO
  uint select(uint idx) const { return 0; }

  std::string to_string() const { return bit_stream.to_string(); }

 private:
  BitArray bit_stream;
};

}  // namespace lib
}  // namespace compact
