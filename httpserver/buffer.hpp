#pragma once 

#include "file.hpp"
#include <string.h>
#include <string>

namespace httpserver {

class Buffer {
public:
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  
  Buffer(Buffer&&) = default;
  Buffer& operator=(Buffer&&) = default;

  Buffer() {}

  Buffer(char* buffer, int size) {
    request_.assign(buffer, size);
  }

  std::string assignBuffer(char* buffer, int size) {
    if (size > 0) {
      std::string s(buffer, size);
      request_ += s;
    }
    return request_;
  }

  std::string& getBuffer() {
    return request_;
  }

  size_t size() const{
    return request_.size();
  }

  bool isReady() const {
    return ready_;
  }

  void setReady() {
    ready_ = true;
  }

private:
  std::string request_;
  bool ready_{false};
};

} // namespace httpserver

