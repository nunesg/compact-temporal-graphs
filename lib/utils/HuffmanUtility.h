#pragma once

#include <math.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "glog/logging.h"
#include "lib/BitArray.h"
#include "lib/FixedSizeArray.h"
#include "lib/Heap.h"

namespace compact {
namespace lib {

namespace {

// Node for HuffmanTree
struct Node {
  Node *left, *right, *parent;
  uint val;
  Node() : left(NULL), right(NULL), val(0) {}
  Node(uint v) : left(NULL), right(NULL), val(v) {}

  void infix(std::string& str) const {
    str += "{";
    if (left) left->infix(str);
    str += std::to_string(val);
    if (right) right->infix(str);
    str += "}";
  }
};

}  // namespace

class HuffmanUtility {
 public:
  using ContainerType = std::vector<uint>;
  using ii = std::pair<uint, uint>;
  using FrequencyContainer = std::vector<ii>;

  HuffmanUtility() {}

  template <typename ArrayType>
  void encode(const ArrayType& values) {
    FrequencyContainer freq;
    build_frequencies(values, freq);
    Node* root = build_tree(freq);

    std::string tree_str;
    root->infix(tree_str);
    LOG(INFO) << "Huffman_tree: " + tree_str;
    LOG(INFO) << "Huffman_bit_tree: " + bit_tree.to_string();

    // vector<ii> codes = {(code_i, code_length)};
    // build_codes();
    // build_stream(values, codes);
  }

  // TODO
  ContainerType decode(const BitArray& bit_stream) { return ContainerType(); }

 private:
  /*
    tree stored following the rules below:
    1. if node is leaf:
      bit 1 + N bits for leaf value
    2. else:
      bit 0 + build_left + build_right
  */
  BitArray bit_tree;
  uint bit_size;
  uint diff_labels;

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
    Built huffman tree using pointers.
    - Value of leaves are the labels of the elements.
    - Value of non-leaf nodes are the sum of their subtree frequencies
  */
  Node* build_tree(const FrequencyContainer& freq) {
    const uint m = freq.size();
    const uint tree_max_size = m << 1;
    Node* node[tree_max_size];

    diff_labels = m;

    for (uint i = 0; i < m; i++) {
      node[i] = new Node(freq[i].first);
    }

    auto get_node_value = [m, &freq, &node](uint idx) -> uint {
      return idx < m ? freq[idx].second : node[idx]->val;
    };

    bit_size = 1 + uint(log2(tree_max_size));
    Heap heap(tree_max_size, bit_size,
              [&get_node_value](uint idx1, uint idx2) -> bool {
                return get_node_value(idx1) < get_node_value(idx2);
              });

    for (uint i = 0; i < m; i++) {
      heap.push(i);
    }

    uint tsize = m;
    while (heap.size() > 1) {
      uint idx1 = heap.pop();
      uint idx2 = heap.pop();

      Node* node1 = node[idx1];
      Node* node2 = node[idx2];

      Node* new_root = new Node(get_node_value(idx1) + get_node_value(idx2));
      new_root->left = node1;
      node1->parent = new_root;

      new_root->right = node2;
      node2->parent = new_root;
      node[tsize] = new_root;

      heap.push(tsize++);
    }
    Node* root = node[heap.pop()];
    build_bit_tree(root);
    return root;
  }

  /*
    Builds a compressed form of the huffman tree as a bitstream.
    Each node will follow one of the two rules below:
    - internal nodes: 0{left_subtree}{right_subtree}
      -- note that each internal node has exactly 2 children
    - leaf nodes: 1{label using bit_size bits}
  */
  void build_bit_tree(const Node* root) {
    uint tree_size = (diff_labels * 2) - 1;
    bit_tree.resize(tree_size + diff_labels * bit_size);

    uint idx = 0;
    build_bit_tree(root, idx);
  }

  void build_bit_tree(const Node* root, uint& idx) {
    // leaf
    if (!root->left && !root->right) {
      bit_tree.write(idx++, 1);
      write_value(root->val, idx);
      return;
    }
    bit_tree.write(idx++, 0);
    build_bit_tree(root->left, idx);
    build_bit_tree(root->right, idx);
  }

  /*
    write the value on the bit_tree bitstream using bit_size bits
  */
  void write_value(uint val, uint& idx) {
    for (int i = bit_size - 1; i >= 0; i--) {
      bit_tree.write(idx++, 1 & (val >> i));
    }
  }
};

}  // namespace lib
}  // namespace compact