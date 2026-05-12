// #include "media_observer.hpp"
#include "media_observer.hpp"
#include "server.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using namespace media_observer;

MediaObserver *medobs_ptr = nullptr;
Server *server_ptr = nullptr;

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "[ERROR] Not enough arguments to run the programm\n";
  }
  std::chrono::seconds interval(std::stoi(argv[1]));
  std::filesystem::path dir_path(argv[2]);
  int port = 1234;
  if (argc < 5) {
    port = std::stoi(argv[3]);
  }
  MediaObserver medobs(interval, dir_path);
  Server server(port);
  server.LinkWithObserver(&medobs);

  medobs_ptr = &medobs;
  server_ptr = &server;

  std::thread medobs_thread(&MediaObserver::Run, &medobs);
  std::thread server_thread(&Server::Serve, &server);

  medobs_thread.join();
  server_thread.join();

  server.Serve();

  return 0;
}