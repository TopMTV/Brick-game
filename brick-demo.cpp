#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

// Function to move the cursor to a specific position
void gotoxy(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

// Function to hide the console cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// Game dimensions
const int SCREEN_WIDTH = 40;
const int SCREEN_HEIGHT = 20;

// Paddle properties
int paddleX = SCREEN_WIDTH / 2 - 3;
const int PADDLE_WIDTH = 7;

// Ball properties
int ballX = SCREEN_WIDTH / 2;
int ballY = SCREEN_HEIGHT - 2;
int ballDirX = 1; // 1 for right, -1 for left
int ballDirY = -1; // 1 for down, -1 for up

// Bricks
std::vector<std::pair<int, int>> bricks;

// Initialize bricks
void setupBricks() {
    for (int y = 2; y < 5; ++y) {
        for (int x = 2; x < SCREEN_WIDTH - 2; x += 3) {
            bricks.push_back({x, y});
        }
    }
}

// Draw game elements
void draw() {
    system("cls"); // Clear console

    // Draw walls
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        std::cout << "#";
    }
    std::cout << std::endl;
    for (int i = 1; i < SCREEN_HEIGHT - 1; ++i) {
        std::cout << "#";
        for (int j = 1; j < SCREEN_WIDTH - 1; ++j) {
            std::cout << " ";
        }
        std::cout << "#" << std::endl;
    }
    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        std::cout << "#";
    }
    std::cout << std::endl;

    // Draw paddle
    gotoxy(paddleX, SCREEN_HEIGHT - 1);
    for (int i = 0; i < PADDLE_WIDTH; ++i) {
        std::cout << "=";
    }

    // Draw ball
    gotoxy(ballX, ballY);
    std::cout << "O";

    // Draw bricks
    for (const auto& brick : bricks) {
        gotoxy(brick.first, brick.second);
        std::cout << "[]";
    }
}

// Handle input
void input() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'a' && paddleX > 1) {
            paddleX--;
        }
        if (key == 'd' && paddleX + PADDLE_WIDTH < SCREEN_WIDTH - 1) {
            paddleX++;
        }
        if (key == 'q') { // Quit game
            exit(0);
        }
    }
}

// Update game logic
void logic() {
    // Move ball
    ballX += ballDirX;
    ballY += ballDirY;

    // Ball collision with walls
    if (ballX <= 1 || ballX >= SCREEN_WIDTH - 2) {
        ballDirX *= -1;
    }
    if (ballY <= 1) {
        ballDirY *= -1;
    }

    // Ball collision with paddle
    if (ballY == SCREEN_HEIGHT - 2 && ballX >= paddleX && ballX < paddleX + PADDLE_WIDTH) {
        ballDirY *= -1;
    }

    // Ball collision with bricks
    for (size_t i = 0; i < bricks.size(); ++i) {
        if (ballX >= bricks[i].first && ballX < bricks[i].first + 2 &&
            ballY == bricks[i].second) {
            bricks.erase(bricks.begin() + i);
            ballDirY *= -1;
            break;
        }
    }

    // Game over condition (ball falls below paddle)
    if (ballY >= SCREEN_HEIGHT - 1) {
        gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
        std::cout << "GAME OVER!";
        _getch(); // Wait for a key press
        exit(0);
    }

    // Win condition (all bricks destroyed)
    if (bricks.empty()) {
        gotoxy(SCREEN_WIDTH / 2 - 5, SCREEN_HEIGHT / 2);
        std::cout << "YOU WIN!";
        _getch(); // Wait for a key press
        exit(0);
    }
}

int main() {
    hideCursor();
    setupBricks();

    while (true) {
        draw();
        input();
        logic();
        Sleep(100); // Control game speed
    }

    return 0;
}