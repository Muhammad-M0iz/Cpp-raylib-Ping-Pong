// MenuOption.cpp
#include "MenuOption.h"

bool MenuOption(const char* title, Vector2 position, int fontSize, Color color) {
     Rectangle rec = { position.x, position.y, 200, 40 };
    bool clicked = false;

    // Check for button press
    if (CheckCollisionPointRec(GetMousePosition(), rec) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        clicked = true;
    }

    // Draw the button
        DrawRectangleRounded(rec, 9.0f, 0, color);

    DrawText(title, position.x + 10, position.y + 10, fontSize, WHITE);

    return clicked;
}