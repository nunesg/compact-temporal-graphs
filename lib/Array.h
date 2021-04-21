#pragma once

#include "glog/logging.h"

namespace compact {
namespace lib {

class Array {
 public:
  Array() {}

  virtual unsigned int size() const = 0;

  virtual unsigned int read(const unsigned int&) const = 0;

  virtual unsigned int operator[](const unsigned int& idx) const = 0;

  virtual void write(const unsigned int&, const unsigned int&) = 0;
};

}  // namespace lib
}  // namespace compact
