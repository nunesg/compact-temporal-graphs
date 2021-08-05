#include <memory>
#include <unordered_map>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/HuffmanWaveletTree.h"
#include "lib/WaveletTree.h"
#include "lib/utils/Utils.h"

namespace compact {
namespace lib {
namespace test {

std::unique_ptr<std::vector<uint>> get_random_array(uint sz, uint low,
                                                    uint high) {
  srand((unsigned)time(NULL));
  std::unique_ptr<std::vector<uint>> vet(new std::vector<uint>(sz));
  uint alph_size = high - low + 1;
  for (uint i = 0; i < sz; i++) (*vet)[i] = (rand() % alph_size) + low;
  return vet;
}

// test WaveletTree
TEST(WaveletTreeTest, waveletTreeTest) {
  /*
    init
  */
  WaveletTree tree;
  std::unordered_map<uint, uint> f;
  std::unique_ptr<std::vector<uint>> vet_ptr;

  auto run_tests = [&]() {
    tree.reset(*vet_ptr);
    f.clear();
    for (uint i = 0; i < vet_ptr->size(); i++) {
      uint c = (*vet_ptr)[i];
      EXPECT_EQ(tree[i], c);
      EXPECT_EQ(tree.rank(i, c), f[c]);
      EXPECT_EQ(tree.select(f[c], c), i);
      f[c]++;
    }
  };

  // test basic operations
  vet_ptr = get_random_array(100, 0, 20);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_tests();

  // test 1-size alphabet
  vet_ptr = get_random_array(10, 3, 3);
  run_tests();

  // test high alphabet
  vet_ptr = get_random_array(10, UINT32_MAX - 2, UINT32_MAX);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_tests();

  // ========= test range_count ===========
  vet_ptr = get_random_array(15, 1, 4);
  LOG(INFO) << "random_vet for range_count test: "
            << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  tree.reset(*vet_ptr);
  for (uint i = 0; i < vet_ptr->size(); i++) {
    f.clear();
    for (uint j = i; j < vet_ptr->size(); j++) {
      uint c = (*vet_ptr)[j];
      f[c]++;
      EXPECT_EQ(tree.range_count(i, j, c), f[c]);
    }
  }
}

// test HuffmanWaveletTree
TEST(HuffmanWaveletTreeTest, huffmanWaveletTreeTest) {
  /*
    init
  */
  HuffmanWaveletTree tree;
  std::unordered_map<uint, uint> f;
  std::unique_ptr<std::vector<uint>> vet_ptr;

  auto run_tests = [&]() {
    tree.reset(*vet_ptr);
    f.clear();
    for (uint i = 0; i < vet_ptr->size(); i++) {
      uint c = (*vet_ptr)[i];
      EXPECT_EQ(tree[i], c);
      EXPECT_EQ(tree.rank(i, c), f[c]);
      EXPECT_EQ(tree.select(f[c], c), i);
      f[c]++;
    }
  };

  // test basic operations
  vet_ptr = get_random_array(10, 0, 5);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_tests();

  // test 1-size alphabet
  vet_ptr = get_random_array(10, 3, 3);
  run_tests();

  // test high alphabet
  vet_ptr = get_random_array(10, UINT32_MAX - 2, UINT32_MAX);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_tests();
}

}  // namespace test
}  // namespace lib
}  // namespace compact