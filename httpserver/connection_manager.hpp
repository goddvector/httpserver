#ifndef HTTP_SREVER_CONNECTION_MANAGER_HPP_
#define HTTP_SREVER_CONNECTION_MANAGER_HPP_

#include "connection.hpp"
#include <memory>
#include <set>

namespace httpserver {

using connection_ptr = std::shared_ptr<Connection>;

class ConnectionManager {
public:
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;
  
  ConnectionManager() : max_(0) {}
  ~ConnectionManager(){}
  uint64_t start(int fd) {
    max_++;
    connections_.insert(std::pair<uint64_t, connection_ptr>(max_, std::make_shared<Connection>(fd, max_, *this)));
    return max_;
  }

  void stop(uint64_t num) {
    connections_.erase(num);
  }

  connection_ptr getConnection(uint64_t num) {
    return connections_.at(num);
  }

  void stop_all() {
    connections_.clear();
  }
private:
  uint64_t max_;
  std::map<uint64_t, connection_ptr> connections_;
};

} // namespace httpserver
#endif // HTTP_SREVER_CONNECTION_MANAGER_HPP__
