#ifndef CONNECTION_H
#define CONNECTION_H
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <memory>
#include <string>

namespace conn {
struct IConnection {
  virtual void send(const std::string& message) = 0;
  virtual std::string receive() = 0;
  virtual ~IConnection() = default;
};

// Concrete unix socket class
class DomainSocketConnection : public IConnection {
 public:
  explicit DomainSocketConnection(int fd) : sock_fd(fd) {}
  void send(const std::string& message) override;
  std::string receive() override;
  ~DomainSocketConnection();

 protected:
  int sock_fd = -1;
};

std::unique_ptr<IConnection> connect_client(const std::string& path);

class Server {
 public:
  Server(const std::string& file_path);
  std::unique_ptr<IConnection> accept();
  ~Server();

 private:
  int server_fd = -1;
  std::string socket_path;
  sockaddr_un addr{};
};
}  // namespace conn

#endif  // CONNECTION_H
