#pragma once
#include "Edge.h"

struct Vertex {
    Edge a, b, c;

    bool isValid() const;      // 2 čísla + 1 operátor
    int evaluate() const;      // výsledek
    int penaltyValue() const;  // max hodnota
};

