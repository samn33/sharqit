#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>
#include <vector>

namespace Sharq {
  
  inline std::vector<std::string> split(const std::string& str, const char sep)
  {
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
      v.push_back(buffer);
    }
    return v;
  }
  inline std::string ltrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
  {
    str.erase(0, str.find_first_not_of(whitespace));
    return str;
  }
  inline std::string rtrim(std::string &str, std::string const &whitespace = " \r\n\t\v\f")
  {
    str.erase(str.find_last_not_of(whitespace) + 1);
    return str;
  }
  inline std::string trim(std::string &str, std::string const &whitespace=" \r\n\t\v\f")
  {
    std::string rstr = rtrim(str, whitespace);
    return ltrim(rstr, whitespace);
  }
  inline std::string stats_to_string(const std::map<std::string, uint32_t>& sts)
  {
    std::stringstream ss;
    for (auto it = sts.begin(); it != sts.end(); ++it) {
      ss << it->first << ": " << it->second << std::endl;
    }
    return ss.str();
  }
}

#endif
