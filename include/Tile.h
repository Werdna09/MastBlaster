#pragma once
#include "Edge.h"
#include "raylib.h"
#include "ThemeManager.h"
#include <array>
#include <string>

struct Tile {
    std::array<Edge, 6> edges; // 6 hran (hex)
    int q = 0, r = 0;
    int rotation = 0;

    Tile();

    void randomize();
    void draw(Vector2 center, float size, const Font& font, bool highlight, const Theme& theme) const;
    void rotateClockwise();
    void rotateCounterClockwise();
};

