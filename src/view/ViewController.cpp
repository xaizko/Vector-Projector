#include "../controller/VectorController.cpp"

#define WINDOW_WIDTH 800 
#define WINDOW_HEIGHT 800 
#define TITLE "Vector Viewer"

VectorController vectorController;
Vector3 origin;
Vector3 v1, v2;
Operations operation;

// Grid settings?

// Camera Object
Camera3D camera;

Color generateRandColor() {
    Color randomColor = (Color){
        (unsigned char)GetRandomValue(0, 255), // Explicit cast to Red
        (unsigned char)GetRandomValue(0, 255), // Explicit cast to Green
        (unsigned char)GetRandomValue(0, 255), // Explicit cast to Blue
        255                                    // Alpha
    };

    return randomColor;
} 

void init() {
	const int window_width =  WINDOW_WIDTH; 
	const int window_height =  WINDOW_HEIGHT; 

	InitWindow(window_width, window_height, TITLE);

	camera = { 0 };
	camera.projection = CAMERA_PERSPECTIVE;
	DisableCursor();

	SetTargetFPS(60);

	vectorController = VectorController();
}

void run() {
	init();

	while (!WindowShouldClose()) {
		// Update
		vectorController.setV1(v1.x, v1.y, v1.z);
		vectorController.setV2(v2.x, v2.y, v2.z);
		vectorController.performOperation(operation);

		BeginDrawing();
			ClearBackground(RAYWHITE);

			// Draw Vectors
			BeginMode3D(camera);
				DrawGrid(10, 1.0f);

				DrawLine3D(origin, v1, generateRandColor());
				DrawLine3D(origin, v2, generateRandColor());
			EndMode3D();
		EndDrawing();
	}

	CloseWindow();
}

