#include <iostream>

#include "connection.h"

int main() {
  std::unique_ptr<conn::IConnection> conn;
  conn = conn::connect_client("/tmp/messager.sock");

  conn->send("Hello from the client!");

  if (auto message = conn->receive(); message.size() != 0) {
    std::cout << "received: " << message << " back from the server\n";
  }

  return 0;
}
