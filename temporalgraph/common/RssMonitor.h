#pragma once

#include <sys/resource.h>

#include <map>
#include <string>

namespace compact {
namespace temporalgraph {

class RssMonitor {
 public:
  RssMonitor() { measure("baseline"); }

  void measure(std::string label) {
    getrusage(RUSAGE_SELF, &ru);
    lookup[label] = ru.ru_maxrss;
  }

  uint get_discounted(std::string label) {
    return get(label) - get("baseline");
  }

  uint get(std::string label) { return lookup[label]; }

 private:
  struct rusage ru;
  std::map<std::string, uint> lookup;
};

}  // namespace temporalgraph
}  // namespace compact