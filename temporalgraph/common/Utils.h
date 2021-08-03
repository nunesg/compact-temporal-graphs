#pragma once

#include <iostream>
#include <string>
#include <utility>

namespace compact {
namespace temporalgraph {

template <typename F, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<F, S>& dt) {
  os << "{" << dt.first << "," << dt.second << "}";
  return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) {
      os << ",";
    }
    os << *it;
  }
  return os;
}

class Utils {
 public:
  template <typename Iterator>
  static std::string join(Iterator begin, Iterator end,
                          const std::string& delim) {
    std::ostringstream os;
    if (begin != end) {
      os << *begin++;
      for (; begin != end; begin++) {
        os << delim << *begin;
      }
    }
    return os.str();
  }
};

}  // namespace temporalgraph
}  // namespace compact