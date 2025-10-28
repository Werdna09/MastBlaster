#include "Board.h"
#include <cmath>
#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

// ====================================================================
//  Umístění dílku na herní plochu + výpočet bodů
// ====================================================================
void Board::placeTile(int q, int r, const Tile &tile) {
    if (gameOver) return;

    Hex pos(q, r);
    if (isOccupied(pos)) return;

    tiles[pos] = tile;

    // vyhodnocení všech vzniklých vortexů kolem nově položeného dílku
    auto vortexes = getVortexesAround(pos);
    int gain = evaluateVortexes(vortexes);
    totalScore += gain;

    ++deckIndex;

    std::cout << "Deck index: " << deckIndex << " / " << tileDeck.size() << std::endl;

    if (deckIndex >= (int)tileDeck.size()) {
        std::cout << "=== GAME OVER ===" << std::endl;
        gameOver = true;
        return;
    }

    std::cout << "Placed tile at (" << q << ", " << r
              << "), gained " << gain
              << " points. Total: " << totalScore << std::endl;
}

// ====================================================================
//  Vykreslení všech položených dílků na herní plochu
// ====================================================================
void Board::draw(Vector2 origin, const Font& font, const Theme& theme) const {
    for (auto &[pos, tile] : tiles) {
        Vector2 pixel = hex_to_pixel(layout, pos);
        pixel.x += origin.x;
        pixel.y += origin.y;
        tile.draw(pixel, layout.size, font, false, theme);  // 🟢 používá aktuální font
    }
}

// ====================================================================
//  Vykreslení šestiúhelníkové mřížky
// ====================================================================
void Board::drawGrid(Vector2 origin, int radius) const {
    for (int q = -radius; q <= radius; ++q) {
        for (int r = std::max(-radius, -q - radius);
             r <= std::min(radius, -q + radius); ++r)
        {
            Hex h(q, r);
            Vector2 p = hex_to_pixel(layout, h);
            p.x += origin.x;
            p.y += origin.y;
            DrawPolyLines(p, 6, layout.size, 30, LIGHTGRAY);
        }
    }
}

// ====================================================================
//  Určení hexu podle pozice myši
// ====================================================================
Hex Board::getHexAtPixel(Vector2 pixel, Vector2 origin) const {
    pixel.x -= origin.x;
    pixel.y -= origin.y;
    return pixel_to_hex(layout, pixel);
}

// ====================================================================
//  Vrátí sousedy hexu (6 směrů)
// ====================================================================
std::vector<Hex> Board::getNeighbors(const Hex &h) const {
    std::vector<Hex> n;
    for (int i = 0; i < 6; ++i)
        n.push_back(hex_neighbor(h, i));
    return n;
}

// ====================================================================
//  Vygeneruje zásobník všech dílků pro hru
// ====================================================================
void Board::generateTileDeck(int count) {
    tileDeck.clear();
    tileDeck.reserve(count);

    for (int i = 0; i < count; ++i) {
        Tile t;
        t.randomize();
        tileDeck.push_back(t);
    }

    tileStack.clear();
    for (int i = 0; i < 3 && i < count; ++i)
        tileStack.push_back(tileDeck[i]);

    deckIndex = 3;
}

// ====================================================================
// 🧩 Zobrazení zásobníku dlaždic (vpravo od hrací plochy)
// ====================================================================
void Board::drawTileStack(Vector2 origin, const Font& font, const Theme& theme) const {
    float spacing = layout.size * 2.5f;
    Vector2 base = {origin.x + 400, origin.y - 100};

    // text „Deck: X / Y“
    std::string deckText = TextFormat("Deck: %d / %d",
        (int)tileDeck.size() - deckIndex,
        (int)tileDeck.size());
    DrawTextEx(font, deckText.c_str(),
               {base.x - 20, base.y - 100}, 20, 0, DARKGRAY);

    // text „Next tiles:“
    DrawTextEx(font, "Next tiles:",
               {base.x - 20, base.y - 70}, 20, 0, DARKGRAY);

    // náhled následujících dílků
    for (int i = 0; i < previewCount; ++i) {
        int idx = deckIndex + i;
        if (idx >= (int)tileDeck.size()) break;

        Vector2 pos = {base.x, base.y + i * spacing};
        bool selected = (i == 0); // první je aktivní tile
        tileDeck[idx].draw(pos, layout.size, font, selected, theme);  //  font
    }
}

// ====================================================================
//  Reakce na kliknutí myší (na herní plochu i zásobník)
// ====================================================================
void Board::handleClick(Vector2 mouse, Vector2 origin) {
    // --- Kliknutí na hex na hrací ploše ---
    Hex hex = getHexAtPixel(mouse, origin);
    Vector2 p = hex_to_pixel(layout, hex);
    p.x += origin.x;
    p.y += origin.y;

    // Kontrola hranic herní desky
    int R = 4;
    int s = -hex.q - hex.r;
    if (std::abs(hex.q) > R || std::abs(hex.r) > R || std::abs(s) > R) {
        std::cout << "Out of bounds (" << hex.q << ", " << hex.r << ")\n";
        return;
    }

    float dx = mouse.x - p.x;
    float dy = mouse.y - p.y;
    if (std::sqrt(dx * dx + dy * dy) < layout.size) {
        if (isOccupied(hex)) return;

        Tile chosen = tileDeck[deckIndex];
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

// ====================================================================
//  Pomocné funkce
// ====================================================================
bool Board::isOccupied(const Hex& h) const {
    return tiles.find(h) != tiles.end();
}

void Board::rotateSelectedTile(bool counter) {
    if (tileDeck.empty() || deckIndex >= (int)tileDeck.size()) return;

    if (counter)
        tileDeck[deckIndex].rotateCounterClockwise();
    else
        tileDeck[deckIndex].rotateClockwise();
}

// ====================================================================
//  Vyhledání všech „vortexů“ kolem zadaného dílku
// ====================================================================
std::vector<Vertex> Board::getVortexesAround(const Hex &h) const {
    std::vector<Vertex> result;
    auto neighbors = getNeighbors(h);

    auto itC = tiles.find(h);
    if (itC == tiles.end()) return result;
    const Tile &center = itC->second;

    for (int i = 0; i < 6; ++i) {
        Hex left = neighbors[i];
        Hex right = neighbors[(i + 1) % 6];

        auto itL = tiles.find(left);
        auto itR = tiles.find(right);
        if (itL == tiles.end() || itR == tiles.end()) continue;

        const Tile &tileL = itL->second;
        const Tile &tileR = itR->second;

        Edge eC = center.edges[i];
        Edge eL = tileL.edges[(i + 2) % 6];
        Edge eR = tileR.edges[(i + 4) % 6];

        result.push_back(Vertex{eC, eL, eR, h, left, right});
    }

    return result;
}

// ====================================================================
//  Vyhodnocení všech nových vortexů (bodování)
// ====================================================================
int Board::evaluateVortexes(const std::vector<Vertex> &vortexes) {
    int sum = 0;
    for (auto &v : vortexes) {
        auto triple = std::array<Hex, 3>{v.ha, v.hb, v.hc};
        std::sort(triple.begin(), triple.end(),
                  [](const Hex &x, const Hex &y){
                      return std::tie(x.q, x.r, x.s) < std::tie(y.q, y.r, y.s);
                  });

        auto key = std::make_tuple(triple[0], triple[1], triple[2]);

        if (evaluatedVortexSet.find(key) != evaluatedVortexSet.end())
            continue;
        evaluatedVortexSet.insert(key);

        if (v.isValid())
            sum += v.evaluate();
        else
            sum -= v.penaltyValue();
    }
    return sum;
}

// ====================================================================
//  Hover efekt (zvýraznění místa pod kurzorem)
// ====================================================================
void Board::updateHover(Vector2 mouse, Vector2 origin) {
    Hex h = getHexAtPixel(mouse, origin);

    int s = -h.q - h.r;
    int R = 4;
    if (std::abs(h.q) > R || std::abs(h.r) > R || std::abs(s) > R) {
        hoverHex.reset();
        return;
    }

    hoverHex = h;
}

void Board::drawHoverHighlight(Vector2 origin) const {
    if (!hoverHex.has_value()) return;

    Hex h = hoverHex.value();
    Vector2 pos = hex_to_pixel(layout, h);
    pos.x += origin.x;
    pos.y += origin.y;

    DrawPolyLines(pos, 6, layout.size * 1.02f, 30, Fade(ORANGE, 0.6f));
}

// ====================================================================
//  Vykreslení „ducha“ dílku pod kurzorem (preview)
// ====================================================================
void Board::drawGhostTile(Vector2 origin, const Font& font, const Theme& theme) const {
    if (!hoverHex.has_value() || tileDeck.empty() || deckIndex >= (int)tileDeck.size())
        return;

    Hex h = hoverHex.value();
    if (isOccupied(h)) return;

    Vector2 p = hex_to_pixel(layout, h);
    p.x += origin.x;
    p.y += origin.y;

    float ghostScale = 0.98f;

    const Tile &ghostTile = tileDeck[deckIndex];
    ghostTile.draw(p, layout.size * ghostScale, font, false, theme); // Aktuální font
    DrawPolyLines(p, 6, layout.size, 30, Fade(DARKGRAY, 0.5f));
}

