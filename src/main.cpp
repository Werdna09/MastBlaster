#include <iostream>
#include <ctime>
#include "Edge.h"
#include "Tile.h"
#include "Vertex.h"
#include "raylib.h"

int main() {
    InitWindow(800, 600, "Mathmosaic v0.2.0 – Logic test");
    SetTargetFPS(60);

    srand(time(nullptr));
    Edge e1(3);
    Edge e2(5);
    Edge e3('+');
    Vertex v{e1, e2, e3};

    int result = v.evaluate();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Mathmosaic v0.2.0", 250, 260, 30, DARKBLUE);
        DrawText(TextFormat("Valid: %s", v.isValid() ? "true" : "false"), 300, 320, 20, BLACK);
        DrawText(TextFormat("Result: %i", result), 300, 350, 20, BLACK);
        DrawText(TextFormat("Penalty: %i", v.penaltyValue()), 300, 380, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

