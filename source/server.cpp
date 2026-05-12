#include "server.hpp"

#include "json_util.hpp"
#include <array>
#include <asm-generic/socket.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace media_observer {
std::string Server::MakeResponse(const std::string &body) {
  std::string response;

  response = "HTTP/1.1 200 OK\r\n";

  response += "Content-Type: application/json\r\n";
  response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
  response += "Connection: close\r\n";
  response += "\r\n";
  response += body;

  return response;
}

std::string Server::MakeErrorResponse(int status_code,
                                      const std::string &message) {
  std::string response;
  if (status_code == 405) {
    response = "HTTP/1.1 405 Method Not Allowed\r\n";
  } else {
    response = "HTTP/1.1 400 Bad Request\r\n";
  }

  response += "Content-Type: text/plain\r\n";
  response += "Content-Length: " + std::to_string(message.length()) + "\r\n";
  response += "Connection: close\r\n";
  response += "\r\n";

  response += message;

  return response;
}

bool Server::SetUpSocket() {
  server_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_ == 0) {
    std::cerr << "[ERROR] Unable to create socket\n";
    return false;
  }

  int opt = 0;
  if (setsockopt(server_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)) != 0) {
    std::cerr << "[ERROR] Setting up socket options faiels\n";
    return false;
  }

  address_.sin_family = AF_INET;
  address_.sin_addr.s_addr = INADDR_ANY;
  address_.sin_port = htons(port_);

  if (bind(server_, reinterpret_cast<struct sockaddr *>(&address_),
           sizeof(address_)) < 0) {
    std::cerr << "[ERROR] Failed to bind socket to port. Check if it's not "
                 "already in use\n";
    return false;
  }

  if (listen(server_, 3) < 0) {
    std::cerr << "[ERROR] Failed to listen port\n";
    return false;
  }

  return true;
}

bool Server::IsGetRequest(const std::string &request) {
  return request.starts_with("GET");
}

void Server::HandleClient(int client) {
  std::array<char, BUFFER_SIZE> buffer = {0};
  read(client, buffer.data(), BUFFER_SIZE - 1);

  std::string request(buffer.data());
  std::string response;
std:;
  if (!IsGetRequest(request)) {
    response = MakeErrorResponse(405, "Method Not Allowed");
    send(client, response.c_str(), response.length(), 0);
    close(client);
    return;
  }
  std::string path = request.substr(4, request.find(' ', 4) - 4);
  if (path != "/media_files") {
    response = MakeErrorResponse(404, "Not Found");
    send(client, response.c_str(), response.length(), 0);
    close(client);
    return;
  }
  response = MakeResponse(json_util::ToJSON(observer_->GetState()));
  send(client, response.c_str(), response.length(), 0);
  close(client);
}

Server::Server(int port)
    : port_(port), is_running_(false), server_(-1), addrlen_(sizeof(address_)),
      observer_(nullptr) {
  memset(&address_, 0, sizeof(address_));
}

Server::~Server() {
  if (server_ != -1) {
    close(server_);
  }
}

bool Server::Serve() {
  if (!SetUpSocket()) {
    throw std::runtime_error("[ERROR] Can't set up socket");
    return false;
  }

  is_running_ = true;
  std::cout << "\n\nServer listening on: http://localhost:" << port_
            << "/media_files\n";
  std::cout << "Press CTRL + C to stop\n\n";

  while (is_running_) {
    int client = accept(server_, reinterpret_cast<struct sockaddr *>(&address_),
                        reinterpret_cast<socklen_t *>(&addrlen_));
    if (client < 0) {
      std::cerr << "[ERROR] Client acception falied. Contuning...\n";
    }
    HandleClient(client);
  }
  return true;
}

void Server::LinkWithObserver(MediaObserver *observer) {
  observer_ = observer;
};
} // namespace media_observer