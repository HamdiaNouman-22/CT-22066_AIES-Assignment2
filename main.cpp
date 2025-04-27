#include <iostream>
#include <vector>
#include <algorithm>
#include <raylib.h> // Raylib for graphics

using namespace std;

char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; // 3x3 board
char currentPlayer = 'X'; // 'X' = Minimax AI, 'O' = Alpha-Beta Pruning AI

// Screen settings
const int screenWidth = 600;
const int screenHeight = 600;
const int cellSize = 200;

// Function to draw the board
void drawBoard() {
    ClearBackground(RAYWHITE);

    // Draw grid lines
    for (int i = 1; i < 3; i++) {
        DrawLine(i * cellSize, 0, i * cellSize, screenHeight, BLACK);
        DrawLine(0, i * cellSize, screenWidth, i * cellSize, BLACK);
    }

    // Draw X and O
    for (int i = 0; i < 9; i++) {
        int x = (i % 3) * cellSize;
        int y = (i / 3) * cellSize;
        if (board[i] == 'X') {
            DrawLine(x + 30, y + 30, x + cellSize - 30, y + cellSize - 30, BLUE);
            DrawLine(x + cellSize - 30, y + 30, x + 30, y + cellSize - 30, BLUE);
        } else if (board[i] == 'O') {
            DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize / 2 - 30, RED);
        }
    }
}

// Check if a player wins
bool checkWin(char player) {
    int winConditions[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
    };
    for (auto &condition : winConditions) {
        if (board[condition[0]] == player &&
            board[condition[1]] == player &&
            board[condition[2]] == player) {
            return true;
        }
    }
    return false;
}

// Get available moves
vector<int> getAvailableMoves() {
    vector<int> moves;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') moves.push_back(i);
    }
    return moves;
}

// Check if game over
bool isGameOver() {
    return checkWin('X') || checkWin('O') || getAvailableMoves().empty();
}

// Minimax without alpha-beta pruning
int minimax(int depth, bool isMaximizing) {
    if (checkWin('O')) return 10 - depth;
    if (checkWin('X')) return depth - 10;
    if (getAvailableMoves().empty()) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int move : getAvailableMoves()) {
            board[move] = 'O';
            int score = minimax(depth + 1, false);
            board[move] = ' ';
            bestScore = max(bestScore, score);
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int move : getAvailableMoves()) {
            board[move] = 'X';
            int score = minimax(depth + 1, true);
            board[move] = ' ';
            bestScore = min(bestScore, score);
        }
        return bestScore;
    }
}

// Minimax with alpha-beta pruning
int minimaxAlphaBeta(int depth, bool isMaximizing, int alpha, int beta) {
    if (checkWin('O')) return 10 - depth;
    if (checkWin('X')) return depth - 10;
    if (getAvailableMoves().empty()) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int move : getAvailableMoves()) {
            board[move] = 'O';
            int score = minimaxAlphaBeta(depth + 1, false, alpha, beta);
            board[move] = ' ';
            bestScore = max(bestScore, score);
            alpha = max(alpha, bestScore);
            if (beta <= alpha) break;
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int move : getAvailableMoves()) {
            board[move] = 'X';
            int score = minimaxAlphaBeta(depth + 1, true, alpha, beta);
            board[move] = ' ';
            bestScore = min(bestScore, score);
            beta = min(beta, bestScore);
            if (beta <= alpha) break;
        }
        return bestScore;
    }
}

// Find best move for X (Minimax without pruning)
int getBestMoveMinimax() {
    int bestScore = 1000;
    int move = -1;
    for (int m : getAvailableMoves()) {
        board[m] = 'X';
        int score = minimax(0, true);
        board[m] = ' ';
        if (score < bestScore) {
            bestScore = score;
            move = m;
        }
    }
    return move;
}

// Find best move for O (Minimax with alpha-beta pruning)
int getBestMoveAlphaBeta() {
    int bestScore = -1000;
    int move = -1;
    for (int m : getAvailableMoves()) {
        board[m] = 'O';
        int score = minimaxAlphaBeta(0, false, -1000, 1000);
        board[m] = ' ';
        if (score > bestScore) {
            bestScore = score;
            move = m;
        }
    }
    return move;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe: Minimax vs Alpha-Beta");
    SetTargetFPS(1);

    while (!WindowShouldClose()) {
        BeginDrawing();
        drawBoard();

        if (!isGameOver()) {
            if (currentPlayer == 'X') {
                int move = getBestMoveMinimax();
                if (move != -1) board[move] = 'X';
                currentPlayer = 'O';
            } else {
                int move = getBestMoveAlphaBeta();
                if (move != -1) board[move] = 'O';
                currentPlayer = 'X';
            }
        } else {
            if (checkWin('X')) {
                DrawText("X (Minimax) Wins!", 100, screenHeight / 2 - 20, 40, BLUE);
            } else if (checkWin('O')) {
                DrawText("O (Alpha-Beta) Wins!", 100, screenHeight / 2 - 20, 40, RED);
            } else {
                DrawText("It's a Draw!", 200, screenHeight / 2 - 20, 40, DARKGRAY);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
