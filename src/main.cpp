#pragma once
#include<raylib.h>
#include <iostream>
#include"functions.h"
#include <stdexcept>
#include "MenuOption.h"
#include <vector>
#include"MainMenu.h"

//abstract class for game objects
class GameObject {
public:
virtual ~GameObject() {}
    virtual void Update() = 0;
    virtual void Draw() const = 0;
    
};
//ball class (inherits from game object)
class Ball : public GameObject{
 public:
Ball() {
    try {
//loadind sounds in constructor so that it loads with objects formed 
        InitAudioDevice();
        hit = LoadSound("Sounds/hit.mp3");
        win = LoadSound("Sounds/win.mp3");
        score = LoadSound("Sounds/score.mp3");
//throw exception if sound files are not loaded
        if (hit.stream.buffer == nullptr || win.stream.buffer == nullptr || score.stream.buffer == nullptr) {
            throw std::runtime_error("Failed to load sound files");
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
//close window if exception is thrown
        CloseAudioDevice();
        CloseWindow();
        return;
    }
}
 ~Ball(){
    try {
//throws exception if sound files are not unloaded
        if (hit.stream.buffer == nullptr || win.stream.buffer == nullptr || score.stream.buffer == nullptr) {
            throw std::runtime_error("Failed to unload sound files");
        }
//unloads the sound to clears ram , in destructor so that it unloads when object is destroyed
        UnloadSound(hit);
        UnloadSound(win);
        UnloadSound(score);
        CloseAudioDevice();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        CloseAudioDevice();
        return;
    }
 }
 //variables for ball
    float x, y;
    int speed_x, speed_y;
    int radius;

//draw and update functions
     void Draw() const override {
        DrawCircle(x, y, radius, YELLOW);
    }
    void Update()override{
        x += speed_x;
        y += speed_y; 
//checks for collision with walls and changes direction
        if (y + radius >= screen_height || y - radius <= 0) {
            speed_y *= -1;
        }
//checks for collision with player and cpu paddle and changes direction
        if (x + radius >= screen_width) {
            PlaySound(score);
            cpu_score++;
            ResetBall();
        }
//checks for collision with player and cpu paddle and changes direction
        if (x - radius <= 0) {
            PlaySound(score);
            player_score++;
            ResetBall();
        }
    }
//resets ball position and speed
    void ResetBall() {
        x = screen_width/ 2;
        y = screen_height / 2;
        int speed_choices[2] = {-1, 1};
//randomises speed of ball after reset
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};
class Paddle : public GameObject{
 protected:
 //limits movement of paddle so that the paddle does not go out of screen
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= screen_height) {
            y = screen_height - height;
        }
    }
 public:
//variables for paddle
 Paddle() : speed(6) {}
    float x, y;
    float width, height;
    int speed;
//draw and update functions
    void Draw() const override {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }
    void Update() override {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        LimitMovement();
    }
//checks for winner and displays winner
    void CheckWinner() {
    if (player_score >= 10) {
        DrawText("Player Wins!", screen_width / 2 - MeasureText("Player Wins!", 80) / 2, screen_height / 2 - 40, 80, YELLOW);
        EndDrawing();
        WaitTime(5); // Wait for 5 seconds
        CloseWindow();
    }
}
};
//cpu paddle class (inherits from paddle)
class CpuPaddle : public Paddle{
 public:
    void UpdateBall(int ball_y) {
        if (y + height / 2 > ball_y) {
            y = y - speed;
        }
        if (y + height / 2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
//checks for winner and displays winner
    void CheckWinner() {
    if (cpu_score >= 10) {
        PlaySound(win);
        DrawText("CPU Wins!", screen_width / 2 - MeasureText("CPU Wins!", 80) / 2, screen_height / 2 - 40, 80, YELLOW);
        EndDrawing();
        WaitTime(5); // Wait for 5 seconds
        CloseWindow();
    }
}
};
//game function
int game() {
//vector of game objects
    std::vector<GameObject*> gameObjects;
    Ball* ball = new Ball();//creating ball object
    ball->radius = 15;
    ball->x = screen_width / 2;
    ball->y = screen_height / 2;
    ball->speed_x = 7;
    ball->speed_y = 7;
    gameObjects.push_back(ball);//adding ball object to vector

    Paddle* player = new Paddle();//creating player object
    player->width = 10;
    player->height = 100;
    player->x = screen_width - player->width - 10;
    player->y = screen_height / 2 - player->height / 2;
    gameObjects.push_back(player);//adding player object to vector

    CpuPaddle* cpu = new CpuPaddle();//creating cpu object
    cpu->height = 100;
    cpu->width = 10;
    cpu->x = 10;
    cpu->y = screen_height / 2 - cpu->height / 2;
    cpu->speed = 5;
    gameObjects.push_back(cpu);//adding cpu object to vector
    InitWindow(screen_width, screen_height, "Ping Pong!");//initialising window
    SetTargetFPS(60);
 //game loop
    while (WindowShouldClose() == false) {        
    BeginDrawing();       
    for (GameObject* obj : gameObjects) {//updating and drawing objects
    CpuPaddle* cpuPaddle = dynamic_cast<CpuPaddle*>(obj);//dynamic cast to check if object is cpu paddle
    if (cpuPaddle != nullptr) {//if object is cpu paddle then update ball
        cpuPaddle->UpdateBall(ball->y);//update ball
    } else {
        obj->Update();//else update object
    }
    obj->Draw();//draw object
} 
      
        if (CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {player->x, player->y, player->width, player->height})) {
            // Check for collision with player paddle
            PlaySound(hit);
            ball->speed_x *= -1;
        }
        if (CheckCollisionCircleRec({ball->x, ball->y}, ball->radius, {cpu->x, cpu->y, cpu->width, cpu->height})) {
            // Check for collision with cpu paddle
            PlaySound(hit);
            ball->speed_x *= -1;
        }
    // Draw the game whole frontend incl background textures , 
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
        DrawRectangle(screen_width / 2 - 400, screen_height / 2 - 200, 800, 400, Light_Green);
        DrawCircle(screen_width / 2, screen_height / 2, 150, Green);
        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
        ball->Draw();
        cpu->Draw();
        player->Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);
        // Check for winner
        player->CheckWinner();
        cpu->CheckWinner();
        EndDrawing();
    }
    // Delete all game objects
    for (GameObject* obj : gameObjects) {
        delete obj;
    }
    // Clear the vector
    gameObjects.clear();
    // Close the window
    CloseWindow();
    return 0;
}

int main(){
    MainMenu();
}
