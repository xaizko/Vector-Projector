#include "../controller/VectorController.cpp"
#include <string>

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 1500
#define TITLE "Vector Viewer"

constexpr int TEXT_BUFFER_SIZE = 32;
constexpr int CONTROL_PANEL_WIDTH = 460;
constexpr int SPLIT_LINE_WIDTH = 2;

VectorController vectorController;
Vector3 origin, v1, v2;
Operations operation;

// Operation Controls
int selectedOperationIndex = 0;
bool operationDropdownOpen = false;

// Input Controls
int activeInputField = -1;
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

// Camera Settings
Camera3D camera;

// Helpers
Vector3 logicalToWorld(Vector3 logicalVector) {
	// Map logical coordinates to world so Y remains on the grid plane and Z stays vertical,
	// while visually swapping X/Y placement to match expected screen orientation.
	return { logicalVector.y, logicalVector.z, logicalVector.x };
}

Rectangle getControlPanelBounds() {
	return { 0.0f, 0.0f, static_cast<float>(CONTROL_PANEL_WIDTH), static_cast<float>(GetScreenHeight()) };
}

Rectangle getViewportBounds() {
	return {
		static_cast<float>(CONTROL_PANEL_WIDTH + SPLIT_LINE_WIDTH),
		0.0f,
		static_cast<float>(GetScreenWidth() - CONTROL_PANEL_WIDTH - SPLIT_LINE_WIDTH),
		static_cast<float>(GetScreenHeight())
	};
}

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
	if ((character == '-' || character == '+') && (buffer == "0" || buffer.empty())) {
		buffer = static_cast<char>(character);
		return;
	}

	if (buffer == "0" && character >= '0' && character <= '9') {
		buffer = static_cast<char>(character);
		return;
	}

	if (buffer.size() < TEXT_BUFFER_SIZE - 1) {
		buffer.push_back(static_cast<char>(character));
	}
}

// Draw Labels
void drawAxisTag(Vector2 screenPos, const char *text, Color color, const Rectangle &viewport) {
	if (!CheckCollisionPointRec(screenPos, viewport)) {
		return;
	}

	const int fontSize = 20;
	const int paddingX = 6;
	const int paddingY = 4;
	int textWidth = MeasureText(text, fontSize);
	int drawX = static_cast<int>(screenPos.x) - (textWidth / 2);
	int drawY = static_cast<int>(screenPos.y) - (fontSize / 2);

	DrawRectangle(drawX - paddingX, drawY - paddingY, textWidth + (paddingX * 2), fontSize + (paddingY * 2), Fade(RAYWHITE, 0.85f));
	DrawRectangleLines(drawX - paddingX, drawY - paddingY, textWidth + (paddingX * 2), fontSize + (paddingY * 2), Fade(color, 0.55f));
	DrawText(text, drawX, drawY, fontSize, color);
}

void drawGridAxisLabels(const Rectangle &viewport) {
	Vector3 posXAxisLabelPos = logicalToWorld({ 15.5f, 0.0f, 0.0f });
	Vector3 negXAxisLabelPos = logicalToWorld({ -15.5f, 0.0f, 0.0f });
	Vector3 posYAxisLabelPos = logicalToWorld({ 0.0f, 15.5f, 0.0f });
	Vector3 negYAxisLabelPos = logicalToWorld({ 0.0f, -15.5f, 0.0f });
	Vector3 posZAxisLabelPos = logicalToWorld({ 0.0f, 0.0f, 15.5f });
	Vector3 negZAxisLabelPos = logicalToWorld({ 0.0f, 0.0f, -15.5f });

	Vector2 posXScreen = GetWorldToScreen(posXAxisLabelPos, camera);
	Vector2 negXScreen = GetWorldToScreen(negXAxisLabelPos, camera);
	Vector2 posYScreen = GetWorldToScreen(posYAxisLabelPos, camera);
	Vector2 negYScreen = GetWorldToScreen(negYAxisLabelPos, camera);
	Vector2 posZScreen = GetWorldToScreen(posZAxisLabelPos, camera);
	Vector2 negZScreen = GetWorldToScreen(negZAxisLabelPos, camera);

	drawAxisTag(posXScreen, "+X", RED, viewport);
	drawAxisTag(negXScreen, "-X", RED, viewport);
	drawAxisTag(posYScreen, "+Y", GREEN, viewport);
	drawAxisTag(negYScreen, "-Y", GREEN, viewport);
	drawAxisTag(posZScreen, "+Z", BLUE, viewport);
	drawAxisTag(negZScreen, "-Z", BLUE, viewport);
}

void drawVectorLabels(const Rectangle &viewport, Vector3 result) {
	Vector2 v1Screen = GetWorldToScreen(logicalToWorld(v1), camera);
	Vector2 v2Screen = GetWorldToScreen(logicalToWorld(v2), camera);
	Vector2 rvScreen = GetWorldToScreen(logicalToWorld(result), camera);

	drawAxisTag(v1Screen, "V1", RED, viewport);
	drawAxisTag(v2Screen, "V2", BLUE, viewport);
	drawAxisTag(rvScreen, "RV", PURPLE, viewport);
}

// Draw Controls
void drawTextInput(Rectangle bounds, const char *label, std::string &buffer, bool isActive) {
	int labelWidth = MeasureText(label, 18);
	DrawText(label, static_cast<int>(bounds.x + (bounds.width - labelWidth) * 0.5f), static_cast<int>(bounds.y - 24), 18, DARKGRAY);
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
	if (GuiDropdownBox(bounds, "add;subtract;dot product;cross product", &selectedOperationIndex, operationDropdownOpen)) {
		operationDropdownOpen = !operationDropdownOpen;
		activeInputField = -1;
	}

	setOperationFromIndex(selectedOperationIndex);
}

void updateInputState(const Rectangle inputBounds[INPUT_COUNT], Rectangle dropdownBounds) {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		Vector2 mousePosition = GetMousePosition();

		for (int i = 0; i < INPUT_COUNT; i++) {
			if (CheckCollisionPointRec(mousePosition, inputBounds[i])) {
				activeInputField = i;
				return;
			}
		}

		if (!CheckCollisionPointRec(mousePosition, dropdownBounds)) {
			activeInputField = -1;
		}
	}
}

void drawVectorDisplay(const Rectangle &controlPanel) {
	const float labelX = controlPanel.x + 28.0f;
	const float startX = controlPanel.x + 142.0f;
	const float inputWidth = 78.0f;
	const float inputGap = 14.0f;
	const float row1Y = controlPanel.y + 130.0f;
	const float row2Y = controlPanel.y + 225.0f;
	const float operationY = controlPanel.y + 320.0f;
	const float resultY = controlPanel.y + 440.0f;
	const float legendY = controlPanel.y + 530.0f;

	Rectangle inputBounds[INPUT_COUNT] = {
		{ startX, row1Y, inputWidth, 34 },
		{ startX + (inputWidth + inputGap), row1Y, inputWidth, 34 },
		{ startX + 2.0f * (inputWidth + inputGap), row1Y, inputWidth, 34 },
		{ startX, row2Y, inputWidth, 34 },
		{ startX + (inputWidth + inputGap), row2Y, inputWidth, 34 },
		{ startX + 2.0f * (inputWidth + inputGap), row2Y, inputWidth, 34 },
	};
	Rectangle operationBounds = { startX, operationY, 260, 34 };

	updateInputState(inputBounds, operationBounds);

	drawTextInput(inputBounds[INPUT_V1_X], "x", v1XText, activeInputField == INPUT_V1_X);
	drawTextInput(inputBounds[INPUT_V1_Y], "y", v1YText, activeInputField == INPUT_V1_Y);
	drawTextInput(inputBounds[INPUT_V1_Z], "z", v1ZText, activeInputField == INPUT_V1_Z);

	drawTextInput(inputBounds[INPUT_V2_X], "x", v2XText, activeInputField == INPUT_V2_X);
	drawTextInput(inputBounds[INPUT_V2_Y], "y", v2YText, activeInputField == INPUT_V2_Y);
	drawTextInput(inputBounds[INPUT_V2_Z], "z", v2ZText, activeInputField == INPUT_V2_Z);

	drawOperationDropdown(operationBounds);

	v1.x = parseFieldValue(v1XText);
	v1.y = parseFieldValue(v1YText);
	v1.z = parseFieldValue(v1ZText);

	v2.x = parseFieldValue(v2XText);
	v2.y = parseFieldValue(v2YText);
	v2.z = parseFieldValue(v2ZText);

	Vector3 result = vectorController.getResult();

	DrawText("vector 1", static_cast<int>(labelX), static_cast<int>(row1Y + 6), 22, DARKGRAY);
	DrawText("vector 2", static_cast<int>(labelX), static_cast<int>(row2Y + 6), 22, DARKGRAY);
	DrawText("result", static_cast<int>(labelX), static_cast<int>(resultY), 22, DARKGRAY);
	DrawLine(static_cast<int>(labelX), static_cast<int>(resultY - 16), static_cast<int>(controlPanel.width - 22), static_cast<int>(resultY - 16), LIGHTGRAY);
	DrawText(TextFormat("<%.2f, %.2f, %.2f>", result.x, result.y, result.z), static_cast<int>(startX), static_cast<int>(resultY), 22, DARKGRAY);

	DrawText("operation", static_cast<int>(labelX), static_cast<int>(operationY + 8), 20, DARKGRAY);

	DrawLine(static_cast<int>(labelX), static_cast<int>(legendY - 20), static_cast<int>(controlPanel.width - 22), static_cast<int>(legendY - 20), LIGHTGRAY);
	DrawText("legend", static_cast<int>(labelX), static_cast<int>(legendY - 8), 22, DARKGRAY);

	DrawRectangle(static_cast<int>(labelX), static_cast<int>(legendY + 30), 20, 10, RED);
	DrawText("vector 1", static_cast<int>(labelX + 30), static_cast<int>(legendY + 22), 20, DARKGRAY);

	DrawRectangle(static_cast<int>(labelX), static_cast<int>(legendY + 64), 20, 10, BLUE);
	DrawText("vector 2", static_cast<int>(labelX + 30), static_cast<int>(legendY + 56), 20, DARKGRAY);

	DrawRectangle(static_cast<int>(labelX), static_cast<int>(legendY + 98), 20, 10, PURPLE);
	DrawText("result vector", static_cast<int>(labelX + 30), static_cast<int>(legendY + 90), 20, DARKGRAY);

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

        Rectangle controlPanel = getControlPanelBounds();
        Rectangle viewport = getViewportBounds();

		Vector2 mp = GetMousePosition();
		if (CheckCollisionPointRec(mp, viewport)) {
			UpdateCamera(&camera, CAMERA_THIRD_PERSON);
		}

		BeginDrawing();
			ClearBackground((Color){ 239, 241, 245, 255 });

			DrawRectangleRec(controlPanel, (Color){ 250, 250, 252, 255 });
			DrawRectangle(static_cast<int>(viewport.x), static_cast<int>(viewport.y), static_cast<int>(viewport.width), static_cast<int>(viewport.height), (Color){ 230, 235, 242, 255 });
			DrawRectangle(controlPanel.width, 0, SPLIT_LINE_WIDTH, GetScreenHeight(), (Color){ 192, 197, 208, 255 });

			DrawText("controls", 22, 22, 32, DARKGRAY);
			DrawText("WASD to move, mouse to look, mouse wheel to zoom", static_cast<int>(viewport.x + 18), 18, 18, GRAY);

			// Draw Vectors
			BeginScissorMode(static_cast<int>(viewport.x), static_cast<int>(viewport.y), static_cast<int>(viewport.width), static_cast<int>(viewport.height));
			BeginMode3D(camera);
				DrawGrid(30, 1.0f);
				DrawLine3D(logicalToWorld({ -15.0f, 0.0f, 0.0f }), logicalToWorld({ 15.0f, 0.0f, 0.0f }), RED);
				DrawLine3D(logicalToWorld({ 0.0f, -15.0f, 0.0f }), logicalToWorld({ 0.0f, 15.0f, 0.0f }), GREEN);
				DrawLine3D(logicalToWorld({ 0.0f, 0.0f, -15.0f }), logicalToWorld({ 0.0f, 0.0f, 15.0f }), BLUE);

				DrawLine3D(origin, logicalToWorld(v1), RED);
				DrawLine3D(origin, logicalToWorld(v2), BLUE);
				DrawLine3D(origin, logicalToWorld(vectorController.getResult()), PURPLE);
			EndMode3D();
			drawVectorLabels(viewport, vectorController.getResult());
			drawGridAxisLabels(viewport);
			EndScissorMode();

			// Draw Control GUI
			BeginScissorMode(static_cast<int>(controlPanel.x), static_cast<int>(controlPanel.y), static_cast<int>(controlPanel.width), static_cast<int>(controlPanel.height));
			drawVectorDisplay(controlPanel);
			EndScissorMode();
		EndDrawing();
	}

	CloseWindow();
}
