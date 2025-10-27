#pragma once
#include "Tile.h"
#include "Vertex.h"
#include <map>
#include <vector>

struct HexPos {
    int q, r;

    bool operator<(const HexPos &other) const{
        return std::tie(q,r) < std::tie(other.q, other.r);
    }
};

class Board {
private:
    std::map<HexPos, Tile> tiles;
    int totalScore = 0;

public:
    void placeTile(int q, int r, const Tile &tile);
    std::vector<Vertex> getVortexesAround(int q, int r) const;
    int getScore() const {return totalScore;}

private:
    std::vector<HexPos> getNeighbors(int g, int r) const;
    int evaluateVortexes(const std::vector<Vertex> &vortexes);

};
