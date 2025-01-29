#pragma once

#include <string>
#include <stdexcept>

#include "raylib_wrapper.h"

class AssetsLoader {
    public:
        static Font loadFont(const std::string& path) {
            Font font = LoadFont(path.c_str());

            if(font.texture.id == 0) {
                throw std::runtime_error("Failed to load font from " + path);
            }

            return font;
        }
};
