#pragma once

#include <math.h>

#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitArray.h"
#include "lib/ClarkSelect.h"
#include "lib/JacobsonRank.h"

namespace compact {
namespace lib {

class BitVector : public Array {
 public:
  BitVector()
      : bit_stream(new BitArray()),
        rank_manager(bit_stream),
        select_manager({ClarkSelect<BitArray>(bit_stream, 0),
                        ClarkSelect<BitArray>(bit_stream)}) {}

  BitVector(uint n) : BitVector() { resize(n); }

  BitVector(const Array& values) : BitVector() { reset(values); }

  BitVector(const std::vector<uint>& values) : BitVector() { reset(values); }

  BitVector(const std::initializer_list<uint>& values)
      : BitVector(std::vector<uint>(values)) {}

  void resize(uint n) { assign(n, 0); }

  template <typename ArrayType>
  void reset(const ArrayType& values) {
    bit_stream->reset(values);
    build();
  }

  void assign(uint n, uint val) {
    bit_stream->assign(n, val);
    build();
  }

  uint size() const override { return bit_stream->size(); }

  uint read(uint idx) const override { return bit_stream->read(idx); }

  void write(uint idx, uint val) override {
    throw std::runtime_error("BitVector is a read-only array!");
  }

  uint rank(uint pos, uint bit_value = 1) const {
    bit_value %= 2;
    uint rank1 = rank_manager.rank(pos);
    if (bit_value)
      return rank1;  // rank 1
    else
      return pos - rank1;  // rank 0
  }

  uint select(uint idx, uint bit_value = 1) const {
    bit_value %= 2;
    return select_manager[bit_value].select(idx);
  }

  std::string to_string() const { return bit_stream->to_string(); }

 private:
  std::shared_ptr<BitArray> bit_stream;
  JacobsonRank<BitArray> rank_manager;
  ClarkSelect<BitArray> select_manager[2];

  void build() {
    rank_manager.build();
    select_manager[0].build();
    select_manager[1].build();
  }
};

}  // namespace lib
}  // namespace compact
