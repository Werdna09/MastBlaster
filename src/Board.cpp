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
#include <cmath>

// === ZÁSOBNÍK DLAŽDIC ===

void Board::generateTileStack(int count) {
    tileStack.clear();
    for (int i = 0; i < count; ++i)
        tileStack.emplace_back();  // volá Tile::Tile() → randomize()
    selectedIndex = 0;
}

void Board::drawTileStack(Vector2 origin) const {
    float spacing = layout.size * 2.5f;
    Vector2 base = {origin.x + 400, origin.y - 100};

    DrawText("Tile Stack:", base.x - 20, base.y - 80, 20, DARKGRAY);

    for (int i = 0; i < (int)tileStack.size(); ++i) {
        Vector2 pos = {base.x, base.y + i * spacing};
        bool selected = (i == selectedIndex);
        tileStack[i].draw(pos, layout.size, selected);
    }
}

void Board::handleClick(Vector2 mouse, Vector2 origin) {
    // --- Kliknutí na hex na hrací ploše ---
    Hex hex = getHexAtPixel(mouse, origin);
    Vector2 p = hex_to_pixel(layout, hex);
    p.x += origin.x;
    p.y += origin.y;

    float dx = mouse.x - p.x;
    float dy = mouse.y - p.y;
    if (std::sqrt(dx * dx + dy * dy) < layout.size) {
        if (isOccupied(hex)) {
            return;
        }
        Tile chosen = tileStack[selectedIndex];
        placeTile(hex.q, hex.r, chosen);
        tileStack[selectedIndex] = Tile(); // nahradíme novým
        return;
    }

    // --- Kliknutí na zásobník ---
    Vector2 base = {origin.x + 400, origin.y - 100};
    float spacing = layout.size * 2.5f;
    for (int i = 0; i < (int)tileStack.size(); ++i) {
        Vector2 pos = {base.x, base.y + i * spacing};
        float dx = mouse.x - pos.x;
        float dy = mouse.y - pos.y;
        if (std::sqrt(dx * dx + dy * dy) < layout.size) {
            selectedIndex = i;
            return;
        }
    }
}

bool Board::isOccupied(const Hex& h) const {
    return tiles.find(h) != tiles.end();
}

void Board::rotateSelectedTile(bool counter) {
    if (tileStack.empty()) return;

    if (counter)
        tileStack[selectedIndex].rotateCounterClockwise();
    else
        tileStack[selectedIndex].rotateClockwise();
}

