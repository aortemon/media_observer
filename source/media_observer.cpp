#include "media_observer.hpp"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <thread>

#include "common.hpp"

namespace media_observer {

MediaObserver::MediaObserver(const std::chrono::seconds &interval)
    : interval_(interval) {
  GetHomeDirectory();
}

MediaObserver::MediaObserver(const std::chrono::seconds &interval,
                             std::filesystem::path directory)
    : directory_(std::move(directory)), interval_(interval) {}

void MediaObserver::GetHomeDirectory() {
  uid_t uid = getuid();
  // NOLINTNEXTLINE
  struct passwd *pw = getpwuid(uid);

  if (pw != nullptr) {
    directory_ = pw->pw_dir;
  } else {
    throw std::runtime_error("Can't define user and his hoe directory");
  }
}

void MediaObserver::Update() {
  if (!std::filesystem::exists(directory_) ||
      !std::filesystem::is_directory(directory_)) {
    throw std::runtime_error(directory_.string() +
                             " doesnot exist or is not a directory");
  }
  files_.clear();

  for (const auto &entry : std::filesystem::directory_iterator(directory_)) {
    if (entry.is_regular_file()) {
      std::string extension = entry.path().extension();
      const auto &[is_observing, filetype] = DetectType(extension);
      if (is_observing) {
        files_[filetype].emplace_back(entry.path().filename());
      }
    }
  }
}

void MediaObserver::Run() {
  while (true) {
    auto start = std::chrono::steady_clock::now();
    Update();
    auto end = std::chrono::steady_clock::now();
    auto elapsed = end - start;
    if (elapsed < interval_) {
      std::this_thread::sleep_for(interval_ - elapsed);
    }
  }
}

std::map<ObservingFileType, std::vector<std::string>>
MediaObserver::GetState() const {
  return files_;
};

} // namespace media_observer