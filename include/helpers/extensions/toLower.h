#pragma once

#include <string>
#include <algorithm>

inline std::string toLower(const std::string& str) {
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
}