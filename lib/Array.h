#pragma once

#include "glog/logging.h"

namespace compact {
namespace lib {

class Array {
 public:
  Array() {}

  virtual uint size() const = 0;

  virtual uint read(uint) const = 0;

  virtual uint operator[](uint idx) const = 0;

  virtual void write(uint, uint) = 0;
};

}  // namespace lib
}  // namespace compact
