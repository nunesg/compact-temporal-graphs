#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/FixedSizeArray.h"
#include "lib/VariableSizeArray.h"
#include "lib/VariableSizeDenseArray.h"

namespace compact {
namespace lib {
namespace test {

// test array of fixed-length elements
TEST(ArrayTest, fixedSizeArrayTest) {
  /*
    init

    Array of seven 5-bit-length elements. Will use 2 positions of an unsigned
    int array to be stored

    00000 00000 00000 00000 00000 00000 00 | 000 00000 00000 ...
    idx 0 idx 1 idx 2 idx 3 idx 4 idx 5  idx 6
  */
  FixedSizeArray arr(7, 5);
  std::vector<int> v(7);

  // 1-cell value
  arr.write(0, 22);
  v[0] = 22;
  EXPECT_EQ(arr[0], v[0]);

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

  /*
    create an array of compact arrays
  */
  FixedSizeArray mat[4];
  mat[0].resize(4, 5);
  mat[0].write(3, 5);
  LOG(INFO) << mat[0][3];
  EXPECT_EQ(mat[0][3], 5);

  FixedSizeArray* matPtr;
  matPtr = new FixedSizeArray[4];
  matPtr[0].assign(4, 0, 5);
  matPtr[0].write(3, 5);
  LOG(INFO) << matPtr[0][3];
  EXPECT_EQ(matPtr[0][3], 5);

  std::vector<FixedSizeArray> vet(4);
  vet[0].resize(4, 5);
  vet[0].write(3, 3);
  LOG(INFO) << vet[0][3];
  EXPECT_EQ(vet[0][3], 3);

  /*
    initialize array using vector constructor
  */
  FixedSizeArray arr2(std::vector<uint>({0, 2, 3, 4}));
  FixedSizeArray arr3(4);
  arr3.write(0, 0);
  arr3.write(1, 2);
  arr3.write(2, 3);
  arr3.write(3, 4);
  LOG(INFO) << "arr2 size = " << arr2.size();
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i = " << i << " arr2[i] = " << arr2[i]
              << " arr3[i] = " << arr3[i];
  }
  EXPECT_EQ(arr2, arr3);
}

// test array of variable-length elements
TEST(ArrayTest, variableSizeArrayTest) {
  /*
    init

    Array of three elements {2, 3, 4}

    Will generate the bitstream in gama format:

    0110010000101
  */

  std::vector<uint> values{2, 3, 4};
  VariableSizeArray arr(values);
  for (uint i = 0; i < arr.size(); i++) {
    LOG(INFO) << "i: " << i << ", val = " << values[i] << ", arr = " << arr[i];
    EXPECT_EQ(arr[i], values[i]);
  }

  std::vector<uint> values2{3, 2, 3, 2, 5, 2, 3, 2, 8, 2};
  VariableSizeDenseArray arr2(values2);
  LOG(INFO) << "Variable size array using dense pointers:";
  for (uint i = 0; i < values2.size(); i++) {
    LOG(INFO) << "i: " << i << ", val = " << values2[i];
  }
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i: " << i << ", val = " << values2[i]
              << ", arr = " << arr2[i];
    EXPECT_EQ(arr2[i], values2[i]);
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact