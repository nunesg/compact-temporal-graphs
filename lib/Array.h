#pragma once

#include "glog/logging.h"

namespace compact {
namespace lib {

class Array {
 public:
  Array() {}

  virtual uint size() const = 0;

  virtual uint read(uint) const = 0;

  virtual void write(uint, uint) = 0;

  uint operator[](uint idx) const { return this->read(idx); }

  bool operator==(const Array& other) const {
    if (other.size() != this->size()) return false;
    for (uint i = 0; i < this->size(); i++) {
      if (read(i) != other.read(i)) {
        return false;
      }
    }
    return true;
  }
};

}  // namespace lib
}  // namespace compact
