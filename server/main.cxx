#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <expected>
#include <iostream>

class ServerConnection {
 public:
  void connect() {
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

    client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd < 0) {
      throw std::runtime_error("Accept on socket failed");
    }
  }

  std::string receive() {
    char buffer[100];
    ssize_t n = read(client_fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
      buffer[n] = '\0';
      return std::string(buffer);
    }

    return "";
  }

  void send(const std::string& message) { write(client_fd, message.data(), message.size()); }

  ~ServerConnection() {
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
  }

 private:
  int server_fd = 0;
  int client_fd = 0;
  sockaddr_un addr{};
  const char* SOCKET_PATH = "/tmp/messager.sock";
};

int main() {
  auto conn = ServerConnection();

  try {
    conn.connect();
  } catch (std::exception& e) {
    std::cerr << "Error connecting to socket, details: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  if (auto data = conn.receive(); data.size() != 0) {
    std::clog << "got: " << data << "\n";
    conn.send("Got your message:)");
  }

  return EXIT_SUCCESS;
}
