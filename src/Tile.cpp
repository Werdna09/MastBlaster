#include "Tile.h"
#include <cmath>
#include <cstdlib>

Tile::Tile() {
    randomize();
}

void Tile::randomize() {
    const char ops[] = {'+', '-', '*', '/'};
    for (auto &e : edges) {
        int t = rand() % 3; // 0 = číslo, 1 = operátor, 2 = prázdno
        if (t == 0) {
            e = Edge(rand() % 13 + 1);  // náhodné číslo 1–13
        } 
        else if (t == 1) {
            e = Edge(ops[rand() % 4]);  // náhodný operátor
        } 
        else {
            e = Edge();                 // prázdná hrana
        }
    }
}

void Tile::draw(Vector2 center, float size, bool highlight) const {
    Color fill = highlight ? GOLD : LIGHTGRAY;
    Color border = GRAY;

    DrawPoly(center, 6, size, 30, fill);
    DrawPolyLines(center, 6, size, 30, border);

    int sum = 0;
    for (auto &e : edges)
        if (e.type == EdgeType::Number)
            sum += e.value;

    if (sum > 0)
        DrawText(TextFormat("%i", sum), center.x - 10, center.y - 10, 20, DARKBLUE);
}

