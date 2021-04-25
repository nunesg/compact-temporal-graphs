#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/BitmaskUtility.h"
#include "lib/DensePointersUtility.h"
#include "lib/GamaUtility.h"

namespace compact {
namespace lib {
namespace test {

// test gama-compression utilities
TEST(GamaUtilityTest, gamaCompressionTest) {
  /*
    test ::get_code(uint) function
  */
  GamaUtility::BitArray code = GamaUtility::get_code(5);
  GamaUtility::BitArray arr1(std::vector<uint>{0, 0, 1, 1, 0});

  for (int i = 0; i < 5; i++) {
    LOG(INFO) << "i = " << i << "code = " << code[i] << ", arr1 = " << arr1[i];
  }

  EXPECT_EQ(arr1, code);

  /*
    test ::get_code(array) function
  */
  FixedSizeArray<BitmaskUtility::kMaxLength> arr2(std::vector<uint>{2, 3, 4});
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i = " << i << " arr2[i] = " << arr2[i];
  }
  GamaUtility::BitArray code_stream = GamaUtility::get_array_code(arr2);
  GamaUtility::BitArray expected_stream(
      std::vector<uint>{0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1});

  EXPECT_EQ(code_stream, expected_stream);

  /*
    Test decoding a bit array in gama format
  */
  uint start = 0;
  for (uint i = 0; i < 3; i++) {
    uint nxt = GamaUtility::decode_next(code_stream, start);
    LOG(INFO) << "Next value decoded: " << nxt << ", start index = " << start;
    EXPECT_EQ(arr2[i], nxt);
  }

  // invalid case
  try {
    start = 11;
    GamaUtility::decode_next(code_stream, start);
    LOG(FATAL) << "Exception not caught during decoding!";
  } catch (std::exception& e) {
    LOG(INFO) << "Caught exception during decoding: " << e.what();
  }
}

// test dense-pointers-compression utilities
TEST(DensePointersUtilityTest, densePointersCompressionTest) {
  /*
    test ::get_code(uint) function
  */
  DensePointersUtility::BitArray code = DensePointersUtility::get_code(5);
  DensePointersUtility::BitArray arr1(std::vector<uint>{0, 1});

  for (int i = 0; i < 2; i++) {
    LOG(INFO) << "i = " << i << ", code = " << code[i]
              << ", arr1 = " << arr1[i];
  }

  EXPECT_EQ(arr1, code);

  /*
    test ::get_code(array) function
  */
  FixedSizeArray<BitmaskUtility::kMaxLength> arr2(std::vector<uint>{1, 2, 1});
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i = " << i << " arr2[i] = " << arr2[i];
  }
  DensePointersUtility::BitArray code_stream =
      DensePointersUtility::get_array_code(arr2);
  DensePointersUtility::BitArray expected_stream(std::vector<uint>{0});

  EXPECT_EQ(code_stream, expected_stream);

  /*
    Test decoding a bit array in dense-pointers format
  */
  uint start = 0;
  for (uint i = 0; i < 3; i++) {
    uint nxt = DensePointersUtility::decode(
        code_stream, start,
        start + DensePointersUtility::get_code_length(arr2[i]));
    LOG(INFO) << "Next value decoded: " << nxt << ", start index = " << start;
    EXPECT_EQ(arr2[i], nxt);
    start += DensePointersUtility::get_code_length(arr2[i]);
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact