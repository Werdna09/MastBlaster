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
    //  nejprve definice struktury
    struct ScorePopup {
        Vector2 pos;
        int value;
        float timer = 0.0f;
    };

    //  až pak pole, které ji používá
    mutable std::vector<ScorePopup> scorePopups;

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
    void draw(Vector2 origin, const Font& font, const Theme& theme) const;
    void drawGrid(Vector2 origin, int radius = 4) const;
    Hex getHexAtPixel(Vector2 pixel, Vector2 origin) const;

    int getScore() const { return totalScore; }
    bool isDeckEmpty() const { return deckIndex >= tileDeck.size(); }
    bool isGameOver() const { return gameOver; }

    // --- zásobník / balíček ---
    void generateTileDeck(int count = 50);
    void drawTileStack(Vector2 origin, const Font& font, const Theme& theme) const;
    void handleClick(Vector2 mouse, Vector2 origin);

    // --- interakce ---
    void updateHover(Vector2 mouse, Vector2 origin);
    void drawHoverHighlight(Vector2 origin) const;
    void drawGhostTile(Vector2 origin, const Font& font, const Theme& theme) const;

    // --- rotace ---
    void rotateSelectedTile(bool counter = false);

    // --- nové pomocné gettery ---
    int getPlacedTileCount() const { return (int)tiles.size(); }
    int getRemainingTileCount() const { return (int)(tileDeck.size() - deckIndex); }
    int getTotalTileCount() const { return (int)tileDeck.size(); }

private:
    std::vector<Hex> getNeighbors(const Hex &h) const;
    bool isOccupied(const Hex &h) const;
    std::vector<Vertex> getVortexesAround(const Hex &h) const;
    int evaluateVortexes(const std::vector<Vertex> &vortexes);
};

