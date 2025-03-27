#include "teeko.h"

int isValidMove(GameState *state, Move move)
{
    int row = getRow(move.position);
    int col = getCol(move.position);

    // Check if position is within bounds
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
    {
        return 0;
    }

    if (state->currentPhase == PLACEMENT_PHASE)
    {
        // Check if position is empty
        if (state->board[row][col] != EMPTY)
        {
            return 0;
        }

        // Check if player has marks left to place
        int playerIndex = (move.player == PLAYER_O) ? 0 : 1;
        if (state->marksPlaced[playerIndex] >= MAX_MARKS_PER_PLAYER)
        {
            return 0;
        }
    }
    else // MOVEMENT_PHASE
    {
        // Check if position contains player's mark
        if (state->board[row][col] != move.player)
        {
            return 0;
        }

        // Calculate destination based on direction
        int destRow = row;
        int destCol = col;

        switch (move.direction)
        {
        case 1:
            destRow--;
            destCol--;
            break;
        case 2:
            destRow--;
            break;
        case 3:
            destRow--;
            destCol++;
            break;
        case 4:
            destCol--;
            break;
        case 5:
            destCol++;
            break;
        case 6:
            destRow++;
            destCol--;
            break;
        case 7:
            destRow++;
            break;
        case 8:
            destRow++;
            destCol++;
            break;
        }

        // Check if destination is within bounds
        if (destRow < 0 || destRow >= BOARD_SIZE || destCol < 0 || destCol >= BOARD_SIZE)
        {
            return 0;
        }

        // Check if destination is empty
        if (state->board[destRow][destCol] != EMPTY)
        {
            return 0;
        }
    }

    return 1;
}

int checkWinCondition(GameState *state)
{
    // Check rows
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j <= BOARD_SIZE - 4; j++)
        {
            if (state->board[i][j] != EMPTY &&
                state->board[i][j] == state->board[i][j + 1] &&
                state->board[i][j] == state->board[i][j + 2] &&
                state->board[i][j] == state->board[i][j + 3])
            {
                return state->board[i][j];
            }
        }
    }

    // Check columns
    for (int i = 0; i <= BOARD_SIZE - 4; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j] != EMPTY &&
                state->board[i][j] == state->board[i + 1][j] &&
                state->board[i][j] == state->board[i + 2][j] &&
                state->board[i][j] == state->board[i + 3][j])
            {
                return state->board[i][j];
            }
        }
    }

    // Check diagonals
    for (int i = 0; i <= BOARD_SIZE - 4; i++)
    {
        for (int j = 0; j <= BOARD_SIZE - 4; j++)
        {
            if (state->board[i][j] != EMPTY &&
                state->board[i][j] == state->board[i + 1][j + 1] &&
                state->board[i][j] == state->board[i + 2][j + 2] &&
                state->board[i][j] == state->board[i + 3][j + 3])
            {
                return state->board[i][j];
            }
        }
    }

    // Check 2x2 squares
    for (int i = 0; i < BOARD_SIZE - 1; i++)
    {
        for (int j = 0; j < BOARD_SIZE - 1; j++)
        {
            if (state->board[i][j] != EMPTY &&
                state->board[i][j] == state->board[i][j + 1] &&
                state->board[i][j] == state->board[i + 1][j] &&
                state->board[i][j] == state->board[i + 1][j + 1])
            {
                return state->board[i][j];
            }
        }
    }

    return 0;
}