#pragma once

#include <math.h>

#include <algorithm>
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
struct HuffmanTreeNode {
  HuffmanTreeNode *left, *right, *parent;
  uint val;
  HuffmanTreeNode() : left(NULL), right(NULL), parent(NULL), val(0) {}
  HuffmanTreeNode(uint v) : left(NULL), right(NULL), parent(NULL), val(v) {}
  ~HuffmanTreeNode() {
    delete left;
    delete right;
  }

  void infix(std::string& str) const {
    str += "{";
    if (left) left->infix(str);
    str += std::to_string(val);
    if (right) right->infix(str);
    str += "}";
  }
};

}  // namespace

class HuffmanBitTree {
 public:
  using Node = HuffmanTreeNode;
  /*
    Builds a compressed form of the huffman tree as a bitstream.
    Each node will follow one of the two rules below:
    - internal nodes: 0{left_subtree}{right_subtree}
      -- note that each internal node has exactly 2 children
    - leaf nodes: 1{label using leaf_bit_size bits}
  */
  void build(const Node* root, uint leaves, uint leaf_bit_size) {
    uint tree_size = (leaves * 2) - 1;
    this->leaf_bit_size = leaf_bit_size;
    bit_tree.resize(tree_size + leaves * leaf_bit_size);

    uint idx = 0;
    build_pre_order(root, idx);
  }

  Node* get_tree() const {
    uint idx = 0;
    return get_tree_pre_order(idx);
  }

  std::string to_string() const { return bit_tree.to_string(); }

 private:
  BitArray bit_tree;
  // number of bits necessary to store the label of the leaves
  uint leaf_bit_size;

  void build_pre_order(const Node* root, uint& idx) {
    // leaf
    if (!root->left && !root->right) {
      bit_tree.write(idx++, 1);
      write_value(root->val, idx);
      return;
    }
    bit_tree.write(idx++, 0);
    build_pre_order(root->left, idx);
    build_pre_order(root->right, idx);
  }

  Node* get_tree_pre_order(uint& idx) const {
    Node* root;
    root = new Node();
    // leaf
    if (bit_tree[idx++] == 1) {
      root->val = read_value(idx);
      return root;
    }
    root->left = get_tree_pre_order(idx);
    root->right = get_tree_pre_order(idx);
    return root;
  }

  /*
    write the value on the bit_tree bitstream using bit_size bits
  */
  void write_value(uint val, uint& idx) {
    for (int i = leaf_bit_size - 1; i >= 0; i--) {
      bit_tree.write(idx++, 1 & (val >> i));
    }
  }

  /*
    read the value on the bit_tree bitstream using bit_size bits
  */
  uint read_value(uint& idx) const {
    uint val = 0;
    for (uint i = 0; i < leaf_bit_size; i++) {
      val = (val << 1) | bit_tree[idx++];
    }
    return val;
  }
};

class HuffmanTree {
 public:
  using Node = HuffmanTreeNode;
  using ii = std::pair<uint, uint>;
  using ContainerType = std::vector<uint>;
  using FrequencyContainer = std::vector<ii>;
  using CodeContainer = std::unordered_map<uint, ii>;
  using NodeContainer = std::vector<Node*>;

  HuffmanTree() {}

  void build(const FrequencyContainer& freq, CodeContainer& codes) {
    NodeContainer nodes;
    Node* root = build_tree(freq, nodes);

    uint maxx = std::max(uint(1), max_element(freq.begin(), freq.end())->first);
    uint leaf_bit_size = 1 + BitmaskUtility::int_log(maxx);
    bit_tree.build(root, freq.size(), leaf_bit_size);
    build_codes(freq.size(), nodes, codes);

    // std::string tree_str;
    // root->infix(tree_str);
    // LOG(INFO) << "Huffman_tree: " + tree_str;
    // LOG(INFO) << "Huffman_bit_tree: " + bit_tree.to_string();

    delete root;
  }

  void decode(const BitArray& bit_stream, ContainerType& values) const {
    std::string s;
    Node* root = bit_tree.get_tree();

    uint idx = 0;
    values.clear();
    while (idx < bit_stream.size()) {
      values.push_back(decode_next(bit_stream, root, idx));
      // LOG(INFO) << "decoded value: " << values.back();
    }
  }

  std::string to_string() const {
    std::string s;
    bit_tree.get_tree()->infix(s);
    return s;
  }

 private:
  /*
    tree stored following the rules below:
    1. if node is leaf:
      bit 1 + N bits for leaf value
    2. else:
      bit 0 + build_left + build_right
  */
  HuffmanBitTree bit_tree;

  /*
    Build huffman tree using pointers.
    - Value of leaves are the labels of the elements.
    - Value of non-leaf nodes are the sum of their subtree frequencies
  */
  Node* build_tree(const FrequencyContainer& freq, NodeContainer& nodes) {
    const uint leaves = freq.size();
    const uint tree_max_size = (leaves << 1) - 1;

    initialize_nodes(freq, nodes, leaves, tree_max_size);

    auto get_node_value = [leaves, &freq, &nodes](uint idx) -> uint {
      return idx < leaves ? freq[idx].second : nodes[idx]->val;
    };

    return build_tree(leaves, nodes, get_node_value);
  }

  void initialize_nodes(const FrequencyContainer& freq, NodeContainer& nodes,
                        uint leaves, uint tree_max_size) {
    nodes.resize(tree_max_size);

    for (uint i = 0; i < leaves; i++) {
      nodes[i] = new Node(freq[i].first);
    }
  }

  Node* build_tree(uint leaves, NodeContainer& nodes,
                   const std::function<uint(uint)>& node_value_getter) {
    // number of bits to store a tree's node index
    uint heap_bit_size = 1 + BitmaskUtility::int_log(nodes.size());
    Heap heap(nodes.size(), heap_bit_size,
              [&node_value_getter](uint idx1, uint idx2) -> bool {
                return node_value_getter(idx1) < node_value_getter(idx2);
              });

    for (uint i = 0; i < leaves; i++) {
      heap.push(i);
    }

    uint tsize = leaves;
    while (heap.size() > 1) {
      uint idx1 = heap.pop();
      uint idx2 = heap.pop();

      Node* node1 = nodes[idx1];
      Node* node2 = nodes[idx2];

      Node* new_root =
          new Node(node_value_getter(idx1) + node_value_getter(idx2));
      new_root->left = node1;
      node1->parent = new_root;

      new_root->right = node2;
      node2->parent = new_root;
      nodes[tsize] = new_root;

      heap.push(tsize++);
    }
    return nodes[heap.pop()];
  }

  /*
    builds the dictionary {label: {code, code_size}}
  */
  void build_codes(uint leaves, const NodeContainer& nodes,
                   CodeContainer& codes) {
    for (uint i = 0; i < leaves; i++) {
      codes[nodes[i]->val] = get_code(nodes[i]);
    }
  }

  /*
    Get the code (path from root) of a given node.
    Left edges have value 0, and right edges have value 1

    For example, the path: left -> right -> right has code 011
    The size of the code is necessary due to leading zeroes on the bitmask
  */
  static ii get_code(Node* leaf) {
    uint code_size = 0;
    Node *curr, *last;
    curr = leaf;
    uint code = 0;
    while (curr->parent) {
      last = curr;
      curr = curr->parent;
      // got up from left child
      if (last == curr->left) {
        code <<= 1;  // add bit 0
      } else {
        code = (code << 1) | 1;  // add bit 1
      }
      code_size++;
    }
    return {reverse_code(code, code_size), code_size};
  }

  static uint reverse_code(uint code, uint code_size) {
    uint new_code = 0;
    for (uint i = 0; i < code_size; i++) {
      new_code = (new_code << 1) | (1 & (code >> i));
    }
    return new_code;
  }

  static uint decode_next(const BitArray& bit_stream, Node* root, uint& idx) {
    while (root->left && root->right) {
      if (bit_stream[idx++]) {
        root = root->right;
      } else {
        root = root->left;
      }
    }
    return root->val;
  }
};

}  // namespace lib
}  // namespace compact