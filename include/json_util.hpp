#pragma once
#include <map>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "common.hpp"

namespace json_util {

template <typename T>
concept Numeric = std::is_arithmetic_v<T> && !std::is_same_v<T, char>;

std::string ToJSON(const media_observer::ObservingFileType& x);
std::string ToJSON(const std::string& x);
std::string ToJSON(const std::string_view& x);
template <Numeric T>
std::string ToJSON(const T& x);
template <typename T, typename U>
inline std::string ToJSON(const std::map<T, U>& m);
template <typename T>
std::string ToJSON(const std::vector<T>& m);

inline std::string ToJSON(const media_observer::ObservingFileType& x) {
    return "\"" +
           std::string(media_observer::OBSERVING_TYPES_NAMES.at(media_observer::FileTypeToId(x))) +
           "\"";
}

inline std::string ToJSON(const std::string& x) {
    return '"' + x + '"';
}

inline std::string ToJSON(const std::string_view& x) {
    return '"' + std::string(x) + '"';
}

template <Numeric T>
std::string ToJSON(const T& x) {
    return std::to_string(x);
}

template <typename T, typename U>
inline std::string ToJSON(const std::map<T, U>& m) {
    std::stringstream ss;
    bool first = true;
    ss << "{";
    for (const auto& [key, value] : m) {
        if (!first) {
            ss << ", ";
        }
        first = false;
        ss << ToJSON(key) << ": " << ToJSON(value);
    }
    ss << "}";
    return ss.str();
}

template <typename T>
std::string ToJSON(const std::vector<T>& m) {
    std::stringstream ss;
    ss << "[";
    bool first = true;
    for (const auto& item : m) {
        if (!first) {
            ss << ", ";
        }
        first = false;
        ss << ToJSON(item);
    }
    ss << "]";
    return ss.str();
}

} // namespace json_util