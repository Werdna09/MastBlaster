#pragma once
#include <string>

enum class EdgeType { None, Number, Operator };

struct Edge {
    EdgeType type = EdgeType::None;
    int value = 0;      // pokud je číslo
    char op = '\0';     // pokud je operátor (+ - * /)

    Edge() = default;
    Edge(int v) : type(EdgeType::Number), value(v) {}
    Edge(char symbol) : type(EdgeType::Operator), op(symbol) {}

    std::string toString() const;
};

