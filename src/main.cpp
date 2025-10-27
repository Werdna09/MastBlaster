#include "Board.h"
#include "raylib.h"
#include <ctime>

enum class AppState {MENU, PLAYING, SETTINGS, GAMEOVER, EXITING};

struct Button {
    Rectangle rect;
    const char* label;
};

static bool drawButton(const Button& b) {
    Vector2 m = GetMousePosition();
    bool hover = CheckCollisionPointRec(m, b.rect);
    DrawRectangleRec(b.rect, hover ? Color{200,200,200,255} : Color{200,200,200,255});
    DrawRectangleLinesEx(b.rect, 2, DARKGRAY);

    int tw = MeasureText(b.label, 24);
    DrawText(b.label, b.rect.x + (b.rect.width - tw)/2, b.rect.y + 10, 24, BLACK);

    return hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON);
}

int main() {
    InitWindow(1280, 800, "Mathmosaic v0.4.0 – Visual Grid");
    SetTargetFPS(60);

    AppState state = AppState::MENU;

    Board board;
    Vector2 center = {640, 400};

    Button btnPlay      {{540,300,200,48}, "Play"};
    Button btnSettings  {{540,360,200,48}, "Setting"};
    Button btnExit      {{540,420,200,48}, "Exit"};
    // Button btnPlay {{540,300,200,48}, "Play"};

    int gridRadius = 4;


    board.generateTileDeck(50);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == AppState::MENU) {
            // title
            DrawText("MathMosaic", 480, 180, 48, DARKBLUE);
            DrawText("v1.1.0", 620, 230, 20, GRAY);

            if (drawButton(btnPlay)) {
                // nová hra – reset stavu
                board = Board{};
                board.generateTileDeck(50);
                state = AppState::PLAYING;
            }
            if (drawButton(btnSettings)) {
                state = AppState::SETTINGS;
            }
            if (drawButton(btnExit)) {
                state = AppState::EXITING;
            }

        } else if (state == AppState::PLAYING) {
            // vstupy hry
            Vector2 mouse = GetMousePosition();
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                board.handleClick(mouse, center);
                if (board.isGameOver()) {   // nová pomocná metoda
                    state = AppState::GAMEOVER;
                }
            }

            if (IsKeyPressed(KEY_R)) {
                bool counter = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
                board.rotateSelectedTile(counter);
            }

            // návrat do menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                state = AppState::MENU;
            }

            // vykreslení hry
            DrawText("MathMosaic – Play", 440, 20, 24, DARKBLUE);
            DrawText(TextFormat("Score: %d", board.getScore()), 560, 56, 22, BLACK);

            board.drawGrid(center, gridRadius);
            board.draw(center);
            board.drawTileStack(center);

        } else if (state == AppState::SETTINGS) {
            DrawText("Settings", 560, 180, 36, BLACK);
            DrawText("Grid radius:", 520, 260, 20, DARKGRAY);

            // jednoduché +/-
            Rectangle minus = {520, 300, 40, 40};
            Rectangle plus  = {720, 300, 40, 40};
            Rectangle box   = {570, 300, 140, 40};

            auto drawBox = [](Rectangle r, const char* t){
                DrawRectangleRec(r, Color{230,230,230,255});
                DrawRectangleLinesEx(r, 2, DARKGRAY);
                int w = MeasureText(t, 24);
                DrawText(t, r.x + (r.width - w)/2, r.y + 8, 24, BLACK);
            };

            drawBox(minus, "-");
            drawBox(plus,  "+");
            drawBox(box,   TextFormat("%d", gridRadius));

            Vector2 m = GetMousePosition();
            if (CheckCollisionPointRec(m, minus) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                gridRadius = std::max(1, gridRadius - 1);
            if (CheckCollisionPointRec(m, plus) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                gridRadius = std::min(12, gridRadius + 1);

            // tlačítka dole
            Button back {{ 540, 420, 200, 48 }, "Back"};
            if (drawButton(back)) state = AppState::MENU;

        }  else if (state == AppState::GAMEOVER) {
            ClearBackground(RAYWHITE);
            DrawText("GAME OVER", 520, 250, 50, RED);
            DrawText(TextFormat("Final Score: %d", board.getScore()), 530, 330, 30, DARKGRAY);

            Button retry {{540, 420, 200, 48}, "Play again"};
            Button menu  {{540, 480, 200, 48}, "Main Menu"};
            Button exitB {{540, 540, 200, 48}, "Exit"};

            if (drawButton(retry)) {
                board = Board{};
                board.generateTileDeck(50);
                state = AppState::PLAYING;
            }
            if (drawButton(menu)) {
                state = AppState::MENU;
            }
            if (drawButton(exitB)) {
                CloseWindow();
                return 0;
            }
        }  else if (state == AppState::EXITING) {
            EndDrawing();
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
