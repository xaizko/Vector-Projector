#include <raylib.h>

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 800 
#define TITLE "Vector Viewer"

int main() {
    const int window_width =  WINDOW_WIDTH; 
    const int window_height =  WINDOW_HEIGHT; 

    InitWindow(window_width, window_height, TITLE);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	EndDrawing();
    }

    CloseWindow();

    return 0;
}
