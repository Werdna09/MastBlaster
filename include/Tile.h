#pragma once
#include "Edge.h"
#include <array>

struct Tile {
    std::array<Edge, 6> edges; // 6 hran (hex)
    int q = 0, r = 0;          // pozice v hex-gridu
    int rotation = 0;          // 0–5

    Tile();
};

