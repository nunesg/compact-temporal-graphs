#include <iostream>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/FixedSizeArray.h"

TEST(ArrayTest, fixedSizeArrayTest) {
  FixedSizeArray arr(4, 8);
  arr.write(0, 33);
  arr.write(1, 42);
  LOG(INFO) << "arr[0] = " << arr[0];
  LOG(INFO) << "arr[1] = " << arr[1];
  EXPECT_EQ(arr[0], 33);
  EXPECT_EQ(arr[1], 42);
}