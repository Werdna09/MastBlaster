#pragma once
#include "Edge.h"
#include "Hex.h"

struct Vertex {
    Edge a, b, c;
    Hex ha, hb, hc;

    bool isValid() const;      // 2 čísla + 1 operátor
    int evaluate() const;      // výsledek
    int penaltyValue() const;  // max hodnota
};

