#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class ClientConnection {
 public:
  void connect() {
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

  std::string receive() {
    char buf[100];
    ssize_t n = read(sock_fd, buf, sizeof(buf) - 1);
    if (n > 0) {
      buf[n] = '\0';
      return std::string(buf);
    }

    return "";
  }

  void send(const std::string& message) { write(sock_fd, message.data(), message.size()); }

  ~ClientConnection() { close(sock_fd); }

 private:
  int sock_fd = 0;
  sockaddr_un addr{};
  const char* SOCKET_PATH = "/tmp/messager.sock";
};

int main() {
  auto conn = ClientConnection();

  try {
    conn.connect();
  } catch (std::exception& e) {
    std::cerr << "Error connecting to socket, details: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  conn.send("Hello from the client!");

  if (auto message = conn.receive(); message.size() != 0) {
    std::cout << "received: " << message << " back from the server\n";
  }

  return 0;
}
