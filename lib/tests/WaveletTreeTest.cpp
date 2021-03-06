#include <memory>
#include <unordered_map>

#include "glog/logging.h"
#include "gtest/gtest.h"
// #include "lib/HuffmanWaveletTree.h"
#include "lib/WaveletTree.h"
#include "lib/WaveletTreeInterface.h"
#include "lib/utils/Utils.h"

namespace compact {
namespace lib {
namespace test {

std::vector<uint> get_random_array(uint sz, uint low, uint high) {
  srand((unsigned)time(NULL));
  std::vector<uint> vet(sz);
  uint alph_size = high - low + 1;
  for (uint i = 0; i < sz; i++) vet[i] = (rand() % alph_size) + low;
  return vet;
}

uint range_next_value_pos(const std::vector<uint> &vet, uint l, uint r,
                          uint val) {
  for (uint i = l; i <= r; i++) {
    if (vet[i] >= val) return i;
  }
  return r + 1;
}

void range_report(const std::vector<uint> &vet, uint l, uint r,
                  std::unordered_map<uint, uint> &result) {
  result.clear();
  for (uint i = l; i <= r; i++) {
    result[vet[i]]++;
  }
}

bool check_reports(std::unordered_map<uint, uint> &r1,
                   std::unordered_map<uint, uint> &r2) {
  if (r1.size() != r2.size()) return false;
  for (auto &it1 : r1) {
    if (!r2.count(it1.first) || r2[it1.first] != it1.second) return false;
  }
  return true;
}

void run_basic_tests(WaveletTreeInterface &tree, std::vector<uint> &vet) {
  std::unordered_map<uint, uint> f;
  tree.reset(vet);
  f.clear();
  for (uint i = 0; i < vet.size(); i++) {
    uint c = vet[i];
    EXPECT_EQ(tree[i], c);
    EXPECT_EQ(tree.rank(i, c), f[c]);
    EXPECT_EQ(tree.select(f[c], c), i);
    f[c]++;
  }
  LOG(INFO) << "tree size(b): " << tree.measure_memory();
};

void run_range_count_tests(WaveletTreeInterface &tree) {
  std::unordered_map<uint, uint> f;
  std::vector<uint> vet;
  // std::unique_ptr<std::vector<uint>> vet_ptr(
  //     new std::vector<uint>({23, 2, 28, 1, 2, 31, 1, 33, 2, 35, 2}));

  // ========= test range_count ===========
  vet = get_random_array(15, 1, 8);
  LOG(INFO) << "random_vet for range_count test: "
            << Utils::join(vet.begin(), vet.end(), ",");
  uint min_val = *std::min_element(vet.begin(), vet.end());
  uint max_val = *std::max_element(vet.begin(), vet.end());
  tree.reset(vet);
  for (uint i = 0; i < vet.size(); i++) {
    f.clear();
    for (uint j = i; j < vet.size(); j++) {
      uint c = vet[j];
      f[c]++;
      for (uint x = min_val; x <= max_val; x++) {
        // LOG(INFO) << "f[" << x << "] in [" << i << "," << j
        //           << "] = " << tree.range_count(i, j, x) << ", fx = " <<
        //           f[x];
        EXPECT_EQ(tree.range_count(i, j, x), f[x]);
      }
    }
  }
  // LOG(INFO) << "f[1] in [0, 5] = " << tree.range_count(0, 5, 1) << ", fc =
  // 1";
}

void run_range_next_value_pos_tests(WaveletTree &tree) {
  std::unordered_map<uint, uint> f;
  std::vector<uint> vet;
  // std::unique_ptr<std::vector<uint>> vet_ptr(
  //     new std::vector<uint>{1, 1, 3, 4, 4, 3, 2});

  // ========= test range_count ===========
  vet = get_random_array(15, 1, 4);
  LOG(INFO) << "random_vet for range_count test: "
            << Utils::join(vet.begin(), vet.end(), ",");
  tree.reset(vet);
  for (uint i = 0; i < vet.size(); i++) {
    f.clear();
    for (uint j = i; j < vet.size(); j++) {
      uint c = vet[j];
      LOG(INFO) << "i = " << i << ", j = " << j << ", val = " << c;
      EXPECT_EQ(tree.range_next_value_pos(i, j, c),
                range_next_value_pos(vet, i, j, c));
    }
  }
}

void run_range_report_tests(WaveletTree &tree) {
  std::unordered_map<uint, uint> r1, r2;
  std::vector<uint> vet;
  // std::unique_ptr<std::vector<uint>> vet_ptr(
  //     new std::vector<uint>{1, 1, 3, 4, 4, 3, 2});

  // ========= test range_report ===========
  vet = get_random_array(5, 1, 100);
  LOG(INFO) << "random_vet for range_report test: "
            << Utils::join(vet.begin(), vet.end(), ",");
  tree.reset(vet);
  for (uint i = 0; i < vet.size(); i++) {
    for (uint j = i; j < vet.size(); j++) {
      range_report(vet, i, j, r1);
      tree.range_report(i, j, r2);
      LOG(INFO) << "r1 [" << i << "," << j << "]:";
      for (auto &p : r1) {
        LOG(INFO) << p.first << ": " << p.second;
      }
      LOG(INFO) << "r2 [" << i << "," << j << "]:";
      for (auto &p : r2) {
        LOG(INFO) << p.first << ": " << p.second;
      }
      EXPECT_EQ(true, check_reports(r1, r2));
    }
  }
}

// test WaveletTree
TEST(WaveletTreeTest, waveletTreeTest) {
  /*
    init
  */
  WaveletTree tree;
  std::vector<uint> vet;

  // test basic operations
  vet = get_random_array(100, 0, 20);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_basic_tests(tree, vet);

  // test 1-size alphabet
  vet = get_random_array(10, 3, 3);
  run_basic_tests(tree, vet);

  // test high alphabet
  vet = get_random_array(10, UINT32_MAX - 2, UINT32_MAX);
  // LOG(INFO) << "random_vet: "
  //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
  run_basic_tests(tree, vet);

  // ========= test range_count ===========
  run_range_count_tests(tree);

  // ========= test range_next_value_pos ==
  run_range_next_value_pos_tests(tree);

  // ========= test range_report ==========
  run_range_report_tests(tree);
}

// [DEPRECATED]
// // test HuffmanWaveletTree
// TEST(HuffmanWaveletTreeTest, huffmanWaveletTreeTest) {
//   /*
//     init
//   */
//   HuffmanWaveletTree tree;
//   std::unique_ptr<std::vector<uint>> vet_ptr;

//   // test basic operations
//   vet_ptr = get_random_array(10, 0, 5);
//   // LOG(INFO) << "random_vet: "
//   //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
//   run_basic_tests(tree, vet_ptr);

//   // test 1-size alphabet
//   vet_ptr = get_random_array(10, 3, 3);
//   run_basic_tests(tree, vet_ptr);

//   // test high alphabet
//   vet_ptr = get_random_array(10, UINT32_MAX - 2, UINT32_MAX);
//   // LOG(INFO) << "random_vet: "
//   //           << Utils::join(vet_ptr->begin(), vet_ptr->end(), ",");
//   run_basic_tests(tree, vet_ptr);

//   // run_range_count_tests(tree);
// }

}  // namespace test
}  // namespace lib
}  // namespace compact