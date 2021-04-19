#include "glog/logging.h"

class Array {
 public:
  Array() {}

  virtual unsigned int size() const = 0;

  virtual unsigned int read(const unsigned int&) const = 0;

  virtual unsigned int operator[](const unsigned int& idx) const = 0;

  virtual void write(const unsigned int&, const unsigned int&) = 0;
};