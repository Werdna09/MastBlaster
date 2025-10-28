#include "Tile.h"
#include <cmath>
#include <cstdlib>

// ----------------------------------------------------
// Konstruktor + randomizace
// ----------------------------------------------------
Tile::Tile() {
    randomize();
}

void Tile::randomize() {
    const char ops[] = {'+', '-', '*', '/'};
    for (auto &e : edges) {
        int roll = rand() % 100;
        if (roll < 50) {
            e = Edge(); // prázdná hrana
        }
        else if (roll < 80) {
            e = Edge(rand() % 13 + 1); // číslo
        }
        else {
            e = Edge(ops[rand() % 4]); // operátor
        }
    }
}

// ----------------------------------------------------
// Vykreslení celé destičky
// ----------------------------------------------------
void Tile::draw(Vector2 center, float size, const Font& font, bool highlight, const Theme& theme) const {
    Color fill = highlight ? theme.highlightColor : theme.tileFill;
    DrawPoly(center, 6, size, 30, fill);
    DrawPolyLines(center, 6, size, 30, theme.tileOutline);

    for (int i = 0; i < 6; ++i) {
        const Edge &e = edges[i];
        if (e.type == EdgeType::None) continue;

        float angle_deg = 30 + i * 60;
        float angle_rad = angle_deg * DEG2RAD;

        float textRadius = size * 0.7f;
        Vector2 pos = {
            center.x + textRadius * cosf(angle_rad),
            center.y - textRadius * sinf(angle_rad)
        };

        std::string text = e.toString();
        int fontSize = (int)(size * 0.45f);
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 0);

        Color textColor = (e.type == EdgeType::Number)
            ? theme.numberColor
            : theme.operatorColor;

        DrawTextEx(font, text.c_str(),
                   {pos.x - textSize.x / 2, pos.y - textSize.y / 2},
                   fontSize, 0, textColor);
    }
}


// ----------------------------------------------------
// Rotace tile
// ----------------------------------------------------
void Tile::rotateClockwise() {
    Edge last = edges.back();
    for (int i = 5; i > 0; --i)
        edges[i] = edges[i - 1];
    edges[0] = last;
    rotation = (rotation + 1) % 6;
}

void Tile::rotateCounterClockwise() {
    Edge first = edges.front();
    for (int i = 0; i < 5; ++i)
        edges[i] = edges[i + 1];
    edges[5] = first;
    rotation = (rotation + 5) % 6;
}

