#include "raylib.h"

int main() {
    InitWindow(800, 600, "Mathmosaic: Vertex Nexus");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Mathmosaic project structure works!", 120, 280, 20, DARKBLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

