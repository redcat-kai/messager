#include <iostream>
#include <vector>

#include "connection.h"

int main() {
  auto server = conn::Server("/tmp/messager.sock");
  std::vector<std::unique_ptr<conn::IConnection>> clients;

  while (true) {
    try {
      clients.push_back(server.accept());
    } catch (std::exception& e) {
      std::cerr << "Error connecting to socket, details: " << e.what() << '\n';
      return EXIT_FAILURE;
    }

    for (auto it = clients.begin(); it != clients.end();) {
      if (auto data = (*it)->receive(); data.size() != 0) {
        std::clog << "got: " << data << "\n";
        (*it)->send("Got your message:)");
        it = clients.erase(it);
      } else {
      ++it;
      }
    }

    for (auto& client : clients) {
      if (auto data = client->receive(); data.size() != 0) {
        std::clog << "got: " << data << "\n";
        client->send("Got your message:)");
      }
    }
  }

  return EXIT_SUCCESS;
}
