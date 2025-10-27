#include "Board.h"
#include "raylib.h"
#include <ctime>

int main() {
    InitWindow(900, 700, "Mathmosaic v0.3.0 – Board System");
    SetTargetFPS(60);
    srand(time(nullptr));

    Board board;

    // Umístíme 3 testovací tiles
    Tile t1, t2, t3;
    board.placeTile(0, 0, t1);
    board.placeTile(1, 0, t2);
    board.placeTile(0, 1, t3);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Mathmosaic v0.3.0 – Board System", 180, 40, 20, DARKBLUE);
        DrawText(TextFormat("Score: %d", board.getScore()), 350, 100, 30, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

