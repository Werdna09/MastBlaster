#include "Board.h"
#include <cmath>
#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

void Board::placeTile(int q, int r, const Tile &tile) {
    if (gameOver) return;

    Hex pos(q, r);
    if (isOccupied(pos)) return;

    tiles[pos] = tile;

    auto vortexes = getVortexesAround(pos);
    int gain = evaluateVortexes(vortexes);
    totalScore += gain;

    ++deckIndex;

    // Trouble šůt
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

// === ZÁSOBNÍK DLAŽDIC ===
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

void Board::drawTileStack(Vector2 origin) const {
    float spacing = layout.size * 2.5f;
    Vector2 base = {origin.x + 400, origin.y - 100};

    DrawText(TextFormat("Deck: %d / %d", (int)tileDeck.size() - deckIndex, (int)tileDeck.size()),
             base.x - 20, base.y - 100, 20, DARKGRAY);
    DrawText("Next tiles:", base.x - 20, base.y - 70, 20, DARKGRAY);

    // zobraz náhled dalších previewCount dílků z decku
    for (int i = 0; i < previewCount; ++i) {
        int idx = deckIndex + i;
        if (idx >= (int)tileDeck.size()) break;

        Vector2 pos = {base.x, base.y + i * spacing};
        bool selected = (i == 0); // první je aktivní tile
        tileDeck[idx].draw(pos, layout.size, selected);
    }
}

void Board::handleClick(Vector2 mouse, Vector2 origin) {
    // --- Kliknutí na hex na hrací ploše ---
    Hex hex = getHexAtPixel(mouse, origin);
    Vector2 p = hex_to_pixel(layout, hex);
    p.x += origin.x;
    p.y += origin.y;

    // Kontrola borady, že je hráč v rozsahu
    int R = 4;
    int s = -hex.q - hex.r;
    if (std::abs(hex.q) > R || std::abs(hex.r) > R || std::abs(s) > R) {
        std::cout << "Out of bounds (" << hex.q << ", " << hex.r << ")\n";
        return;
    }

    float dx = mouse.x - p.x;
    float dy = mouse.y - p.y;
    if (std::sqrt(dx * dx + dy * dy) < layout.size) {
        if (isOccupied(hex)) {
            return;
        }
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

std::vector<Vertex> Board::getVortexesAround(const Hex &h) const {
    std::vector<Vertex> result;

    // Hledá sousdy
    auto neighbors = getNeighbors(h);

    // Pokud tile neexistuje (a co když tu je), nic nepočítá
    auto itC = tiles.find(h);
    if (itC == tiles.end()) return result;
    const Tile &center = itC->second;

    // Pro každý roh hledáme (6 kolem hexu) - oprava PSÁT SPRÁVNĚ NEIGHBOR  
    for (int i = 0; i < 6; ++i) {
        // soused vlevo a vpravo od aktuálního rohu
        Hex left = neighbors[i];
        Hex right = neighbors[(i + 1) % 6];

        auto itL = tiles.find(left);
        auto itR = tiles.find(right);

        // musí exisotvat všechny vole pčio au, všechny tiles (centrální + 2 sousedé)
        if (itL == tiles.end() || itR == tiles.end()) continue;

        const Tile &tileL = itL->second;
        const Tile &tileR = itR->second;

        // A teď pozor, jdem si vysvětlit tyto indexy, protože by ses po tom
        // (tak za 30 miniut už ztratil
        // Takže:
        // i = roh smeřem od středu po směru hodinek
        // hrana center[i] míří ven do souseda "i"
        // Hrana tile[(i+2) % 6] míří k centru
        // Hrana tile[(i+4) % 6] míří k centru
        // -> tyto tři hrany se setjaí ve stejném hrou
        
        Edge eC = center.edges[i];
        Edge eL = tileL.edges[(i + 2) % 6];
        Edge eR = tileR.edges[(i + 4) % 6];

        result.push_back(Vertex{eC, eL, eR, h, left, right});
    }

    return result;
}

int Board::evaluateVortexes(const std::vector<Vertex> &vortexes) {
    int sum = 0;
    for (auto &v : vortexes) {
        // vytvoříme setříděný klíč (aby byla kombinace jedinečná bez ohledu na pořadí)
        auto triple = std::array<Hex, 3>{v.ha, v.hb, v.hc};
        std::sort(triple.begin(), triple.end(),
                  [](const Hex &x, const Hex &y){
                      return std::tie(x.q, x.r, x.s) < std::tie(y.q, y.r, y.s);
                  });

        auto key = std::make_tuple(triple[0], triple[1], triple[2]);

        // ⛔ Pokud už byl tento vortex jednou započítán, přeskočíme ho
        if (evaluatedVortexSet.find(key) != evaluatedVortexSet.end())
            continue;
        // ✅ Jinak ho uložíme jako nově spočítaný
        evaluatedVortexSet.insert(key);

        // 📈 Vyhodnocení bodů
        if (v.isValid())
            sum += v.evaluate();
        else
            sum -= v.penaltyValue();
    }
    return sum;
}

