#include "connection.h"

namespace conn {
const char* SOCKET_PATH = "/tmp/messager.sock";

void ClientConnection::connect() {
  sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd < 0) {
    throw std::runtime_error("Could not acquire socket file descriptor");
  }

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  if (::connect(sock_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Could not connect to socket");
  }
}

void DomainSocketConnection::send(const std::string& message) {
  write(sock_fd, message.data(), message.size());
}

std::string DomainSocketConnection::receive() {
  char buf[100];
  ssize_t n = read(sock_fd, buf, sizeof(buf) - 1);
  if (n > 0) {
    buf[n] = '\0';
    return std::string(buf);
  }

  return "";
}

ClientConnection::~ClientConnection() {
  close(sock_fd);
}

void ServerConnection::connect() {
  this->server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (this->server_fd < 0) {
    throw std::runtime_error("Could not acquire socket file descriptor");
  }

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  // remove existing socket
  unlink(SOCKET_PATH);

  if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
    throw std::runtime_error("Could not bind to socket");
  }

  if (listen(server_fd, 5) < 0) {
    throw std::runtime_error("Could not listen to socket");
  }

  sock_fd = accept(server_fd, nullptr, nullptr);
  if (sock_fd < 0) {
    throw std::runtime_error("Accept on socket failed");
  }
}

ServerConnection::~ServerConnection() {
  close(sock_fd);
  close(server_fd);
  unlink(SOCKET_PATH);
}
};  // namespace conn
