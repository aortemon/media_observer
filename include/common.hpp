#pragma once
#include <array>
#include <cstdint>
#include <string_view>

namespace media_observer {

constexpr uint8_t OBSERVING_TYPES_COUNT = 3;
constexpr std::array<std::string_view, OBSERVING_TYPES_COUNT> OBSERVING_TYPES_NAMES = {
    "audio", "video", "images"};

enum ObservingFileType : uint8_t { AUDIO, VIDEO, IMAGES };

constexpr uint8_t FileTypeToId(ObservingFileType ftype) {
    return static_cast<uint8_t>(ftype);
}

struct ExtensionTypePair {
    std::string_view ext;
    ObservingFileType type;
};

constexpr std::array<ExtensionTypePair, 12> EXT_TO_TYPE = {
    {{.ext = ".mp3", .type = ObservingFileType::AUDIO},
     {.ext = ".wav", .type = ObservingFileType::AUDIO},
     {.ext = ".m4a", .type = ObservingFileType::AUDIO},
     {.ext = ".mp4", .type = ObservingFileType::VIDEO},
     {.ext = ".avi", .type = ObservingFileType::VIDEO},
     {.ext = ".mov", .type = ObservingFileType::VIDEO},
     {.ext = ".mkv", .type = ObservingFileType::VIDEO},
     {.ext = ".jpg", .type = ObservingFileType::IMAGES},
     {.ext = ".jpeg", .type = ObservingFileType::IMAGES},
     {.ext = ".png", .type = ObservingFileType::IMAGES},
     {.ext = ".bmp", .type = ObservingFileType::IMAGES},
     {.ext = ".gif", .type = ObservingFileType::IMAGES}}};

constexpr std::pair<bool, ObservingFileType> DetectType(std::string_view extension) {
    for (const auto& [ext, type] : EXT_TO_TYPE) {
        if (extension == ext) {
            return {true, type};
        }
    }
    return {false, {}};
}

} // namespace media_observer
