#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitArray.h"
#include "lib/JacobsonRank.h"

namespace compact {
namespace lib {

class BitVector : public Array {
 public:
  BitVector() : bit_stream(new BitArray()), rank_manager(bit_stream) {}

  BitVector(uint n) : BitVector() { resize(n); }

  BitVector(const std::vector<uint>& values) : BitVector() { reset(values); }

  BitVector(const std::initializer_list<uint>& values)
      : BitVector(std::vector<uint>(values)) {}

  void resize(uint n) { bit_stream->resize(n); }

  template <typename ArrayType>
  void reset(const ArrayType& values) {
    bit_stream->reset(values);
    rank_manager.build();
  }

  void assign(uint n, uint val) {
    bit_stream->assign(n, val);
    rank_manager.build();
  }

  uint size() const override { return bit_stream->size(); }

  uint read(uint idx) const override { return bit_stream->read(idx); }

  void write(uint idx, uint val) override { bit_stream->write(idx, val); }

  uint rank(uint pos) const { return rank_manager.rank(pos); }

  // TODO
  uint select(uint idx) const { return 0; }

  std::string to_string() const { return bit_stream->to_string(); }

 private:
  std::shared_ptr<BitArray> bit_stream;
  JacobsonRank<BitArray> rank_manager;
};

}  // namespace lib
}  // namespace compact
