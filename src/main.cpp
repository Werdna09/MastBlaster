#include "Board.h"
#include "FontManager.h"
#include "ThemeManager.h"
#include "raylib.h"
#include <ctime>
#include <string>

// ================================================================
// 🎮 Stavy aplikace
// ================================================================
enum class AppState { MENU, MODE_SELECT, PLAYING, PAUSED, SETTINGS, GAMEOVER, EXITING };
enum class GameMode { TILE_LIMIT, TIME_LIMIT };

// Struktura pro jednoduchá tlačítka
struct Button {
    Rectangle rect;
    const char* label;
};

// Pomocná funkce pro vykreslení tlačítka
static bool drawButton(const Button& b, const Font& font) {
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, b.rect);

    // tlačítko + ohraničení
    DrawRectangleRec(b.rect, hover ? Color{210,210,210,255} : Color{230,230,230,255});
    DrawRectangleLinesEx(b.rect, 2, DARKGRAY);

    // text vycentrovaný podle fontu
    int fontSize = 24;
    Vector2 textSize = MeasureTextEx(font, b.label, fontSize, 0);
    Vector2 pos = {
        b.rect.x + (b.rect.width - textSize.x) / 2,
        b.rect.y + (b.rect.height - textSize.y) / 2
    };
    DrawTextEx(font, b.label, pos, fontSize, 0, BLACK);

    return hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

// ================================================================
// 🧩 Hlavní funkce
// ================================================================
int main() {
    InitWindow(1280, 800, "MathMosaic v1.3.0 – Font System");
    SetTargetFPS(60);

    FontManager fontMgr;
    ThemeManager themeMgr;
    fontMgr.loadAll(); // načtení všech fontů

    AppState state = AppState::MENU;
    GameMode mode = GameMode::TILE_LIMIT;
    Board board;
    Vector2 center = {640, 400};
    int gridRadius = 4;
    float timeRemaining = 180.0f;

    // --- základní tlačítka ---
    Button btnPlay     {{540,300,200,48}, "Play"};
    Button btnSettings {{540,360,200,48}, "Settings"};
    Button btnExit     {{540,420,200,48}, "Exit"};

    // --- inicializace balíčku ---
    board.generateTileDeck(50);

    // ================================================================
    // 🔁 Herní smyčka
    // ================================================================
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(themeMgr.get().background);
        const Theme& theme = themeMgr.get();

        // ============================================================
        // 🏠 MENU
        // ============================================================
        if (state == AppState::MENU) {
            DrawTextEx(fontMgr.get(), "MathMosaic", {460, 180}, 48, 0, theme.operatorColor);
            DrawTextEx(fontMgr.get(), "v1.3.0", {620, 235}, 20, 0, theme.textColor);

            if (drawButton(btnPlay, fontMgr.get())) {
                state = AppState::MODE_SELECT;
            }
            if (drawButton(btnSettings, fontMgr.get())) {
                state = AppState::SETTINGS;
            }
            if (drawButton(btnExit, fontMgr.get())) {
                state = AppState::EXITING;
            }
        }

        // ============================================================
        // 🎮 MODE SELECTION (submenu pod "Play")
        // ============================================================
        else if (state == AppState::MODE_SELECT) {
            DrawTextEx(fontMgr.get(), "Select Game Mode", {450, 180}, 40, 0, theme.operatorColor);

            Button btnTile {{540, 300, 200, 48}, "Tile Mode"};
            Button btnTime {{540, 360, 200, 48}, "Time Mode"};
            Button btnBack {{540, 420, 200, 48}, "Back"};

            if (drawButton(btnTile, fontMgr.get())) {
                mode = GameMode::TILE_LIMIT;
                board = Board{};
                board.generateTileDeck(50);
                state = AppState::PLAYING;
                timeRemaining = 180.0f; // <======== Toto možná bude problém, zde zde zde zde zde zde zde z 
            }

            if (drawButton(btnTime, fontMgr.get())) {
                mode = GameMode::TIME_LIMIT;
                board = Board{};
                board.generateTileDeck(90);
                state = AppState::PLAYING;
                timeRemaining = 180.0f;
            }

            if (drawButton(btnBack, fontMgr.get())) {
                state = AppState::MENU;
            }
        }



        // ============================================================
        // 🎲 HRACÍ SCÉNA
        // ============================================================
        else if (state == AppState::PLAYING) {
            Vector2 mouse = GetMousePosition();
            board.updateHover(mouse, center);

            // kliknutí na herní plochu
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                board.handleClick(mouse, center);
                if (mode == GameMode::TILE_LIMIT && board.isGameOver())
                    state = AppState::GAMEOVER;
            }

            if (mode == GameMode::TIME_LIMIT) {
                timeRemaining -= GetFrameTime();
                if (timeRemaining <= 0.0f) {
                    timeRemaining = 0.0f;
                    state = AppState::GAMEOVER;
                }
            }

            // rotace dílku
            if (IsKeyPressed(KEY_R)) {
                bool counter = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
                board.rotateSelectedTile(counter);
            }

            // návrat do menu
            if (IsKeyPressed(KEY_ESCAPE))
                state = AppState::PAUSED;


            // --- UI ---
            if (mode == GameMode::TILE_LIMIT) {
                DrawTextEx(fontMgr.get(), "MathMosaic - Tile mode", {440, 20}, 22, 0, theme.textColor);
                std::string scoreText = TextFormat("Tiles: %d / %d",
                        board.getPlacedTileCount(),
                        board.getTotalTileCount());
                //DrawTextEx(fontMgr.get(), scoreText.c_str(), {560, 56}, 22, 0, theme.scoreColor);
            } else {
                DrawTextEx(fontMgr.get(), "MathMosaic - Time mode", {440, 20}, 22, 0, theme.textColor);
                std::string timeText = TextFormat("Time: %.0f s", timeRemaining, board.getScore());
                DrawTextEx(fontMgr.get(), timeText.c_str(), {800, 56}, 22, 0, theme.scoreColor);
            }


            // --- vykreslení celé hry ---
            // DrawTextEx(fontMgr.get(), "MathMosaic – Play", {440, 20}, 24, 0, theme.operatorColor);
            std::string scoreText = TextFormat("Score: %d", board.getScore());
            DrawTextEx(fontMgr.get(), scoreText.c_str(), {560, 56}, 22, 0, theme.scoreColor);


            board.drawGrid(center, gridRadius);
            board.drawHoverHighlight(center);
            board.drawGhostTile(center, fontMgr.get(), themeMgr.get());
            board.draw(center, fontMgr.get(), themeMgr.get());
            board.drawTileStack(center, fontMgr.get(), themeMgr.get());
        }

        // ============================================================
        // ⏸️ PAUSE MENU (stylové, s rozostřením)
        // ============================================================
        else if (state == AppState::PAUSED) {
            static float pauseFade = 0.0f;
            pauseFade = std::min(1.0f, pauseFade + GetFrameTime() * 2.5f);

            // 🔹 vytvoříme poloprůhledný overlay (jemné “rozostření” efekt)
            // jednoduchý trik – nakreslíme tmavý závoj s mírnou variací průhlednosti
            for (int i = 0; i < GetScreenHeight(); i += 4) {
                Color c = Fade(BLACK, 0.3f + 0.2f * sinf((float)i * 0.01f + GetTime()));
                DrawRectangle(0, i, GetScreenWidth(), 4, Fade(c, pauseFade));
            }

            // 🔹 pulzující nadpis "Paused"
            float pulse = 0.5f + 0.5f * sinf(GetTime() * 2.0f);
            Color titleColor = Fade(WHITE, 0.8f + 0.2f * pulse);

            DrawTextEx(fontMgr.get(), "Paused", {540, 200}, 56, 0, titleColor);

            // 🔹 tlačítka
            Button btnResume  {{540, 320, 200, 48}, "Resume"};
            Button btnRestart {{540, 380, 200, 48}, "Restart"};
            Button btnMenu    {{540, 440, 200, 48}, "Main Menu"};
            Button btnExit    {{540, 500, 200, 48}, "Exit"};

            if (drawButton(btnResume, fontMgr.get())) {
                pauseFade = 0.0f;
                state = AppState::PLAYING;
            }
            if (drawButton(btnRestart, fontMgr.get())) {
                board = Board{};
                board.generateTileDeck(50);
                timeRemaining = 180.0f;
                pauseFade = 0.0f;
                state = AppState::PLAYING;
            }
            if (drawButton(btnMenu, fontMgr.get())) {
                pauseFade = 0.0f;
                state = AppState::MENU;
            }
            if (drawButton(btnExit, fontMgr.get())) {
                CloseWindow();
                return 0;
            }

            //  hint dole
            DrawTextEx(
                    fontMgr.get(),
                    "Press ESC to resume",
                    {530, 580},
                    20, 0,
                    Fade(LIGHTGRAY, 0.7f * pauseFade)
                    );

            DrawTextEx(fontMgr.get(), "Press ESC to resume", {530, 580}, 20, 0,
                    Fade(LIGHTGRAY, 0.7f * pauseFade));
            //  ESC opět obnoví hru
            if (IsKeyPressed(KEY_ESCAPE)) {
                pauseFade = 0.0f;
                state = AppState::PLAYING;
            }
        }


        // ============================================================
        // ⚙ SETTINGS
        // ============================================================
        else if (state == AppState::SETTINGS) {
            DrawTextEx(fontMgr.get(), "Settings", {540, 120}, 40, 0, theme.textColor);

            Vector2 m = GetMousePosition();

            auto drawBox = [&](Rectangle r, const char* t){
                DrawRectangleRec(r, Color{230,230,230,255});
                DrawRectangleLinesEx(r, 2, DARKGRAY);
                Vector2 ts = MeasureTextEx(fontMgr.get(), t, 24, 0);
                DrawTextEx(fontMgr.get(),
                        t,
                        {r.x + (r.width - ts.x)/2, r.y + (r.height - ts.y)/2},
                        24, 0, BLACK);
            };

            // ========================================================
            // GRID SIZE
            // ========================================================
            int yGrid = 220;
            DrawTextEx(fontMgr.get(), "Grid radius:", {520, (float)yGrid}, 22, 0, theme.textColor);

            Rectangle minus = {520, (float)(yGrid + 40), 40, 40};
            Rectangle box   = {570, (float)(yGrid + 40), 140, 40};
            Rectangle plus  = {720, (float)(yGrid + 40), 40, 40};

            drawBox(minus, "-");
            drawBox(plus,  "+");
            drawBox(box,   TextFormat("%d", gridRadius));

            if (CheckCollisionPointRec(m, minus) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                gridRadius = std::max(1, gridRadius - 1);
            if (CheckCollisionPointRec(m, plus) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                gridRadius = std::min(12, gridRadius + 1);


            // ========================================================
            // FONT SELECTION
            // ========================================================
            int yFont = yGrid + 120;
            DrawTextEx(fontMgr.get(), "Font:", {520, (float)yFont}, 22, 0, theme.textColor);

            Rectangle fontLeft  = {520, (float)(yFont + 40), 40, 40};
            Rectangle fontBox   = {570, (float)(yFont + 40), 140, 40};
            Rectangle fontRight = {720, (float)(yFont + 40), 40, 40};

            drawBox(fontLeft,  "<");
            drawBox(fontRight, ">");
            drawBox(fontBox,   fontMgr.getName().c_str());

            static float fontFade = 1.0f;
            static float fontFadeSpeed = 5.0f;

            if (CheckCollisionPointRec(m, fontLeft) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                fontMgr.prev();
                fontFade = 0.0f;
            }
            if (CheckCollisionPointRec(m, fontRight) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                fontMgr.next();
                fontFade = 0.0f;
            }

            if (fontFade < 1.0f) fontFade += GetFrameTime() * fontFadeSpeed;
            if (fontFade > 1.0f) fontFade = 1.0f;

            const char* preview = "123 + × ÷ − = 13";
            Font f = fontMgr.get();
            int fontSize = 28;
            Vector2 textSize = MeasureTextEx(f, preview, fontSize, 0);
            Vector2 pos = {640 - textSize.x / 2, (float)(yFont + 120)};

            DrawRectangle(460, yFont + 100, 360, 55, Fade(LIGHTGRAY, 0.15f * fontFade));
            DrawTextEx(f, preview, pos, fontSize, 0, Fade(DARKBLUE, fontFade));


            // ========================================================
            // THEME SELECTION
            // ========================================================
            int yTheme = yFont + 200;
            DrawTextEx(fontMgr.get(), "Theme:", {520, (float)yTheme}, 22, 0, theme.textColor);

            Rectangle themeLeft  = {520, (float)(yTheme + 40), 40, 40};
            Rectangle themeBox   = {570, (float)(yTheme + 40), 140, 40};
            Rectangle themeRight = {720, (float)(yTheme + 40), 40, 40};

            drawBox(themeLeft,  "<");
            drawBox(themeRight, ">");
            drawBox(themeBox,   themeMgr.getName().c_str());

            if (CheckCollisionPointRec(m, themeLeft) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                themeMgr.prev();
            if (CheckCollisionPointRec(m, themeRight) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                themeMgr.next();


            // ========================================================
            // BACK BUTTON
            // ========================================================
            Button back {{540, (float)(yTheme + 130), 200, 48}, "Back"};
            if (drawButton(back, fontMgr.get()))
                state = AppState::MENU;
        }

        // ============================================================
        // ☠️ GAME OVER
        // ============================================================
        else if (state == AppState::GAMEOVER) {
            ClearBackground(RAYWHITE);

            DrawTextEx(fontMgr.get(), "GAME OVER", {520, 250}, 50, 0, theme.operatorColor);

            // ZDE TOTO JE ASI ZBYTEČNÉ, ZATÍM TO NECHÁM ZAKOMENTOVANÉ, JE TO SICE JEN VYKRESLOVÁNÍ INFA, ALE WHO KNOWS...
            // if (mode == GameMode::TIME_LIMIT)
            //     DrawTextEx(fontMgr.get(), "Time's up!", {540, 320}, 32, 0, theme.operatorColor);
            // else
            //     DrawTextEx(fontMgr.get(), "No tiles left!", {520, 320}, 32, 0, theme.operatorColor);


            std::string finalText = TextFormat("Final Score: %d", board.getScore());
            DrawTextEx(fontMgr.get(), finalText.c_str(), {530, 330}, 30, 0, theme.scoreColor);


            Button retry {{540, 420, 200, 48}, "Play again"};
            Button menu  {{540, 480, 200, 48}, "Main Menu"};
            Button exitB {{540, 540, 200, 48}, "Exit"};

            if (drawButton(retry, fontMgr.get())) {
                board = Board{};
                board.generateTileDeck(50);
                timeRemaining = 180.0f;
                state = AppState::MODE_SELECT;
            }
            if (drawButton(menu, fontMgr.get()))
                state = AppState::MENU;
            if (drawButton(exitB, fontMgr.get())) {
                CloseWindow();
                return 0;
            }
        }

        // ============================================================
        // 🚪 EXITING
        // ============================================================
        else if (state == AppState::EXITING) {
            EndDrawing();
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

