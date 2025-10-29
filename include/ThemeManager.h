#pragma once
#include "raylib.h"
#include <string>
#include <vector>

struct Theme {
    std::string name;

    Color background;
    Color tileFill;
    Color tileOutline;
    Color numberColor;
    Color operatorColor;
    Color highlightColor;

    Color textColor;
    Color scoreColor; 
};

class ThemeManager {
private:
    std::vector<Theme> themes;
    int current = 0;

public:
    ThemeManager();

    const Theme& get() const { return themes[current]; }
    std::string getName() const { return themes[current].name; }

    void next() { current = (current + 1) % themes.size(); }
    void prev() { current = (current + themes.size() - 1) % themes.size(); }
};

