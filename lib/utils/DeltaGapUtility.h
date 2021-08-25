#pragma once

#include <vector>

#include "glog/logging.h"

namespace compact {
namespace lib {

// use one different instance to manipulate each compressed array
class DeltaGapUtility {
 public:
  using Container = typename std::vector<uint>;

  DeltaGapUtility() : offset(0) {}

  /*
    Get the DeltaGap coding of an array of values. The DeltaGap encoding change
    each value to the difference between its value and the previous element on
    the array.

    Example of deltaGap encoding: {0, 4, 1, 2, 0} -> {0, 4, -3, 1, -2}

    As it may result in negative values, we shift the values according to the
    absolute value of the minimum one (offset), in the example above the minimum
    value is -3, therefore the offset would be 3. Given the offset we sum it to
    every DeltaGap value to remove the negatives.

    Example: {0, 4, 1, 2, 0} -> {0, 4, -3, 1, -2} (offset 3)
    result after applying the offset -> {0, 7, 0, 4, 1}

    TODO: make this return a pointer
  */
  template <typename ArrayType>
  Container get_array_code(const ArrayType& values) {
    if (!values.size()) {
      return Container();
    }
    offset = get_offset(values);

    Container deltaGap(values.size());
    deltaGap[0] = values[0];
    for (uint i = 1; i < values.size(); i++) {
      deltaGap[i] = values[i] + offset - values[i - 1];
    }
    return deltaGap;
  }

  /*
    Given a deltaGap which is the result of the get_array_code above, return the
    original array of values.

    Example: {0, 7, 0, 4, 1} -> {0, 4, 1, 2, 0}

    TODO: make this return a pointer
  */
  template <typename ArrayType>
  Container decode_array(const ArrayType& deltaGap) const {
    if (!deltaGap.size()) {
      return Container();
    }
    Container values(deltaGap.size());

    values[0] = deltaGap[0];
    for (uint i = 1; i < deltaGap.size(); i++) {
      values[i] = (int)deltaGap[i] - offset + values[i - 1];
    }
    return values;
  }

 private:
  int offset;

  template <typename ArrayType>
  int get_offset(const ArrayType& values) const {
    int off = 0;
    for (uint i = 1; i < values.size(); i++) {
      off = std::min(off, (int)values[i] - (int)values[i - 1]);
    }
    // off will be <= 0, just return absolute value
    return -off;
  }
};

}  // namespace lib
}  // namespace compact
