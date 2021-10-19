#pragma once

#include <math.h>

#include <functional>
#include <string>

#include "lib/FixedSizeArray.h"
#include "lib/utils/BitmaskUtility.h"

namespace compact {
namespace lib {

class Heap {
 public:
  using Comparator = typename std::function<bool(uint, uint)>;

  Heap() : max_size(0), sz(0) {}

  Heap(uint n, const Comparator& comparator) {
    uint bit_size = BitmaskUtility::kWordSize;
    setup(n, bit_size, comparator);
  }

  Heap(uint n, uint bit_size, const Comparator& comparator) {
    setup(n, bit_size, comparator);
  }

  void set_comparator(const Comparator& comparator) {
    this->comparator = comparator;
  }

  void clear() { sz = 0; }

  void push(uint elem) {
    check_full();
    uint pos = sz++;
    heap.write(pos, elem);
    bubble_up(pos);
  }

  uint pop() {
    check_empty();
    swap_nodes(0, --sz);
    bubble_down(0);
    return heap[sz];
  }

  uint top() const {
    check_empty();
    return heap[0];
  }

  uint size() const { return sz; }

  std::string to_string() const { return heap.to_string(); }

  // measure memory used in bytes
  uint measure_memory() const {
    return heap.measure_memory() + sizeof(max_size) + sizeof(sz) +
           sizeof(comparator);
  }

 private:
  FixedSizeArray heap;
  uint max_size;
  uint sz;
  Comparator comparator;

  void setup(uint n, uint bit_size, const Comparator& comparator) {
    max_size = n;
    sz = 0;
    set_comparator(comparator);
    heap.resize(max_size, bit_size);
  }

  void bubble_up(uint pos) {
    while (pos > 0) {
      uint parent = get_parent(pos);
      uint pos_val = heap[pos];
      uint parent_val = heap[parent];
      if (comparator(pos_val, parent_val)) {
        swap_nodes(parent, pos);
        pos = parent;
      } else {
        break;
      }
    }
  }

  void bubble_down(uint pos) {
    while (pos < sz) {
      uint left_child = get_left(pos);
      uint right_child = get_right(pos);

      if (left_child < sz && right_child < sz) {
        uint left_val = heap[left_child];
        uint right_val = heap[right_child];

        //
        if (/* left child is best */ (comparator(left_val, right_val) &&
                                      bubble_left(pos)) ||
            /* right child is best */ bubble_right(pos)) {
          continue;
        }
        break;
      }

      // try to bubble_left in case left child is valid but right isn't
      bubble_left(pos);
      break;
    }
  }

  bool bubble_left(uint& pos) {
    if (get_left(pos) >= sz) return false;
    uint left_child = get_left(pos);
    uint pos_val = heap[pos];
    uint left_val = heap[left_child];

    if (comparator(left_val, pos_val)) {
      swap_nodes(pos, left_child);
      pos = left_child;
      return true;
    } else {
      return false;
    }
  }

  bool bubble_right(uint& pos) {
    if (get_right(pos) >= sz) return false;
    uint right_child = get_right(pos);
    uint pos_val = heap[pos];
    uint right_val = heap[right_child];

    if (comparator(right_val, pos_val)) {
      swap_nodes(pos, right_child);
      pos = right_child;
      return true;
    } else {
      return false;
    }
  }

  uint get_parent(uint child) const {
    if (!child) return 0;
    return (child - 1) / 2;
  }

  uint get_left(uint pos) const { return (pos << 1) + 1; }

  uint get_right(uint pos) const { return (pos << 1) + 2; }

  void swap_nodes(uint a, uint b) {
    uint val_a = heap[a];
    heap.write(a, heap[b]);
    heap.write(b, val_a);
  }

  void check_empty() const {
    if (!sz) {
      throw std::runtime_error("Heap is empty!");
    }
  }

  void check_full() const {
    if (size() == max_size) {
      throw std::runtime_error("Cannot push to heap. Max size reached");
    }
  }
};

}  // namespace lib
}  // namespace compact
