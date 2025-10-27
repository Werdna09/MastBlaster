#include "Tile.h"
#include <cstdlib>

Tile::Tile() {
    // náhodná ukázková inicializace
    for (auto &e : edges) {
        int t = rand() % 3;
        if (t == 0) e = Edge(rand() % 13 + 1);    // číslo
        else if (t == 1) {
            const char ops[] = {'+', '-', '*', '/'};
            e = Edge(ops[rand() % 4]);
        }
        else e = Edge(); // prázdná
    }
}

