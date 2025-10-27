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
    // Barvy
    Color fill = highlight ? GOLD : LIGHTGRAY;
    Color border = GRAY;

    // Hlavní šestiúhelník
    DrawPoly(center, 6, size, 30, fill);
    DrawPolyLines(center, 6, size, 30, border);

    // --- vykreslení hran (edges) ---
    for (int i = 0; i < 6; ++i) {
        const Edge &e = edges[i];
        if (e.type == EdgeType::None) continue;

        // úhel středu hrany (pro pointy-top orientaci)
        float angle_deg = 30 + i * 60;
        float angle_rad = angle_deg * DEG2RAD;

        // pozice textu kolem hexu
        float textRadius = size * 0.7f;
        Vector2 pos = {
            center.x + textRadius * cosf(angle_rad),
            center.y - textRadius * sinf(angle_rad)
        };

        // text podle typu hrany
        std::string text = e.toString();
        int textWidth = MeasureText(text.c_str(), 20);

        Color textColor = (e.type == EdgeType::Number) ? DARKBLUE : MAROON;
        DrawText(text.c_str(), pos.x - textWidth / 2, pos.y - 10, 20, textColor);
    }
}

void Tile::rotateClockwise() {
    Edge last = edges.back();
    for (int i = 5; i > 0; --i) {
        edges[i] = edges[i - 1];
    }
    edges[0] = last;

    rotation = (rotation + 1) % 6;
}

void Tile::rotateCounterClockwise() {
    Edge first = edges.front();
    for (int i = 0; i < 5; ++i) {
        edges[i] = edges[i + 1];
    }
    edges[5] = first;

    rotation = (rotation + 5) % 6;


}
