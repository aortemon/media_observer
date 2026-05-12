#pragma once

#include <chrono>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include "common.hpp"

namespace media_observer {
class MediaObserver {
    std::map<ObservingFileType, std::vector<std::string>> files_;
    std::filesystem::path directory_;
    std::chrono::seconds interval_;

    void GetHomeDirectory();
    void Update();

  public:
    explicit MediaObserver(const std::chrono::seconds& interval);
    MediaObserver(const std::chrono::seconds& interval, std::filesystem::path directory);

    void Run();
    [[nodiscard]] std::map<ObservingFileType, std::vector<std::string>> GetState() const;
};
} // namespace media_observer