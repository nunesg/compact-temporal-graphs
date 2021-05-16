#pragma once

#include <unordered_map>
#include <vector>

#include "glog/logging.h"
#include "lib/FixedSizeArray.h"

namespace compact {
namespace lib {

namespace {

// Node for HuffmanTree
struct Node {
  Node *left, *right, *parent;
  uint val;
  Node() : left(NULL), right(NULL), val(0) {}
  Node(uint v) : left(NULL), right(NULL), val(v) {}
};

}  // namespace

class HuffmanUtility {
 public:
  using BitArray = FixedSizeArray<1>;
  using ContainerType = std::vector<uint>;
  using HashContainer = std::unordered_map<uint, uint>;

  HuffmanUtility() {}

  // TODO
  template <typename ArrayType>
  void encode(const ArrayType& values) {
    HashContainer freq;
    build_frequencies(values, freq);
    Node* root = build_tree(freq);
  }

  // TODO
  ContainerType decode(const BitArray& bit_stream) { return ContainerType(); }

 private:
  BitArray bit_tree;

  template <typename ArrayType>
  static void build_frequencies(const ArrayType& values, HashContainer& freq) {
    for (uint i = 0; i < values.size(); i++) {
      freq[values[i]]++;
    }
  }

  // TODO
  static Node* build_tree(const HashContainer& freq) { return new Node(); }
};

}  // namespace lib
}  // namespace compact