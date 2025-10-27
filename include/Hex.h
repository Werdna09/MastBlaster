#pragma once
#include <cmath>
#include <vector>
#include <tuple>
#include <raylib.h>

struct Hex {
    int q, r, s;
    Hex(int q_, int r_) : q(q_), r(r_), s(-q_ - r_) {}
    Hex(int q_, int r_, int s_) : q(q_), r(r_), s(s_) {}

    bool operator<(const Hex &other) const {
        return std::tie(q, r, s) < std::tie(other.q, other.r, other.s);
    }

    bool operator==(const Hex &other) const {
        return q == other.q && r == other.r && s == other.s;
    }
};

static const std::vector<Hex> HEX_DIRECTIONS = {
    {+1, 0, -1}, {+1, -1, 0}, {0, -1, +1},
    {-1, 0, +1}, {-1, +1, 0}, {0, +1, -1}
};

inline Hex hex_add(const Hex &a, const Hex &b) {
    return {a.q + b.q, a.r + b.r, a.s + b.s};
}

inline Hex hex_neighbor(const Hex &hex, int direction) {
    const auto &dir = HEX_DIRECTIONS[direction % 6];
    return hex_add(hex, dir);
}

inline int hex_length(const Hex &h) {
    return (std::abs(h.q) + std::abs(h.r) + std::abs(h.s)) / 2;
}

inline int hex_distance(const Hex &a, const Hex &b) {
    return hex_length({a.q - b.q, a.r - b.r, a.s - b.s});
}

/* ---------- Layout ---------- */

struct HexLayout {
    float size;
    bool pointy;
};

/* ---------- Conversion ---------- */

inline Vector2 hex_to_pixel(const HexLayout &layout, const Hex &h) {
    float size = layout.size;
    if (layout.pointy) {
        float x = size * std::sqrt(3.0f) * (h.q + h.r / 2.0f);
        float y = size * 1.5f * h.r;
        return {x, y};
    } else {
        float x = size * 1.5f * h.q;
        float y = size * std::sqrt(3.0f) * (h.r + h.q / 2.0f);
        return {x, y};
    }
}

inline Hex pixel_to_hex(const HexLayout &layout, Vector2 p) {
    float size = layout.size;
    float q, r;
    if (layout.pointy) {
        q = (std::sqrt(3.0f)/3.0f * p.x - 1.0f/3.0f * p.y) / size;
        r = (2.0f/3.0f * p.y) / size;
    } else {
        q = (2.0f/3.0f * p.x) / size;
        r = (-1.0f/3.0f * p.x + std::sqrt(3.0f)/3.0f * p.y) / size;
    }

    float s = -q - r;
    int rq = std::round(q);
    int rr = std::round(r);
    int rs = std::round(s);

    float dq = std::abs(rq - q);
    float dr = std::abs(rr - r);
    float ds = std::abs(rs - s);

    if (dq > dr && dq > ds) rq = -rr - rs;
    else if (dr > ds) rr = -rq - rs;
    else rs = -rq - rr;

    return Hex(rq, rr, rs);
}

