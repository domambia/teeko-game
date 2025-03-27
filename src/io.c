#include "teeko.h"
#include <stdlib.h>

void clearScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayBoard(GameState *state)
{
    printf("\n");
    printf("Current Phase: %s\n", state->currentPhase == PLACEMENT_PHASE ? "Placement" : "Movement");
    printf("Player %d's turn (%s)\n", state->currentPlayer,
           state->currentPlayer == PLAYER_O ? "O" : "X");
    printf("Marks remaining - Player 1: %d, Player 2: %d\n",
           MAX_MARKS_PER_PLAYER - state->marksPlaced[0],
           MAX_MARKS_PER_PLAYER - state->marksPlaced[1]);
    printf("\n");

    // Print column numbers
    printf("    1   2   3   4\n");

    // Print top border
    printf("  +---+---+---+---+\n");

    // Print board rows
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d |", i + 1); // Print row number

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j] == EMPTY)
            {
                printf(" %d |", getPosition(i, j));
            }
            else
            {
                printf(" %c |", state->board[i][j] == PLAYER_O ? 'O' : 'X');
            }
        }
        printf("\n");

        // Print row separator
        if (i < BOARD_SIZE - 1)
        {
            printf("  +---+---+---+---+\n");
        }
    }

    // Print bottom border
    printf("  +---+---+---+---+\n");
    printf("\n");
}

void saveGameState(GameState *state, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Could not open file for writing.\n");
        return;
    }

    // Write board state
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            fprintf(file, "%d", state->board[i][j]);
            if (j < BOARD_SIZE - 1)
            {
                fprintf(file, " ");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void loadGameState(GameState *state, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }

    // Read board state
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (fscanf(file, "%d", &state->board[i][j]) != 1)
            {
                printf("Error: Invalid file format.\n");
                fclose(file);
                return;
            }
        }
    }

    // Count marks placed
    state->marksPlaced[0] = 0; // Player O
    state->marksPlaced[1] = 0; // Player X

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j] == PLAYER_O)
            {
                state->marksPlaced[0]++;
            }
            else if (state->board[i][j] == PLAYER_X)
            {
                state->marksPlaced[1]++;
            }
        }
    }

    // Determine current phase
    if (state->marksPlaced[0] + state->marksPlaced[1] == 8)
    {
        state->currentPhase = MOVEMENT_PHASE;
    }
    else
    {
        state->currentPhase = PLACEMENT_PHASE;
    }

    fclose(file);
}

// Utility Functions
int getRow(int position)
{
    return (position - 1) / BOARD_SIZE;
}

int getCol(int position)
{
    return (position - 1) % BOARD_SIZE;
}

int getPosition(int row, int col)
{
    return row * BOARD_SIZE + col + 1;
}

// Statistics Management
void initGameStats(GameStats *stats)
{
    stats->wins[0] = 0;
    stats->wins[1] = 0;
    stats->losses[0] = 0;
    stats->losses[1] = 0;
    stats->draws = 0;
    stats->totalGames = 0;
}

void updateGameStats(GameStats *stats, int winner)
{
    stats->totalGames++;
    if (winner == 0)
    {
        stats->draws++;
    }
    else if (winner == PLAYER_O)
    {
        stats->wins[0]++;
        stats->losses[1]++;
    }
    else
    {
        stats->wins[1]++;
        stats->losses[0]++;
    }
}

void saveGameStats(GameStats *stats, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error: Could not open statistics file for writing.\n");
        return;
    }

    fprintf(file, "%d %d %d %d %d %d\n",
            stats->wins[0], stats->wins[1],
            stats->losses[0], stats->losses[1],
            stats->draws, stats->totalGames);

    fclose(file);
}

void loadGameStats(GameStats *stats, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // If file doesn't exist, initialize with zeros
        initGameStats(stats);
        return;
    }

    if (fscanf(file, "%d %d %d %d %d %d\n",
               &stats->wins[0], &stats->wins[1],
               &stats->losses[0], &stats->losses[1],
               &stats->draws, &stats->totalGames) != 6)
    {
        printf("Error: Invalid statistics file format.\n");
        initGameStats(stats);
    }

    fclose(file);
}

void displayGameStats(GameStats *stats)
{
    printf("\n=== Game Statistics ===\n");
    printf("Total Games: %d\n", stats->totalGames);
    printf("\nPlayer 1 (O):\n");
    printf("  Wins: %d\n", stats->wins[0]);
    printf("  Losses: %d\n", stats->losses[0]);
    printf("  Win Rate: %.1f%%\n",
           stats->totalGames > 0 ? (float)stats->wins[0] / stats->totalGames * 100 : 0);
    printf("\nPlayer 2 (X):\n");
    printf("  Wins: %d\n", stats->wins[1]);
    printf("  Losses: %d\n", stats->losses[1]);
    printf("  Win Rate: %.1f%%\n",
           stats->totalGames > 0 ? (float)stats->wins[1] / stats->totalGames * 100 : 0);
    printf("\nDraws: %d\n", stats->draws);
    printf("====================\n\n");
}

// Timer Management
void initGameTimer(GameTimer *timer)
{
    timer->moveStartTime = time(NULL);
    timer->gameStartTime = timer->moveStartTime;
    timer->moveTimeRemaining = MOVE_TIME_LIMIT;
    timer->gameTimeRemaining = GAME_TIME_LIMIT;
}

void updateGameTimer(GameTimer *timer)
{
    time_t currentTime = time(NULL);
    int moveElapsed = (int)difftime(currentTime, timer->moveStartTime);
    int gameElapsed = (int)difftime(currentTime, timer->gameStartTime);

    timer->moveTimeRemaining = MOVE_TIME_LIMIT - moveElapsed;
    timer->gameTimeRemaining = GAME_TIME_LIMIT - gameElapsed;

    if (timer->moveTimeRemaining < 0)
        timer->moveTimeRemaining = 0;
    if (timer->gameTimeRemaining < 0)
        timer->gameTimeRemaining = 0;
}

int isTimeUp(GameTimer *timer)
{
    return timer->moveTimeRemaining <= 0 || timer->gameTimeRemaining <= 0;
}

void displayTimeRemaining(GameTimer *timer)
{
    printf("Time Remaining - Move: %d sec, Game: %d:%02d\n",
           timer->moveTimeRemaining,
           timer->gameTimeRemaining / 60,
           timer->gameTimeRemaining % 60);
}