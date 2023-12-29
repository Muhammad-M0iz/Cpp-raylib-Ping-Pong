#pragma once


const int screen_width = 800;
const int screen_height = 450;
int player_score = 0;
int cpu_score = 0;
int game();
void mainGame();
bool MenuOption(const char* title, Vector2 position, int fontSize, Color color);
Sound hit;
Sound win;
Sound score;
Color Green = Color{0, 228, 48, 255};
Color Dark_Green = Color{0, 228, 48, 255};
Color Light_Green = Color{0, 117, 44, 255};
Color Yellow = Color{230, 41, 55, 255 };
//initial values for parameters
