#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/FixedSizeArray.h"

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
  FixedSizeArray<5> arr(7);
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
  FixedSizeArray<5> mat[4];
  mat[0].resize(4);
  mat[0].write(3, 5);
  LOG(INFO) << mat[0][3];
  EXPECT_EQ(mat[0][3], 5);

  FixedSizeArray<5>* matPtr;
  matPtr = new FixedSizeArray<5>[4];
  matPtr[0].assign(4, 0);
  matPtr[0].write(3, 5);
  LOG(INFO) << matPtr[0][3];
  EXPECT_EQ(matPtr[0][3], 5);

  std::vector<FixedSizeArray<5>> vet(4);
  vet[0].resize(4);
  vet[0].write(3, 3);
  LOG(INFO) << vet[0][3];
  EXPECT_EQ(vet[0][3], 3);
}

}  // namespace test
}  // namespace lib
}  // namespace compact