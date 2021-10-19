#pragma once

#include <string>
#include <vector>

#include "glog/logging.h"

namespace compact {
namespace lib {

class WaveletTreeInterface {
 public:
  virtual void reset(const std::vector<uint>& values) = 0;

  virtual uint size() const = 0;

  virtual uint access(uint idx) const = 0;

  virtual uint rank(uint pos, uint c) const = 0;

  virtual uint select(uint pos, uint c) const = 0;

  virtual uint range_count(uint l, uint r, uint val) const = 0;

  virtual uint operator[](uint idx) const = 0;

  virtual std::string to_string() const = 0;

  virtual uint measure_memory() const = 0;
};

}  // namespace lib
}  // namespace compact
