#pragma once

#include "media_observer.hpp"
#include <netinet/in.h>
#include <string>

namespace media_observer {

constexpr int BUFFER_SIZE = 4096;

class Server {
  int server_;
  int port_;
  bool is_running_;
  struct sockaddr_in address_;
  int addrlen_;

  MediaObserver *observer_;

  bool SetUpSocket();

  static bool IsGetRequest(const std::string &request);
  void HandleClient(int client);

  static std::string MakeResponse(const std::string &body);
  static std::string MakeErrorResponse(int status_code,
                                       const std::string &message);

public:
  Server();
  explicit Server(int port);
  ~Server();

  void LinkWithObserver(MediaObserver *observer);
  bool Serve();
  void Stop();
};

} // namespace media_observer