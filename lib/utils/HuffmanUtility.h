#pragma once

#include <math.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/BitArray.h"
#include "lib/FixedSizeArray.h"
#include "lib/HuffmanTree.h"

namespace compact {
namespace lib {

class HuffmanUtility {
 public:
  using FrequencyContainer = HuffmanTree::FrequencyContainer;
  using CodeContainer = HuffmanTree::CodeContainer;
  using ContainerType = HuffmanTree::ContainerType;

  HuffmanUtility() {}

  template <typename ArrayType>
  void encode(const ArrayType& values, BitArray& bit_stream) {
    FrequencyContainer freq;
    build_frequencies(values, freq);

    CodeContainer codes;
    tree.build(freq, codes);
    build_stream(values, codes, bit_stream);
  }

  void decode(const BitArray& bit_stream, ContainerType& values) const {
    tree.decode(bit_stream, values);
    return;
  }

  template <typename ArrayType>
  static HuffmanTree::Node* get_tree(const ArrayType& values,
                                     CodeContainer& codes) {
    FrequencyContainer freq;
    build_frequencies(values, freq);

    return HuffmanTree::get(freq, codes);
  }

 private:
  HuffmanTree tree;

  /*
    build array of pairs (value, frequency), sorted by value
  */
  template <typename ArrayType>
  static void build_frequencies(ArrayType values, FrequencyContainer& freq) {
    if (values.empty()) {
      return;
    }
    sort(values.begin(), values.end());
    freq.push_back({values[0], 1});
    for (uint i = 1; i < values.size(); i++) {
      if (values[i] == values[i - 1]) {
        freq.back().second++;
      } else {
        freq.push_back({values[i], 1});
      }
    }
  }

  /*
    build stream of bits with the encoded array of values

    codes are a dictionary on the form: {value, {code, code_size}}
  */
  template <typename ArrayType>
  static void build_stream(const ArrayType& values, const CodeContainer& codes,
                           BitArray& bit_stream) {
    if (values.empty()) {
      return;
    }
    uint total_length = 0;
    for (auto val : values) {
      total_length += codes.at(val).second;
    }

    bit_stream.resize(total_length);
    uint idx = 0;
    for (auto val : values) {
      write_code(bit_stream, idx, codes.at(val).first, codes.at(val).second);
    }
  }

  /*
    write a code on the bit_stream

    TODO: change to bit_stream.write_interval()
  */
  static void write_code(BitArray& bit_stream, uint& idx, uint code,
                         uint code_size) {
    for (uint i = 0; i < code_size; i++) {
      bit_stream.write(idx++, 1 & (code >> i));
    }
  }
};

}  // namespace lib
}  // namespace compact