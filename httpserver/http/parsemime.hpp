#pragma once 

#include "../handle_string.hpp"
#include <string>
#include <map>
#include <fstream>

namespace httpserver {

class ParseMime {
public:
  ParseMime(std::string filename) : filename_(filename) {}

  void parseMime() {
    std::ifstream input(filename_.c_str());
    for (std::string line; getline(input, line);) {
      line = trim(line);
      if (line.front() == '#') {
        continue;
      }
      std::string key;
      std::string value;
      try {
        std::tie(key, value) = parseline(line, '\t');
      }
      catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        continue;
      }
      conf_.insert(std::pair<std::string, std::string>(value, key));
    }
  }

  std::map<std::string, std::string>& getConf() {
    return conf_;
  }
  void printMime() {
    for (auto p : conf_) {
      std::cout << p.first << ":" << p.second << '\n';
    }
  }


private:
  std::string filename_;
  std::map<std::string, std::string> conf_;  
};

} // namespace httpserver
