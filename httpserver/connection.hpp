#ifndef HTTP_SREVER_CONNECTION_HPP_
#define HTTP_SREVER_CONNECTION_HPP_

#include "request.hpp"
#include "buffer.hpp"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <memory>

#define MAXLEN 4096

namespace httpserver {

class Connection : public std::enable_shared_from_this<Connection> {
public:
  Connection() : fd_(-1) {}
  Connection(int fd) : fd_(fd) {}
  Connection(const Connection&) = delete;
  Connection& operator=(Connection&) = delete;
  Connection(Connection&& c) : fd_(c.fd_), buffer_(std::move(c.buffer_)) {
    c.fd_  = -1;
  }
  Connection& operator=(Connection&& c) {
    using std::swap;
    swap(fd_, c.fd_);
    swap(buffer_, c.buffer_);
    return *this;
  }

  ~Connection() {
    close(fd_);
    fd_ = -1;
  }

  void start() {
    if (handleRead() && buffer_.isReady()) {
      Request req(buffer_.getBuffer());
      std::string response = req.handleRequest();
      handleWrite(response);
    }
    else {
      stop();
    }
  }

  void stop() {
    close(fd_);
    fd_ = -1;
  }

  int getFd() {
    return fd_;
  }
  void setFd(int fd) {
    fd_ = fd;
  }

  int handleWrite(std::string response) {
    std::cout << "Start write ..." << '\n';
    ssize_t wlen = 0;
    int size = response.size();
    const char *buffer = response.c_str();
    for (;;) {
      wlen = send(fd_, buffer, size, 0);
      if (wlen == 0) {
        return 0;
      }

      if (wlen == -1) {
        if (errno == EAGAIN) {
          break;
        }
        std::cout << "send: wlen: " << wlen << " errno: " <<  errno << " " << strerror(errno) << '\n';
        return -1;
      }
      std::cout << "send: fd:" << fd_ << ' ' << wlen << " of" << ' ' << size << '\n'; 
      buffer += wlen;
      size -= wlen;
    }

    return wlen;
  }
  
  bool handleRead() {
    size_t len = MAXLEN;
    char buffer[MAXLEN] = {0};
    int rlen = Read(buffer, len);
    if (rlen <= 0) {
      stop();
    }
    if (!isComplete(buffer_.getBuffer())) {
      return false;
    }
    return true;
  }

  int Read(char* buffer, size_t size) {
    std::cout << "Start read ..." << '\n';
    ssize_t n = 0;
    for(;;) {
      n = recv(fd_, buffer, size, 0);

      if (n == 0) {
        return 0;
      }

      if (n > 0) {
        buffer_.assignBuffer(buffer, size);
        std::cout << "recv: fd:" << fd_ << ' ' << n << " of" << ' ' << size << '\n'; 
      }

      if (n == -1 && errno == EAGAIN) {
        if (errno == EAGAIN) {
          buffer_.setReady();
          break;
        }
        return -1;
      }
    } 

    auto len = buffer_.size();
    return (len !=  0) ? len : n; 
  } 

private:
  bool isComplete(std::string header) {
    auto n = header.find("\r\n\r\n");
    if (n == std::string::npos) {
      return false;
    }
    return true;
  }

  int fd_{-1};
  Buffer buffer_;
};


} // namespace httpserver

#endif // HTTP_SREVER_CONNECTION_HPP__

