#pragma once

#include <iostream>

#include "glog/logging.h"

namespace compact {
namespace lib {

class Utils {
 public:
  /*
    Returns the index of the first position between [bgin, end] on the sorted
    array which has value >= val (arr.size() if no number is >= val).
    Example: array = {0, 2, 4, 4, 5} lower_bound(array, 1, 3, 4) will return 2,
    which is the index of the first 4
  */
  template <typename ArrayType>
  static uint lower_bound(const ArrayType& arr, uint begin, uint end, int val) {
    int low = begin, high = end, mid;

    uint answer = arr.size();
    while (low <= high) {
      mid = low + (high - low) / 2;
      if ((int)arr[mid] >= val) {
        answer = mid;
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }
    return answer;
  }

  /*
    Returns the index of the first position between [bgin, end] on the sorted
    array which has value > val (arr.size() if no number is >= val).
    Example: array = {0, 2, 4, 4, 5} upper_bound(array, 1, 3, 4) will return 4,
    which is the index of the first number greater than 4
  */
  template <typename ArrayType>
  static uint upper_bound(const ArrayType& arr, uint begin, uint end, int val) {
    int low = begin, high = end, mid;

    uint answer = arr.size();
    while (low <= high) {
      mid = low + (high - low) / 2;
      if ((int)arr[mid] > val) {
        answer = mid;
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }
    return answer;
  }
};

}  // namespace lib
}  // namespace compact
