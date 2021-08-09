#pragma once

#include <vector>

#include "glog/logging.h"
#include "lib/Array.h"
#include "lib/BitVector.h"
#include "lib/WaveletTreeInterface.h"
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

    uint mid = get_mid();
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

  uint access(uint idx) const {
    if (low == high) {
      return low;
    }

    if (!bitvec[idx]) return left->access(bitvec.rank(idx, 0));
    return right->access(bitvec.rank(idx));
  }

  uint rank(uint idx, uint c) const {
    if (low == high) {
      return low == c ? idx : 0;
    }

    if (c <= get_mid()) return left->rank(bitvec.rank(idx, 0), c);
    return right->rank(bitvec.rank(idx), c);
  }

  uint select(uint idx, uint c) const {
    if (low == high) return idx;

    uint idx_below;
    if (c <= get_mid()) {
      idx_below = left->select(idx, c);
      return bitvec.select(idx_below, 0);
    }
    idx_below = right->select(idx, c);
    return bitvec.select(idx_below, 1);
  }

  uint range_count(uint l, uint r, uint val) const {
    if (low == high) {
      return low == val ? (r - l + 1) : 0;
    }

    if (val <= get_mid()) {
      return left->range_count(bitvec.rank(l, 0), bitvec.rank(r, 0), val);
    }
    return right->range_count(bitvec.rank(l), bitvec.rank(r), val);
  }

  // considering half-open interval [l, r)
  uint range_next_value_pos(uint l, uint r, uint val) const {
    // LOG(INFO) << "low = " << low << ", high = " << high
    //           << ", mid = " << get_mid() << ", [l = " << l << ", r = " << r
    //           << "), val = " << val;
    if (low == high) {
      if (low < val) {
        // LOG(INFO) << "leaf >> low = " << low << ", return " << r;
        return r;
      }
      // LOG(INFO) << "leaf << low = " << low << ", return " << l;
      return l;
    }
    // LOG(INFO) << "not leaf";
    // if val is on the right, the answer must also be to the right
    if (val > get_mid()) {
      uint idx_below = right ? right->range_next_value_pos(bitvec.rank(l),
                                                           bitvec.rank(r), val)
                             : r;
      LOG(INFO) << "idx_below (right) = " << idx_below;
      return bitvec.select(idx_below, 1);
    }

    // all elements that went to the right are valid, but there may be some
    // valid values to the left as well

    // position of first element to go to the right
    uint ans_right = bitvec.select(bitvec.rank(l, 1), 1);
    // LOG(INFO) << "ans_right = " << ans_right;
    // LOG(INFO) << "left_null = " << (left == nullptr);
    // position of the answer going to the left
    uint idx_below = left ? left->range_next_value_pos(bitvec.rank(l, 0),
                                                       bitvec.rank(r, 0), val)
                          : r;
    // LOG(INFO) << "idx_below = " << idx_below;
    uint ans_left = bitvec.select(idx_below, 0);
    // LOG(INFO) << "ans_left = " << ans_left;

    // LOG(INFO) << "ans_right = " << ans_right
    //           << ", idx_below (left) = " << idx_below
    //           << ", ans_left = " << ans_left;
    return std::min(ans_left, ans_right);
  }

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

  uint get_mid() const { return low + (high - low) / 2; }
};

// ========================= Wavelet Tree ========================

class WaveletTree : public WaveletTreeInterface {
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
    low = *std::min_element(values.begin(), values.end());
    high = *std::max_element(values.begin(), values.end());
    root.reset(new WaveletTreeNode(values, 0, values.size(), low, high));
  }

  uint size() const { return root->size(); }

  uint access(uint idx) const { return root->access(idx); }

  uint rank(uint pos, uint c) const {
    if (!check_value(c)) {
      return 0;
    }
    return root->rank(pos, c);
  }

  uint select(uint pos, uint c) const {
    if (!check_value(c)) return size();
    return root->select(pos, c);
  }

  uint range_count(uint l, uint r, uint val) const {
    if (!check_value(val) || !check_interval(l, r)) {
      return 0;
    }

    return root->range_count(l, r, val);
  }

  // returns the index of the first number >= val in [l, r]
  // or r+1 in case there's no number >= val in [l, r]
  uint range_next_value_pos(uint l, uint r, uint val) const {
    if (!check_interval(l, r)) return r + 1;
    // r+1 because the interval on the wavelet_tree node is half-empty
    return std::min(root->range_next_value_pos(l, r + 1, val), r + 1);
  }

  // returns the value of the first number >= val in [l, r]
  uint range_next_value(uint l, uint r, uint val) const {
    uint idx = range_next_value_pos(l, r + 1, val);
    if (idx > r) {
      LOG(WARNING) << "range_next_value for value " << val
                   << " doesn't exist on the interval [" << l << "," << r
                   << "]";
      return 0;
    }
    return access(idx);
  }

  uint operator[](uint idx) const override { return access(idx); }

  // TODO
  std::string to_string() const { return "Wavelet Tree"; }

 private:
  uint low;
  uint high;
  NodePtr root;

  bool check_value(uint val) const {
    if (val < low || val > high) {
      LOG(INFO) << (std::string() + "Error! The value " + std::to_string(val) +
                    " is not part of the wavelet tree!");
      return false;
    }
    return true;
  }

  bool check_interval(uint l, uint r) const {
    if (l < 0 || l >= size() || r < 0 || r >= size()) {
      LOG(INFO) << ("invalid interval bounds on Wavelet Tree");
      return false;
    }
    return true;
  }
};

}  // namespace lib
}  // namespace compact
