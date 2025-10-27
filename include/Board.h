#pragma once
#include "Hex.h"
#include "Tile.h"
#include "Vertex.h"
#include <optional>
#include <map>
#include <set>
#include <tuple>
#include <vector>
#include <raylib.h>

class Board {
private:
    std::map<Hex, Tile> tiles;
    int totalScore = 0;
    HexLayout layout{40.0f, true};

    std::vector<Tile> tileStack;
    std::vector<Tile> tileDeck;
    int selectedIndex = 0;
    int deckIndex = 0;
    int previewCount = 3;

    bool gameOver = false;
    
    std::set<std::tuple<Hex, Hex, Hex>> evaluatedVortexSet;

    mutable std::optional<Hex> hoverHex;

public:
    // --- základní funkce ---
    void placeTile(int q, int r, const Tile &tile);
    void draw(Vector2 origin) const;
    void drawGrid(Vector2 origin, int radius = 4) const;
    // --- okey toto nevím proč jsem tady měl --- ====>  void drawGhostTile(Vector2 origin) const;
    Hex getHexAtPixel(Vector2 pixel, Vector2 origin) const;

    int getScore() const { return totalScore; }
    bool isDeckEmpty() const { return deckIndex >= tileDeck.size(); }
    bool isGameOver() const {return gameOver; }


    // --- zásobník / balíček ---
    void generateTileDeck(int count = 50);
    void drawTileStack(Vector2 origin) const;
    void handleClick(Vector2 mouse, Vector2 origin);

    // --- tvl nějaký hover už na to seru na ty popisy, stejně, když mám error, tak to fouknu do ChatGPT ---
    void updateHover(Vector2 mouse, Vector2 origin);
    void drawHoverHighlight(Vector2 origin) const;
    void drawGhostTile(Vector2 origin) const;

    // --- rotace ---
    void rotateSelectedTile(bool counter = false);

private:
    std::vector<Hex> getNeighbors(const Hex &h) const;
    bool isOccupied(const Hex &h) const;
    std::vector<Vertex> getVortexesAround(const Hex &h) const;
    int evaluateVortexes(const std::vector<Vertex> &vortexes);
};
