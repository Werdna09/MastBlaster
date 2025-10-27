#include "Board.h"
#include <cmath>
#include <iostream>

void Board::placeTile(int q, int r, const Tile &tile) {
    Hex pos(q, r);
    tiles[pos] = tile;
    std::cout << "Placed tile at (" << q << ", " << r << ")\n";
}

void Board::draw(Vector2 origin) const {
    for (auto &[pos, tile] : tiles) {
        Vector2 pixel = hex_to_pixel(layout, pos);
        pixel.x += origin.x;
        pixel.y += origin.y;
        tile.draw(pixel, layout.size);
    }
}

void Board::drawGrid(Vector2 origin, int radius) const {
    for (int q = -radius; q <= radius; ++q) {
        for (int r = std::max(-radius, -q - radius); r <= std::min(radius, -q + radius); ++r) {
            Hex h(q, r);
            Vector2 p = hex_to_pixel(layout, h);
            p.x += origin.x;
            p.y += origin.y;
            DrawPolyLines(p, 6, layout.size, 30, LIGHTGRAY);
        }
    }
}

Hex Board::getHexAtPixel(Vector2 pixel, Vector2 origin) const {
    pixel.x -= origin.x;
    pixel.y -= origin.y;
    return pixel_to_hex(layout, pixel);
}

std::vector<Hex> Board::getNeighbors(const Hex &h) const {
    std::vector<Hex> n;
    for (int i = 0; i < 6; ++i)
        n.push_back(hex_neighbor(h, i));
    return n;
}

