#pragma once

#include"main.cpp"
void MainMenu(){
    bool exitMenu = false;
    InitWindow(screen_width, screen_height, "Menu");
    Texture2D backgroundImage;
    try{
    backgroundImage = LoadTexture("texture/bac.png");
    if(backgroundImage.id==0){
        throw std::runtime_error("Image not found");
    }
    }catch(const std::exception& e){
        std::cout<<e.what()<<std::endl;
        CloseWindow();
        return;
    }
    Vector2 playButtonPos = { screen_width / 2.0f - 100, screen_height / 1.6f };
    Vector2 quitButtonPos = { screen_width / 2.0f - 100, screen_height / 2.0f + 102 };
    SetTargetFPS(60);
    // Menu loop
    while (!exitMenu && !WindowShouldClose()) {
        BeginDrawing();
        DrawTexture(backgroundImage, 0, 0, WHITE);
        ClearBackground(RAYWHITE);

        // Draw the menu options
        if (MenuOption("        Play", playButtonPos, 20, GREEN)) {
            UnloadTexture(backgroundImage);
            game();
            exitMenu=true;
            return;
        }
        if (MenuOption("        Quit", quitButtonPos, 20, RED)) {
            exitMenu = true;
            UnloadTexture(backgroundImage);
            CloseWindow(); // Quit button was pressed, close the window
            return; // Exit the function to prevent further drawing
        }

        EndDrawing();
    }
    CloseWindow(); // Close the menu window
}