#include "Board.h"
#include "raylib.h"
#include <ctime>

int main() {
    InitWindow(1280, 800, "Mathmosaic v0.4.0 – Visual Grid");
    SetTargetFPS(60);
    srand(time(nullptr));

    Board board;
    board.generateTileDeck(50);

    Vector2 center = {640, 400};

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (IsKeyDown(KEY_R)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
                board.rotateSelectedTile(true);   // proti směru
            else
                board.rotateSelectedTile(false);  // po směru
            // aby se nespamovalo: počkáme krátkou dobu
            WaitTime(0.15);
        }


        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Mathmosaic v0.4.0 – Visual Grid", 400, 40, 20, DARKBLUE);
        DrawText(TextFormat("Score: %d", board.getScore()), 560, 80, 25, BLACK);

        // 💠 vykresli mřížku i všechny aktuální tile
        board.drawGrid(center, 4);
        board.draw(center);
        board.drawTileStack(center);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

