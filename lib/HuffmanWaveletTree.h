// [DEPRECATED]

#pragma once

#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitVector.h"
#include "lib/HuffmanTree.h"
#include "lib/WaveletTreeInterface.h"
#include "lib/utils/BitmaskUtility.h"
#include "lib/utils/HuffmanUtility.h"
#include "lib/utils/Utils.h"

namespace compact {
namespace lib {

// class HuffmanWaveletTreeNode {
//  public:
//   using HuffmanWaveletTreeNodePointer =
//       typename std::unique_ptr<HuffmanWaveletTreeNode>;
//   using HuffmanNode = HuffmanTree::Node;
//   using CodeContainer = HuffmanTree::CodeContainer;

//   HuffmanWaveletTreeNode() : huff_node(NULL), bitvec() {}

//   HuffmanWaveletTreeNode(std::vector<uint> values, uint start, uint end,
//                          const CodeContainer& codes, HuffmanNode* node,
//                          uint level = 0)
//       : HuffmanWaveletTreeNode() {
//     build(values, start, end, codes, node, level);
//   }

//   HuffmanWaveletTreeNode(const std::initializer_list<uint>& values, uint
//   start,
//                          uint end, const CodeContainer& codes,
//                          HuffmanNode* node, uint level = 0)
//       : HuffmanWaveletTreeNode(std::vector<uint>(values), start, end, codes,
//                                node, level) {}

//   void build(std::vector<uint> values, uint start, uint end,
//              const CodeContainer& codes, HuffmanNode* node, uint level) {
//     huff_node = node;
//     // LOG(INFO) << "huffman wavelet - leaf value: " << node->val
//     //           << ", level: " << level;
//     if (node->leaf()) return;

//     auto checker = [&codes, &level](uint val) -> bool {
//       uint code = codes.at(val).first;
//       // LOG(INFO) << "check val: " << val << ", level: " << level << ",
//       code: "
//       //           << BitmaskUtility::to_string(code, codes.at(val).second);
//       return ((code >> level) & 1) == 0;
//     };

//     uint sz = end - start;
//     BitArray b(sz);
//     for (uint i = 0; i < sz; i++) b.write(i, !checker(values[start + i]));
//     // LOG(INFO) << "level: " << level;
//     // LOG(INFO) << "values before partition = "
//     //           << Utils::join(values.begin() + start, values.begin() + end,
//     //           ",");
//     auto pivot_idx = std::stable_partition(values.begin() + start,
//                                            values.begin() + end, checker) -
//                      values.begin();
//     // LOG(INFO) << "values after partition = "
//     //           << Utils::join(values.begin() + start, values.begin() + end,
//     //           ",");
//     bitvec.reset(b);
//     // LOG(INFO) << "bitvector = " << bitvec.to_string();

//     left.reset(new HuffmanWaveletTreeNode(values, start, pivot_idx, codes,
//                                           node->left, level + 1));
//     right.reset(new HuffmanWaveletTreeNode(values, pivot_idx, end, codes,
//                                            node->right, level + 1));
//   }

//   uint size() const { return bitvec.size(); }

//   uint access(uint idx) const {
//     if (huff_node->leaf()) {
//       return huff_node->val;
//     }

//     if (bitvec[idx]) return right->access(bitvec.rank(idx));
//     return left->access(bitvec.rank(idx, 0));
//   }

//   uint rank(uint idx, uint code) const {
//     if (huff_node->leaf()) {
//       return idx;
//     }

//     if (code & 1) return right->rank(bitvec.rank(idx), code >> 1);
//     return left->rank(bitvec.rank(idx, 0), code >> 1);
//   }

//   uint select(uint idx, uint code) const {
//     if (huff_node->leaf()) {
//       return idx;
//     }

//     uint idx_below;
//     if (code & 1) {
//       idx_below = right->select(idx, code >> 1);
//       return bitvec.select(idx_below, 1);
//     }
//     idx_below = left->select(idx, code >> 1);
//     return bitvec.select(idx_below, 0);
//   }

//  private:
//   // pointer to the HuffmanTree node
//   HuffmanNode* huff_node;
//   // bitvector of the values
//   BitVector bitvec;
//   // left node
//   HuffmanWaveletTreeNodePointer left;
//   // right node
//   HuffmanWaveletTreeNodePointer right;
// };

// // ========================= Wavelet Tree ========================

// class HuffmanWaveletTree : public WaveletTreeInterface {
//  public:
//   using Node = HuffmanWaveletTreeNode;
//   using NodePtr = HuffmanWaveletTreeNode::HuffmanWaveletTreeNodePointer;
//   using CodeContainer = HuffmanTree::CodeContainer;

//   ~HuffmanWaveletTree() { delete huff_root; }

//   HuffmanWaveletTree() {}

//   HuffmanWaveletTree(const std::vector<uint>& values) : HuffmanWaveletTree()
//   {
//     reset(values);
//   }

//   HuffmanWaveletTree(const std::initializer_list<uint>& values)
//       : HuffmanWaveletTree(std::vector<uint>(values)) {}

//   void reset(const std::vector<uint>& values) {
//     huff_root = HuffmanUtility::get_tree(values, codes);
//     // std::string s;
//     // huff_root->infix(s);
//     // LOG(INFO) << "huffman tree: " << s;
//     root.reset(
//         new HuffmanWaveletTreeNode(values, 0, values.size(), codes,
//         huff_root));
//   }

//   uint size() const override { return root->size(); }

//   uint access(uint idx) const override { return root->access(idx); }

//   uint rank(uint pos, uint c) const override {
//     check_value(c);
//     return root->rank(pos, codes.at(c).first);
//   }

//   uint select(uint pos, uint c) const override {
//     check_value(c);
//     return root->select(pos, codes.at(c).first);
//   }

//   uint range_count(uint l, uint r, uint val) const override {
//     return rank(r + 1, val) - rank(l, val);
//   }

//   // TODO
//   std::string to_string() const override { return "Huffman Wavelet Tree"; }

//   uint operator[](uint idx) const override { return access(idx); }

//   uint measure_memory() const override {
//     // TODO
//     return 0;
//   }

//  private:
//   uint low;
//   uint high;
//   NodePtr root;
//   HuffmanTree::Node* huff_root;
//   CodeContainer codes;

//   void check_value(uint val) const {
//     if (!codes.count(val)) {
//       throw std::runtime_error(std::string() + "Error! The value " +
//                                std::to_string(val) +
//                                " is not part of the wavelet tree!");
//     }
//   }
// };

}  // namespace lib
}  // namespace compact
