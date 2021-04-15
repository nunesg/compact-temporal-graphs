#pragma once

#include <bits/stdc++.h>

namespace compact {
namespace temporalgraph {

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