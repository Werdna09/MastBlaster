#pragma once
#include "raylib.h"
#include <vector>
#include <string>

struct FontManager {
    std::vector<Font> fonts;
    std::vector<std::string> names;
    int current = 0;

    void loadAll();
    Font get() const { return fonts[current]; }
    std::string getName() const { return names[current]; }
    void next();
    void prev();
    void unload();
};

