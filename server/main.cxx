#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <expected>
#include <iostream>

#include "connection.h"

int main() {
  auto conn = conn::ServerConnection();

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
