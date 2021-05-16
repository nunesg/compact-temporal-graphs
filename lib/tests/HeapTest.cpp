#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/Heap.h"

namespace compact {
namespace lib {
namespace test {

// test heap push
TEST(HeapTest, pushTest) {
  /*
    init
  */
  Heap<3> heap(5, [](uint a, uint b) -> bool { return a < b; });

  heap.push(4);
  LOG(INFO) << "heap = " << heap.to_string();
  EXPECT_EQ(heap.top(), 4);
  heap.push(2);
  LOG(INFO) << "heap = " << heap.to_string();
  EXPECT_EQ(heap.top(), 2);
  heap.push(3);
  LOG(INFO) << "heap = " << heap.to_string();
  EXPECT_EQ(heap.top(), 2);
  heap.push(1);
  LOG(INFO) << "heap = " << heap.to_string();
  EXPECT_EQ(heap.top(), 1);
  heap.push(0);
  LOG(INFO) << "heap = " << heap.to_string();
  EXPECT_EQ(heap.top(), 0);

  try {
    heap.push(0);
    LOG(FATAL) << "Fail! Expected exception";
  } catch (std::exception& e) {
    LOG(INFO) << "Catch: " << e.what();
    EXPECT_EQ(e.what(), std::string("Cannot push to heap. Max size reached"));
  }

  heap.clear();
  try {
    heap.top();
    LOG(FATAL) << "Fail! Expected exception";
  } catch (std::exception& e) {
    LOG(INFO) << "Catch: " << e.what();
    EXPECT_EQ(e.what(), std::string("Heap is empty!"));
  }
}

// test heap pop
TEST(HeapTest, popTest) {
  /*
    init
  */
  Heap<3> heap(5, [](uint a, uint b) -> bool { return a < b; });

  std::vector<uint> vet{4, 2, 3, 1, 0};

  for (uint e : vet) {
    heap.push(e);
  }

  std::sort(vet.begin(), vet.end());
  for (uint i = 0; i < vet.size(); i++) {
    LOG(INFO) << "heap: " << heap.to_string();
    EXPECT_EQ(heap.pop(), vet[i]);
  }

  try {
    heap.pop();
    LOG(FATAL) << "Expected exception on popping empty heap!";
  } catch (std::exception& e) {
    LOG(INFO) << "Catch: " << e.what();
    EXPECT_EQ(e.what(), std::string("Heap is empty!"));
  }
}

// test comparator
TEST(HeapTest, comparatorTest) {
  /*
    init
  */
  std::vector<uint> f{5, 4, 3, 2, 1};
  Heap<3> heap(5, [&f](uint a, uint b) -> bool { return f[a] < f[b]; });
  std::vector<uint> vet{4, 2, 3, 1, 0};

  for (uint e : vet) {
    heap.push(e);
  }

  std::sort(vet.rbegin(), vet.rend());
  for (uint i = 0; i < vet.size(); i++) {
    LOG(INFO) << "heap: " << heap.to_string();
    EXPECT_EQ(heap.pop(), vet[i]);
  }

  LOG(INFO) << "MAX_HEAP:";
  // use heap as a max_heap
  heap.set_comparator([&f](uint a, uint b) -> bool { return f[a] > f[b]; });
  std::sort(vet.begin(), vet.end());
  for (uint e : vet) {
    heap.push(e);
  }
  for (uint i = 0; i < vet.size(); i++) {
    LOG(INFO) << "heap: " << heap.to_string();
    EXPECT_EQ(heap.pop(), vet[i]);
  }
}

}  // namespace test
}  // namespace lib
}  // namespace compact