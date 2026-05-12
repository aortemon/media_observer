// #include "media_observer.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <string>
#include <thread>
#include "media_observer.hpp"
#include "server.hpp"

using namespace media_observer;

MediaObserver* medobs_ptr = nullptr;
Server* server_ptr = nullptr;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        throw std::invalid_argument("[ERROR] Not enough arguments to run the program");
    }
    std::chrono::seconds interval(std::stoi(argv[1]));
    std::filesystem::path dir_path(argv[2]);
    MediaObserver medobs(interval, dir_path);
    Server server(1234);
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