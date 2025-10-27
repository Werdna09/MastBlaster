#include "Vertex.h"
#include <algorithm>

bool Vertex::isValid() const {
    int nums = 0, ops = 0;
    for (auto e : {a, b, c}) {
        if (e.type == EdgeType::Number) nums++;
        if (e.type == EdgeType::Operator) ops++;
    }
    return (nums == 2 && ops == 1);
}

int Vertex::evaluate() const {
    if (!isValid()) return 0;

    int n1 = 0, n2 = 0;
    char op = '+';

    for (auto e : {a, b, c}) {
        if (e.type == EdgeType::Number) {
            if (n1 == 0) n1 = e.value;
            else n2 = e.value;
        } else if (e.type == EdgeType::Operator) op = e.op;
    }

    switch (op) {
        case '+': return n1 + n2;
        case '-': return n1 - n2;
        case '*': return n1 * n2;
        case '/': return n2 != 0 ? n1 / n2 : 0;
    }
    return 0;
}

int Vertex::penaltyValue() const {
    int maxVal = 0;
    for (auto e : {a, b, c}) {
        if (e.type == EdgeType::Number) maxVal = std::max(maxVal, e.value);
    }
    return maxVal;
}

