#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/FixedSizeArray.h"

// test array of fixed-length elements
TEST(ArrayTest, fixedSizeArrayTest) {
  /*
    init

    Array of seven 5-bit-length elements. Will use 2 positions of an unsigned
    int array to be stored

    00000 00000 00000 00000 00000 00000 00 | 000 00000 00000 ...
    idx 0 idx 1 idx 2 idx 3 idx 4 idx 5  idx 6
  */
  FixedSizeArray<5> arr(7);
  std::vector<int> v(7);

  // 1-cell value
  arr.write(1, 22);
  v[1] = 22;
  EXPECT_EQ(arr[1], v[1]);

  // 2 cells value
  arr.write(6, 21);
  v[6] = 21;
  EXPECT_EQ(arr[6], v[6]);

  // overrite 1-cell value
  arr.write(1, 9);
  v[1] = 9;
  EXPECT_EQ(arr[1], v[1]);

  // overrite 2-cell value
  arr.write(1, 12);
  v[1] = 12;
  EXPECT_EQ(arr[1], v[1]);

  // access index out of bounds: should fail
  try {
    arr.write(7, 0);
    LOG(FATAL) << "Uncaught exception!";
  } catch (std::exception& e) {
    LOG(INFO) << "Caught exception: " << e.what();
  }

  try {
    arr.read(-1);
    LOG(FATAL) << "Uncaught exception!";
  } catch (std::exception& e) {
    LOG(INFO) << "Caught exception: " << e.what();
  }
}