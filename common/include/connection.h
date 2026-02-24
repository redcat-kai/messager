#ifndef CONNECTION_H
#define CONNECTION_H
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <string>

namespace conn {
struct IConnection {
  virtual void connect() = 0;
  virtual void send(const std::string& message) = 0;
  virtual std::string receive() = 0;
  ~IConnection() = default;
};

class DomainSocketConnection : public IConnection {
 public:
  void send(const std::string& message) override;
  std::string receive() override;

 protected:
  int sock_fd = 0;
};

class ClientConnection : public DomainSocketConnection {
 public:
  void connect() override;
  ~ClientConnection();

 private:
  sockaddr_un addr{};
};

class ServerConnection : public DomainSocketConnection {
 public:
  void connect() override;
  ~ServerConnection();

 private:
  int server_fd = 0;
  sockaddr_un addr{};
};
}  // namespace conn

#endif  // CONNECTION_H
