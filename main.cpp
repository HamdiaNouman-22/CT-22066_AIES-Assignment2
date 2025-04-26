#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <raylib.h> // Include Raylib

using namespace std;

char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; // 3x3 board
char player = 'X'; // Monte Carlo Player
char opponent = 'O'; // Minimax Player

// Board dimensions for graphical display
const int screenWidth = 600;
const int screenHeight = 600;
const int cellSize = 200;

// Function to draw the game board with Raylib
void drawBoard() {
    ClearBackground(RAYWHITE);
    
    // Draw grid
    for (int i = 1; i < 3; i++) {
        DrawLine(i * cellSize, 0, i * cellSize, screenHeight, BLACK);
        DrawLine(0, i * cellSize, screenWidth, i * cellSize, BLACK);
    }
    
    // Draw X and O
    for (int i = 0; i < 9; i++) {
        int x = (i % 3) * cellSize;
        int y = (i / 3) * cellSize;
        if (board[i] == 'X') {
            DrawLine(x + 20, y + 20, x + cellSize - 20, y + cellSize - 20, BLUE);
            DrawLine(x + cellSize - 20, y + 20, x + 20, y + cellSize - 20, BLUE);
        } else if (board[i] == 'O') {
            DrawCircle(x + cellSize / 2, y + cellSize / 2, cellSize / 2 - 20, RED);
        }
    }
}

bool checkWin(char player) {
    int winConditions[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
        {0, 4, 8}, {2, 4, 6}             // Diagonals
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

vector<int> getAvailableMoves() {
    vector<int> moves;
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') moves.push_back(i);
    }
    return moves;
}

bool isGameOver() {
    return (checkWin('X') || checkWin('O') || getAvailableMoves().empty());
}

int runRandomGame(char tempBoard[], char currentPlayer) {
    vector<int> moves = getAvailableMoves();
    srand(time(0));

    while (!moves.empty()) {
        int move = moves[rand() % moves.size()];
        tempBoard[move] = currentPlayer;
        if (checkWin(currentPlayer)) {
            return currentPlayer == 'X'; // True if 'X' wins
        }
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        moves = getAvailableMoves();
    }
    return false;
}

int runMonteCarloSimulation(int move, int simulations) {
    int wins = 0;
    for (int i = 0; i < simulations; i++) {
        char tempBoard[9];
        copy(begin(board), end(board), tempBoard);
        tempBoard[move] = 'X';
        if (runRandomGame(tempBoard, 'O')) wins++;
    }
    return wins;
}

int monteCarloMove() {
    vector<int> moves = getAvailableMoves();
    vector<int> winCounts(moves.size(), 0);

    for (size_t i = 0; i < moves.size(); i++) {
        winCounts[i] = runMonteCarloSimulation(moves[i], 100);
    }

    int bestMove = moves[0];
    int bestWins = winCounts[0];
    for (size_t i = 1; i < moves.size(); i++) {
        if (winCounts[i] > bestWins) {
            bestMove = moves[i];
            bestWins = winCounts[i];
        }
    }
    return bestMove;
}

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

int minimaxMove() {
    int bestScore = -1000;
    int bestMove = -1;

    for (int move : getAvailableMoves()) {
        board[move] = 'O';
        int score = minimax(0, false);
        board[move] = ' ';
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe: Monte Carlo vs Minimax");
    SetTargetFPS(10);

    while (!WindowShouldClose()) {
        BeginDrawing();
        drawBoard();

        if (!isGameOver()) {
            if (player == 'X') {
                int move = monteCarloMove();
                board[move] = player;
            } else {
                int move = minimaxMove();
                board[move] = opponent;
            }
            player = (player == 'X') ? 'O' : 'X';
        } else {
            // Display Game Over message
            DrawText("Game Over!", screenWidth / 4, screenHeight / 2 - 20, 40, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
