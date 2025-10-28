#include "FontManager.h"
#include <string>
#include <utility>

void FontManager::loadAll() {
    const std::vector<std::pair<std::string, std::string>> fontData = {
        {"Roboto Mono",     "../assets/fonts/RobotoMono-VariableFont_wght.ttf"},
        {"Fira Mono",       "../assets/fonts/FiraMono-Regular.ttf"},
        {"Source Code Pro", "../assets/fonts/SourceCodePro-VariableFont_wght.ttf"}
    };
    for (const auto &fd : fontData) {   // fd = pair<string, string>
        Font f = LoadFontEx(fd.second.c_str(), 32, 0, 0); // načti font
        SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR); // vyhlazení
        fonts.push_back(f);             // uložíme font
        names.push_back(fd.first);      // uložíme jméno
        TraceLog(LOG_INFO, "Loaded font: %s", fd.second.c_str());

    }
}

void FontManager::next() {
    current = (current + 1) % fonts.size();
}

void FontManager::prev() {
    current = (current + fonts.size() - 1) % fonts.size();
}

void FontManager::unload() {
    for (auto &f : fonts) UnloadFont(f);
    fonts.clear();
    names.clear();
}

