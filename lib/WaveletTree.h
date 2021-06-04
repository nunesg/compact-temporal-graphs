#pragma once

#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitVector.h"
#include "lib/utils/Utils.h"

namespace compact {
namespace lib {

class WaveletTreeNode {
 public:
  using WaveletTreeNodePointer = typename std::unique_ptr<WaveletTreeNode>;

  WaveletTreeNode() : low(0), high(0), bitvec() {}

  WaveletTreeNode(std::vector<uint> values, uint start, uint end, uint low,
                  uint high)
      : WaveletTreeNode() {
    build(values, start, end, low, high);
  }

  WaveletTreeNode(const std::initializer_list<uint>& values, uint start,
                  uint end, uint low, uint high)
      : WaveletTreeNode(std::vector<uint>(values), start, end, low, high) {}

  void build(std::vector<uint>& values, uint start, uint end, uint low,
             uint high) {
    this->low = low;
    this->high = high;
    // LOG(INFO) << "wavelet low = " << low << ", high = " << high
    //           << ", start = " << start << ", end = " << end;
    if (start == end || low == high) return;

    uint mid = low + (high - low) / 2;
    auto checker = [&mid](uint val) -> bool { return val <= mid; };

    uint sz = end - start;
    BitArray b(sz);
    for (uint i = 0; i < sz; i++) b.write(i, !checker(values[start + i]));
    // LOG(INFO) << "values before partition = "
    //           << Utils::join(values.begin() + start, values.begin() + end,
    //           ",");
    auto pivot_idx = std::stable_partition(values.begin() + start,
                                           values.begin() + end, checker) -
                     values.begin();
    // LOG(INFO) << "values after partition = "
    //           << Utils::join(values.begin() + start, values.begin() + end,
    //           ",");
    bitvec.reset(b);
    // LOG(INFO) << "bitvector = " << bitvec.to_string();

    left.reset(new WaveletTreeNode(values, start, pivot_idx, low, mid));
    right.reset(new WaveletTreeNode(values, pivot_idx, end, mid + 1, high));
  }

  uint size() const { return bitvec.size(); }

 private:
  // smallest value represented by this node
  uint low;
  // highest value represented by this node
  uint high;
  // bitvector of the values
  BitVector bitvec;
  // left node
  WaveletTreeNodePointer left;
  // right node
  WaveletTreeNodePointer right;
};

// ========================= Wavelet Tree ========================

class WaveletTree {
 public:
  using Node = WaveletTreeNode;
  using NodePtr = WaveletTreeNode::WaveletTreeNodePointer;

  WaveletTree() {}

  WaveletTree(const std::vector<uint>& values) : WaveletTree() {
    reset(values);
  }

  WaveletTree(const std::initializer_list<uint>& values)
      : WaveletTree(std::vector<uint>(values)) {}

  void reset(const std::vector<uint>& values) {
    uint low = *std::min_element(values.begin(), values.end());
    uint high = *std::max_element(values.begin(), values.end());
    root.reset(new WaveletTreeNode(values, 0, values.size(), low, high));
  }

  uint size() const { return root->size(); }

  // TODO
  uint rank(uint pos, uint c) const { return 0; }

  // TODO
  uint select(uint idx, uint c) const { return 0; }

  // TODO
  std::string to_string() const { return "Wavelet Tree"; }

 private:
  NodePtr root;
};

}  // namespace lib
}  // namespace compact
