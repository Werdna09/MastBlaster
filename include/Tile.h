#pragma once
#include "Edge.h"
#include "raylib.h"
#include <cstdlib>
#include <array>

struct Tile {
    std::array<Edge, 6> edges; // 6 hran (hex)
    int q = 0, r = 0;          // pozice v hex-gridu
    int rotation = 0;          // 0–5

    Tile();
    void randomize();
    void draw(Vector2 center, float size, bool highlight = false) const;
    void rotateClockwise();
    void rotateCounterClockwise();
};

