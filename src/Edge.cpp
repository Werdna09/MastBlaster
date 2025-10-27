#include "Edge.h"
#include <string>

std::string Edge::toString() const {
    if (type == EdgeType::Number) return std::to_string(value);
    if (type == EdgeType::Operator) return std::string(1, op);
    return " ";
}

