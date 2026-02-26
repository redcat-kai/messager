#include "connection.h"

namespace conn {
const char* SOCKET_PATH = "/tmp/messager.sock";

std::unique_ptr<IConnection> connect_client(const std::string& path) {
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    throw std::runtime_error("Could not acquire socket file descriptor");
  }

  sockaddr_un addr{};
  addr.sun_family = AF_UNIX;
  std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", path.c_str());

  if (::connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Could not connect to socket");
  }

  return std::make_unique<DomainSocketConnection>(sock_fd);
}

void DomainSocketConnection::send(const std::string& message) {
  write(sock_fd, message.data(), message.size());
}

std::string DomainSocketConnection::receive() {
  char buf[1024];
  ssize_t n = ::read(sock_fd, buf, sizeof(buf));

  if (n == 0) throw std::runtime_error("peer disconnected");
  if (n < 0) throw std::runtime_error("read failed");

  return std::string(buf, static_cast<size_t>(n));
}

DomainSocketConnection::~DomainSocketConnection() {
  if (sock_fd >= 0) close(sock_fd);
}

Server::Server(const std::string& file_path) {
  socket_path = file_path;

  this->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (this->server_fd < 0) {
    throw std::runtime_error("Could not acquire socket file descriptor");
  }

  addr.sun_family = AF_UNIX;
  std::snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", socket_path.c_str());

  // remove existing socket
  unlink(socket_path.c_str());

  if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Could not bind to socket");
  }

  if (listen(server_fd, 5) < 0) {
    throw std::runtime_error("Could not listen to socket");
  }
}

std::unique_ptr<IConnection> Server::accept() {
  int client_fd = ::accept(server_fd, nullptr, nullptr);
  if (client_fd < 0) {
    throw std::runtime_error("Accept on socket failed");
  }

  return std::make_unique<DomainSocketConnection>(client_fd);
}

Server::~Server() {
  close(server_fd);
  unlink(socket_path.c_str());
}
};  // namespace conn
