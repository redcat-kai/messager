#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "connection.h"

int main() {
  auto conn = conn::ClientConnection();

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
