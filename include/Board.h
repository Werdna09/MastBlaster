#pragma once
#include "Hex.h"
#include "Tile.h"
#include "Vertex.h"
#include <map>
#include <raylib.h>

class Board {
private:
    std::map<Hex, Tile> tiles;
    int totalScore = 0;
    HexLayout layout{40.0f, true}; // velikost + pointy orientation

public:
    void placeTile(int q, int r, const Tile &tile);
    void draw(Vector2 origin) const;
    void drawGrid(Vector2 origin, int radius = 4) const;
    Hex getHexAtPixel(Vector2 pixel, Vector2 origin) const;

    int getScore() const { return totalScore; }

private:
    std::vector<Hex> getNeighbors(const Hex &h) const;
};

