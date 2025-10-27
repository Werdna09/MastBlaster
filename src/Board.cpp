#include "Board.h"
#include <algorithm>
#include <iostream>

void Board::placeTile(int q, int r, const Tile &tile) {
    HexPos pos{q, r};
    tiles[pos] = tile;

    // Spočítej všechny vortexy okolo nového tile
    auto vortexes = getVortexesAround(q, r);
    int scoreGain = evaluateVortexes(vortexes);
    totalScore += scoreGain;

    std::cout << "Placed tile at (" << q << ", " << r << "), gained " << scoreGain
              << " points. Total: " << totalScore << std::endl;
}

std::vector<HexPos> Board::getNeighbors(int q, int r) const {
    static const std::vector<std::pair<int,int>> directions = {
        {+1, 0}, {+1, -1}, {0, -1},
        {-1, 0}, {-1, +1}, {0, +1}
    };

    std::vector<HexPos> neighbors;
    for (auto [dq, dr] : directions)
        neighbors.push_back({q + dq, r + dr});
    return neighbors;
}

std::vector<Vertex> Board::getVortexesAround(int q, int r) const {
    std::vector<Vertex> result;
    auto neighbors = getNeighbors(q, r);

    // Jednoduchý placeholder – každý tile se zkusí spojit s 2 sousedy
    for (auto &n1 : neighbors) {
        for (auto &n2 : neighbors) {
            if (n1.q == n2.q && n1.r == n2.r) continue;
            auto it1 = tiles.find(n1);
            auto it2 = tiles.find(n2);
            auto itC = tiles.find({q, r});

            if (it1 != tiles.end() && it2 != tiles.end() && itC != tiles.end()) {
                // vezmi náhodné hrany pro test
                Edge e1 = it1->second.edges[0];
                Edge e2 = it2->second.edges[1];
                Edge e3 = itC->second.edges[2];
                result.push_back(Vertex{e1, e2, e3});
            }
        }
    }
    return result;
}

int Board::evaluateVortexes(const std::vector<Vertex> &vortexes) {
    int sum = 0;
    for (auto &v : vortexes) {
        if (v.isValid()) sum += v.evaluate();
        else sum -= v.penaltyValue();
    }
    return sum;
}

