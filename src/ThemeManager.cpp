#include "ThemeManager.h"

ThemeManager::ThemeManager() {
    themes = {
        // 🎨 1. Neutrální světle šedý motiv
        {
            "Classic",
            Color{245, 245, 245, 255},   // background
            Color{220, 220, 220, 255},   // tile fill
            DARKGRAY,                    // tile outline
            SKYBLUE,                     // number color
            MAROON,                      // operator color
            GOLD                         // highlight
        },

        // 🌈 2. Neon – výrazné barvy
        {
            "Neon",
            Color{10, 10, 20, 255},      // background
            Color{25, 25, 50, 255},      // tile fill
            Color{100, 255, 200, 255},   // outline
            Color{0, 255, 150, 255},     // numbers
            Color{255, 100, 150, 255},   // operators
            Color{255, 200, 0, 255}      // highlight
        },

        // 🕯️ 3. Dark Fantasy – kontrastní teplý styl
        {
            "Dark Fantasy",
            Color{20, 10, 5, 255},       // background
            Color{40, 20, 10, 255},      // tile fill
            Color{100, 80, 50, 255},     // outline
            Color{255, 230, 180, 255},   // numbers
            Color{255, 130, 80, 255},    // operators
            Color{255, 200, 60, 255}     // highlight
        }
    };
}

