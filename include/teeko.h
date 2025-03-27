#ifndef TEEKO_H
#define TEEKO_H

#include <stdio.h>
#include <time.h>

// Constants
#define BOARD_SIZE 4
#define EMPTY 0
#define PLAYER_O 1
#define PLAYER_X 2
#define PLACEMENT_PHASE 1
#define MOVEMENT_PHASE 2
#define MAX_MARKS_PER_PLAYER 4
#define MAX_MOVES 100
#define MOVE_TIME_LIMIT 30  // seconds per move
#define GAME_TIME_LIMIT 600 // 10 minutes per game

// Difficulty levels
#define DIFFICULTY_EASY 1
#define DIFFICULTY_MEDIUM 2
#define DIFFICULTY_HARD 3

// Move representation
typedef struct
{
    int position;  // 1-16 for board position
    int direction; // 1-8 for movement direction
    int player;    // 1=O, 2=X
} Move;

// Move history entry
typedef struct
{
    Move move;
    int phase;
    int player;
    int marksPlaced[2];
} MoveHistory;

// Game statistics
typedef struct
{
    int wins[2];   // [Player 1, Player 2]
    int losses[2]; // [Player 1, Player 2]
    int draws;
    int totalGames;
} GameStats;

// Timer structure
typedef struct
{
    time_t moveStartTime;
    time_t gameStartTime;
    int moveTimeRemaining;
    int gameTimeRemaining;
} GameTimer;

// Game state
typedef struct
{
    int board[BOARD_SIZE][BOARD_SIZE];  // 0=empty, 1=O, 2=X
    int currentPhase;                   // 1=placement, 2=movement
    int currentPlayer;                  // 1=O, 2=X
    int gameMode;                       // 1=human-human, 2=human-computer
    int isGameActive;                   // 1=active, 0=ended
    int marksPlaced[2];                 // Number of marks placed by each player [O, X]
    int difficulty;                     // AI difficulty level
    MoveHistory moveHistory[MAX_MOVES]; // Move history for undo
    int moveCount;                      // Number of moves made
    GameTimer timer;                    // Game timer
} GameState;

// Game State Management
void initGameState(GameState *state, int gameMode, int difficulty);
void updateGameState(GameState *state, Move move);
int isGameOver(GameState *state);
int hasValidMoves(GameState *state, int player);

// Game Logic
int isValidMove(GameState *state, Move move);
int checkWinCondition(GameState *state);
void transitionPhase(GameState *state);

// Player Interface
Move getHumanMove(GameState *state);
Move getComputerMove(GameState *state);
void executeMove(GameState *state, Move move);

// I/O Operations
void displayBoard(GameState *state);
void saveGameState(GameState *state, const char *filename);
void loadGameState(GameState *state, const char *filename);

// Statistics Management
void initGameStats(GameStats *stats);
void updateGameStats(GameStats *stats, int winner);
void saveGameStats(GameStats *stats, const char *filename);
void loadGameStats(GameStats *stats, const char *filename);
void displayGameStats(GameStats *stats);

// Move History Management
void addMoveToHistory(GameState *state, Move move);
int undoLastMove(GameState *state);

// Timer Management
void initGameTimer(GameTimer *timer);
void updateGameTimer(GameTimer *timer);
int isTimeUp(GameTimer *timer);
void displayTimeRemaining(GameTimer *timer);

// Utility Functions
int getRow(int position);
int getCol(int position);
int getPosition(int row, int col);
void clearScreen(void);

#endif // TEEKO_H