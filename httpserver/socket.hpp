#ifndef HTTP_SREVER_SOCKET_HPP_
#define HTTP_SREVER_SOCKET_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

namespace httpserver {
class Socket {
public:
  Socket(const Socket&) = delete;
  Socket& operator=(const Socket&) = delete;

  explicit Socket(const std::string& addr, const std::string& port) {
    int ret;
    int reuse;
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *rp;
    memset(&hints, 0, sizeof(struct addrinfo));
    bool ipv6 = true; 
    if (ipv6) {
      hints.ai_family = AF_INET6;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = 0;
      hints.ai_canonname = NULL;
      hints.ai_addr = NULL;
      hints.ai_next = NULL;
      ret = getaddrinfo(addr.c_str(), port.c_str(),&hints, &result);
      if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
      }
    }
    else {
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_protocol = 0;
      hints.ai_canonname = NULL;
      hints.ai_addr = NULL;
      hints.ai_next = NULL;
      ret = getaddrinfo(addr.c_str(), port.c_str(),&hints, &result);
      if (ret != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(EXIT_FAILURE);
      }
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
      fd_ = socket(rp->ai_family, rp->ai_socktype,
                   rp->ai_protocol);
      if (fd_ == -1)
        continue;
      reuse = 1;
      ret = setsockopt( fd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

      if (bind(fd_ , rp->ai_addr, rp->ai_addrlen) == 0)
        break;                  /* Success */

      close(fd_);
    }

    if (rp == NULL) {               /* No address succeeded */
      fprintf(stderr, "Could not bind\n");
      exit(EXIT_FAILURE);
    }
    freeaddrinfo(result);
  }

  void listen() {
    int ret;
    ret = ::listen(fd_, 1024);
    if (ret < 0) {
      perror("listen");
      exit(EXIT_FAILURE);
    }
  }

  int getfd() {
    return fd_;
  }

  ~Socket() {
    close(fd_);
    fd_ = -1;
  }

private:
  int fd_;
};

} // namespace httpserver

#endif // HTTP_SREVER_SOCKET_HPP_