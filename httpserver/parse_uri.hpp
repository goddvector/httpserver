#ifndef HTTP_SREVER_PARSE_URI_HPP_
#define HTTP_SREVER_PARSE_URI_HPP_

#include <string>

namespace httpserver {
class ParseUri {
public:
  ParseUri() : uri_(""), method_("GET"), path_("/") {}
  std::string handleParseUri(char* buffer) {
    char *p = buffer;
    std::string requestline;
    while (*p != '\n') {
      requestline.push_back(*p);
      *p++;
    }
    std::cout << requestline << '\n';
    return requestline;
  }

private:
  std::string uri_;
  std::string method_;
  std::string path_;
};

} // namespace httpserver
#endif // HTTP_SREVER_PARSE_URI_HPP__
