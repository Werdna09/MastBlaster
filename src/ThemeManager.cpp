#include "ThemeManager.h"

ThemeManager::ThemeManager() {
    themes = {
        {
            "Classic",
            RAYWHITE,                // background
            LIGHTGRAY,               // tileFill
            DARKGRAY,                // tileOutline
            DARKBLUE,                // numberColor
            MAROON,                  // operatorColor
            GOLD,                    // highlightColor
            BLACK,                   // textColor
            MAROON                   // scoreColor
        },
        {
            "Dark",
            Color{25, 25, 35, 255},  // background
            Color{40, 40, 55, 255},  // tileFill
            Color{70, 70, 90, 255},  // tileOutline
            SKYBLUE,                 // numberColor
            ORANGE,                  // operatorColor
            GOLD,                    // highlightColor
            LIGHTGRAY,               // textColor
            GOLD                     // scoreColor
        },
        {
            "Future",
            Color{10, 15, 25, 255},  // background
            Color{20, 30, 60, 255},  // tileFill
            Color{60, 80, 140, 255}, // tileOutline
            Color{80, 200, 255, 255},// numberColor
            Color{255, 160, 60, 255},// operatorColor
            Color{255, 220, 120, 255},// highlightColor
            Color{200, 220, 255, 255},// textColor
            Color{255, 200, 60, 255} // scoreColor
        }
    };
}

