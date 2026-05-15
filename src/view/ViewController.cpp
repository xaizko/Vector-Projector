#include "../controller/VectorController.cpp"
#include <raygui.h>
#include <string>

#define RAYGUI_IMPLEMENTATION
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define TITLE "Vector Viewer"

constexpr int TEXT_BUFFER_SIZE = 32;

VectorController vectorController;
Vector3 origin, v1, v2;
Operations operation;
int activeInputField = -1;
int selectedOperationIndex = 0;
bool operationDropdownOpen = false;

// Labels
enum InputField {
	INPUT_V1_X,
	INPUT_V1_Y,
	INPUT_V1_Z,
	INPUT_V2_X,
	INPUT_V2_Y,
	INPUT_V2_Z,
	INPUT_COUNT
};

std::string v1XText = "0";
std::string v1YText = "0";
std::string v1ZText = "0";
std::string v2XText = "0";
std::string v2YText = "0";
std::string v2ZText = "0";

// Grid settings?

// Camera Object
Camera3D camera;

// Helpers
float parseFieldValue(const std::string &text) {
	if (text.empty() || text == "-" || text == "+" || text == ".") {
		return 0.0f;
	}

	try {
		return std::stof(text);
	} catch (...) {
		return 0.0f;
	}
}

void syncVectorText(Vector3 vector, std::string &xText, std::string &yText, std::string &zText) {
	xText = TextFormat("%.2f", vector.x);
	yText = TextFormat("%.2f", vector.y);
	zText = TextFormat("%.2f", vector.z);
}

void setOperationFromIndex(int index) {
	selectedOperationIndex = index;
	operation = static_cast<Operations>(index);
}

void appendInputCharacter(std::string &buffer, int character) {
	if (buffer == "0" && character >= '0' && character <= '9') {
		buffer = static_cast<char>(character);
		return;
	}

	if (buffer.size() < TEXT_BUFFER_SIZE - 1) {
		buffer.push_back(static_cast<char>(character));
	}
}

void drawTextInput(Rectangle bounds, const char *label, std::string &buffer, bool isActive) {
	DrawText(label, static_cast<int>(bounds.x - 18), static_cast<int>(bounds.y + 8), 18, DARKGRAY);
	DrawRectangleRec(bounds, RAYWHITE);
	DrawRectangleLinesEx(bounds, 2, isActive ? MAROON : LIGHTGRAY);
	DrawText(buffer.c_str(), static_cast<int>(bounds.x + 10), static_cast<int>(bounds.y + 8), 18, DARKGRAY);

	if (isActive) {
		int textWidth = MeasureText(buffer.c_str(), 18);
		DrawLine(static_cast<int>(bounds.x + 10 + textWidth + 1), static_cast<int>(bounds.y + 7),
				 static_cast<int>(bounds.x + 10 + textWidth + 1), static_cast<int>(bounds.y + bounds.height - 7),
				 DARKGRAY);

		int character = GetCharPressed();
		while (character > 0) {
			if ((character >= '0' && character <= '9') || character == '-' || character == '+' || character == '.' || character == 'e' || character == 'E') {
				appendInputCharacter(buffer, character);
			}
			character = GetCharPressed();
		}

		if (IsKeyPressed(KEY_BACKSPACE)) {
			if (!buffer.empty()) {
				buffer.pop_back();
				if (buffer.empty()) {
					buffer = "0";
				}
			}
		}
	}
}

void drawOperationDropdown(Rectangle bounds) {
	DrawText("operation", static_cast<int>(bounds.x - 12), static_cast<int>(bounds.y + 8), 18, DARKGRAY);

	if (GuiDropdownBox(bounds, "add;subtract;dot product;cross product", &selectedOperationIndex, operationDropdownOpen)) {
		operationDropdownOpen = !operationDropdownOpen;
		activeInputField = -1;
	}

	setOperationFromIndex(selectedOperationIndex);
}

void updateInputState() {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePosition = GetMousePosition();
		Rectangle v1XBounds = { 130, 120, 85, 34 };
		Rectangle v1YBounds = { 228, 120, 85, 34 };
		Rectangle v1ZBounds = { 326, 120, 85, 34 };
		Rectangle v2XBounds = { 130, 205, 85, 34 };
		Rectangle v2YBounds = { 228, 205, 85, 34 };
		Rectangle v2ZBounds = { 326, 205, 85, 34 };

		if (CheckCollisionPointRec(mousePosition, v1XBounds)) {
			activeInputField = INPUT_V1_X;
		} else if (CheckCollisionPointRec(mousePosition, v1YBounds)) {
			activeInputField = INPUT_V1_Y;
		} else if (CheckCollisionPointRec(mousePosition, v1ZBounds)) {
			activeInputField = INPUT_V1_Z;
		} else if (CheckCollisionPointRec(mousePosition, v2XBounds)) {
			activeInputField = INPUT_V2_X;
		} else if (CheckCollisionPointRec(mousePosition, v2YBounds)) {
			activeInputField = INPUT_V2_Y;
		} else if (CheckCollisionPointRec(mousePosition, v2ZBounds)) {
			activeInputField = INPUT_V2_Z;
		} else {
			activeInputField = -1;
		}
	}
}

void drawVectorDisplay() {
	updateInputState();

	drawTextInput({ 130, 120, 85, 34 }, "x", v1XText, activeInputField == INPUT_V1_X);
	drawTextInput({ 228, 120, 85, 34 }, "y", v1YText, activeInputField == INPUT_V1_Y);
	drawTextInput({ 326, 120, 85, 34 }, "z", v1ZText, activeInputField == INPUT_V1_Z);

	drawTextInput({ 130, 205, 85, 34 }, "x", v2XText, activeInputField == INPUT_V2_X);
	drawTextInput({ 228, 205, 85, 34 }, "y", v2YText, activeInputField == INPUT_V2_Y);
	drawTextInput({ 326, 205, 85, 34 }, "z", v2ZText, activeInputField == INPUT_V2_Z);

	drawOperationDropdown({ 130, 290, 280, 34 });

	v1.x = parseFieldValue(v1XText);
	v1.y = parseFieldValue(v1YText);
	v1.z = parseFieldValue(v1ZText);

	v2.x = parseFieldValue(v2XText);
	v2.y = parseFieldValue(v2YText);
	v2.z = parseFieldValue(v2ZText);

	Vector3 result = vectorController.getResult();

	DrawText("vector 1", 48, 126, 22, DARKGRAY);
	DrawText("vector 2", 48, 211, 22, DARKGRAY);
	DrawText("vector 3", 48, 376, 22, DARKGRAY);

	DrawText("result", 48, 410, 18, GRAY);
	DrawText(TextFormat("<%.2f, %.2f, %.2f>", result.x, result.y, result.z), 130, 404, 20, DARKGRAY);

}

void init() {
	const int window_width =  WINDOW_WIDTH; 
	const int window_height =  WINDOW_HEIGHT; 

	InitWindow(window_width, window_height, TITLE);

	camera = Camera3D{};
	camera.position = (Vector3){ 10.0f, 10.0f, 10.0f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f }; 
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	SetTargetFPS(60);

	vectorController = VectorController();
	setOperationFromIndex(0);
	syncVectorText(v1, v1XText, v1YText, v1ZText);
	syncVectorText(v2, v2XText, v2YText, v2ZText);
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

				DrawLine3D(origin, v1, RED);
				DrawLine3D(origin, v2, BLUE);
				DrawLine3D(origin, vectorController.getResult(), PURPLE);
			EndMode3D();

			// Draw Control GUI
			drawVectorDisplay();
		EndDrawing();
	}

	CloseWindow();
}

