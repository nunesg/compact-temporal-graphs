#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/BitArray.h"
#include "lib/FixedSizeArray.h"
#include "lib/VariableSizeArray.h"
#include "lib/VariableSizeDenseArray.h"
#include "lib/utils/BitmaskUtility.h"
#include "lib/utils/DeltaGapUtility.h"
#include "lib/utils/DensePointersUtility.h"
#include "lib/utils/GamaUtility.h"
#include "lib/utils/HuffmanUtility.h"
#include "lib/utils/Utils.h"

namespace compact {
namespace lib {
namespace test {

// test gama-compression utilities
TEST(GamaUtilityTest, gamaCompressionTest) {
  /*
    test ::get_code(uint) function
  */
  BitArray code = GamaUtility::get_code(5);
  BitArray arr1(std::vector<uint>{0, 0, 1, 1, 0});

  for (int i = 0; i < 5; i++) {
    LOG(INFO) << "i = " << i << "code = " << code[i] << ", arr1 = " << arr1[i];
  }

  EXPECT_EQ(arr1, code);

  /*
    test ::get_code(array) function
  */
  FixedSizeArray arr2(std::vector<uint>{2, 3, 4});
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i = " << i << " arr2[i] = " << arr2[i];
  }
  BitArray code_stream = GamaUtility::get_array_code(arr2);
  BitArray expected_stream(
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
  BitArray code = DensePointersUtility::get_code(5);
  BitArray arr1(std::vector<uint>{0, 1});

  for (int i = 0; i < 2; i++) {
    LOG(INFO) << "i = " << i << ", code = " << code[i]
              << ", arr1 = " << arr1[i];
  }

  EXPECT_EQ(arr1, code);

  /*
    test ::get_code(array) function
  */
  FixedSizeArray arr2(std::vector<uint>{1, 2, 1});
  for (uint i = 0; i < arr2.size(); i++) {
    LOG(INFO) << "i = " << i << " arr2[i] = " << arr2[i];
  }
  BitArray code_stream = DensePointersUtility::get_array_code(arr2);
  BitArray expected_stream(std::vector<uint>{0});

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

// test utils
TEST(UtilsTest, utilitiesTest) {
  // init
  FixedSizeArray farr(std::vector<uint>{0, 2, 4, 4, 5}, 3);
  VariableSizeArray varr(std::vector<uint>{0, 2, 4, 4, 5});
  VariableSizeDenseArray vdarr(std::vector<uint>{0, 2, 4, 4, 5});

  /*
    test lower_bound function
  */
  EXPECT_EQ(Utils::lower_bound(farr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::lower_bound(farr, 0, 4, 4), 2);
  EXPECT_EQ(Utils::lower_bound(farr, 0, 4, 5), 4);
  EXPECT_EQ(Utils::lower_bound(farr, 0, 4, 6), 5);
  EXPECT_EQ(Utils::lower_bound(varr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::lower_bound(varr, 0, 4, 4), 2);
  EXPECT_EQ(Utils::lower_bound(varr, 0, 4, 5), 4);
  EXPECT_EQ(Utils::lower_bound(varr, 0, 4, 6), 5);
  EXPECT_EQ(Utils::lower_bound(vdarr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::lower_bound(vdarr, 0, 4, 4), 2);
  EXPECT_EQ(Utils::lower_bound(vdarr, 0, 4, 5), 4);
  EXPECT_EQ(Utils::lower_bound(vdarr, 0, 4, 6), 5);

  /*
    test upper_bound function
  */
  EXPECT_EQ(Utils::upper_bound(farr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::upper_bound(farr, 0, 4, 4), 4);
  EXPECT_EQ(Utils::upper_bound(farr, 0, 4, 5), 5);
  EXPECT_EQ(Utils::upper_bound(farr, 0, 4, 6), 5);
  EXPECT_EQ(Utils::upper_bound(farr, 0, 4, 3), 2);
  EXPECT_EQ(Utils::upper_bound(varr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::upper_bound(varr, 0, 4, 4), 4);
  EXPECT_EQ(Utils::upper_bound(varr, 0, 4, 5), 5);
  EXPECT_EQ(Utils::upper_bound(varr, 0, 4, 6), 5);
  EXPECT_EQ(Utils::upper_bound(varr, 0, 4, 3), 2);
  EXPECT_EQ(Utils::upper_bound(vdarr, 0, 4, -2), 0);
  EXPECT_EQ(Utils::upper_bound(vdarr, 0, 4, 4), 4);
  EXPECT_EQ(Utils::upper_bound(vdarr, 0, 4, 5), 5);
  EXPECT_EQ(Utils::upper_bound(vdarr, 0, 4, 6), 5);
  EXPECT_EQ(Utils::upper_bound(vdarr, 0, 4, 3), 2);
}

// test deltagap compression utility
TEST(UtilsTest, deltaGapTest) {
  // init

  /*
    after compressed using deltagap, te result would be:
    deltaGap -> {0, 4, -3, 1, -2}
    after applying the offset, the result would be:
    deltaGap -> {0, 7, 0, 4, 1}
  */
  DeltaGapUtility::Container arr{0, 4, 1, 2, 0};
  DeltaGapUtility::Container expectedArr{0, 7, 0, 4, 1};

  // test coding
  DeltaGapUtility utils;
  DeltaGapUtility::Container code(utils.get_array_code(arr));

  for (uint i = 0; i < code.size(); i++) {
    LOG(INFO) << "code[" << i << "]: " << code[i] << ", arr = " << arr[i];
  }
  EXPECT_EQ(code, expectedArr);

  // test decoding
  DeltaGapUtility::Container decoded(utils.decode_array(code));
  for (uint i = 0; i < decoded.size(); i++) {
    LOG(INFO) << "decoded[" << i << "]: " << decoded[i] << ", arr = " << arr[i];
  }
  EXPECT_EQ(decoded, arr);

  // test coding using dense pointers array
  VariableSizeDenseArray codeDense(utils.get_array_code(arr));

  for (uint i = 0; i < codeDense.size(); i++) {
    LOG(INFO) << "codeDense[" << i << "]: " << codeDense[i]
              << ", arr = " << arr[i];
  }
  EXPECT_EQ(codeDense, expectedArr);

  // test decoding using dense pointers array
  DeltaGapUtility::Container decodedDense(utils.decode_array(codeDense));
  for (uint i = 0; i < decodedDense.size(); i++) {
    LOG(INFO) << "decodedDense[" << i << "]: " << decodedDense[i]
              << ", arr = " << arr[i];
  }
  EXPECT_EQ(decodedDense, arr);
}

// test huffman compression utility
TEST(UtilsTest, huffmanTest) {
  // init
  HuffmanUtility huff;

  std::vector<uint> values{1, 1, 1, 2, 2, 3, 4};
  std::vector<uint> decoded_values;
  BitArray expected_bit_stream({0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1});
  BitArray bit_stream;

  huff.encode(values, bit_stream);
  LOG(INFO) << "bit_stream: " << bit_stream.to_string();
  EXPECT_EQ(bit_stream.to_string(), expected_bit_stream.to_string());
  huff.decode(bit_stream, decoded_values);
  EXPECT_EQ(values, decoded_values);

  // test with big values which uses more than logm bits to store
  std::vector<uint> big_values{1000, 1000, 1000, 2000, 2000, 3000, 4000};

  huff.encode(big_values, bit_stream);
  huff.decode(bit_stream, decoded_values);
  LOG(INFO) << "bit_stream: " << bit_stream.to_string();
  EXPECT_EQ(bit_stream.to_string(), expected_bit_stream.to_string());
  EXPECT_EQ(big_values, decoded_values);
}

// test bitmask utility
TEST(UtilsTest, bitmaskTest) {
  // init
  EXPECT_EQ(BitmaskUtility::count_bits(1 << 7), 8);
  EXPECT_EQ(BitmaskUtility::clear_mask_interval(15, 1, 2), 9);
  EXPECT_EQ(BitmaskUtility::get_most_significant(0),
            BitmaskUtility::kWordSize - 1);
  EXPECT_EQ(BitmaskUtility::get_mask_interval(31, 1, 3), 14);
  EXPECT_EQ(BitmaskUtility::get_mask_prefix(42, 3), 10);
  EXPECT_EQ(BitmaskUtility::get_full_ones(4), 15);
  EXPECT_EQ(BitmaskUtility::get_full_ones(32), 0xFFFFFFFF);
}
}  // namespace test
}  // namespace lib
}  // namespace compact