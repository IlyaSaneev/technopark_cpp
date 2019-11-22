#include "utils.hpp"
#include <boost/algorithm/string.hpp>

namespace utils {
std::vector<std::string> split(const std::string& line,
                               const std::string& delimiter) {
  std::vector<std::string> strs;
  boost::split(strs, line, boost::is_any_of(delimiter));
  return strs;
}

bool starts_with(const std::string& s, const std::string& predicate) {
  return boost::starts_with(s, predicate);
}
}  // namespace utils
