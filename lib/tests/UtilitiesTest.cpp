#include <exception>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"
#include "lib/GamaUtility.h"

namespace compact {
namespace lib {
namespace test {

// test gama-compression utilities
TEST(GamaUtilityTest, gamaCompressionTest) {
  /*
    test ::get_code function
  */
  GamaUtility::BitArray code = GamaUtility::get_code(5);
  GamaUtility::BitArray arr(5);
  arr.write(0, 0);
  arr.write(1, 0);
  arr.write(2, 1);
  arr.write(3, 1);
  arr.write(4, 0);

  EXPECT_EQ(arr, code);
}

}  // namespace test
}  // namespace lib
}  // namespace compact