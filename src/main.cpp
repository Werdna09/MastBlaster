#include "Board.h"
#include "raylib.h"
#include <ctime>

int main() {
    InitWindow(1280, 800, "Mathmosaic v0.4.0 – Visual Grid");
    SetTargetFPS(60);
    srand(time(nullptr));

    Board board;
    Tile t1, t2, t3;
    board.placeTile(1, 1, t1);
    board.placeTile(1, 0, t2);
    board.placeTile(0, 1, t3);

    Vector2 center = {640, 400};

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Mathmosaic v0.4.0 – Visual Grid", 400, 40, 20, DARKBLUE);
        DrawText(TextFormat("Score: %d", board.getScore()), 560, 80, 25, BLACK);

        // 💠 vykresli mřížku i všechny aktuální tile
        board.drawGrid(center, 4);
        board.draw(center);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

