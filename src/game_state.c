#include "teeko.h"

void initGameState(GameState *state, int gameMode, int difficulty)
{
    // Initialize board to empty
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            state->board[i][j] = EMPTY;
        }
    }

    // Set initial game state
    state->currentPhase = PLACEMENT_PHASE;
    state->currentPlayer = PLAYER_O;
    state->gameMode = gameMode;
    state->isGameActive = 1;
    state->difficulty = difficulty;

    // Initialize marks placed counter
    state->marksPlaced[0] = 0; // Player O
    state->marksPlaced[1] = 0; // Player X

    // Initialize move history
    state->moveCount = 0;

    // Initialize timer
    initGameTimer(&state->timer);
}

void addMoveToHistory(GameState *state, Move move)
{
    if (state->moveCount < MAX_MOVES)
    {
        state->moveHistory[state->moveCount].move = move;
        state->moveHistory[state->moveCount].phase = state->currentPhase;
        state->moveHistory[state->moveCount].player = state->currentPlayer;
        state->moveHistory[state->moveCount].marksPlaced[0] = state->marksPlaced[0];
        state->moveHistory[state->moveCount].marksPlaced[1] = state->marksPlaced[1];
        state->moveCount++;
    }
}

int undoLastMove(GameState *state)
{
    if (state->moveCount == 0)
    {
        return 0; // No moves to undo
    }

    state->moveCount--;
    MoveHistory *lastMove = &state->moveHistory[state->moveCount];

    // Restore game state
    state->currentPhase = lastMove->phase;
    state->currentPlayer = lastMove->player;
    state->marksPlaced[0] = lastMove->marksPlaced[0];
    state->marksPlaced[1] = lastMove->marksPlaced[1];

    // Restore board state
    if (state->currentPhase == PLACEMENT_PHASE)
    {
        int row = getRow(lastMove->move.position);
        int col = getCol(lastMove->move.position);
        state->board[row][col] = EMPTY;
    }
    else // MOVEMENT_PHASE
    {
        // Restore source position
        int srcRow = getRow(lastMove->move.position);
        int srcCol = getCol(lastMove->move.position);
        state->board[srcRow][srcCol] = lastMove->player;

        // Clear destination position
        int destRow = srcRow;
        int destCol = srcCol;

        switch (lastMove->move.direction)
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

        state->board[destRow][destCol] = EMPTY;
    }

    return 1; // Move successfully undone
}

void updateGameState(GameState *state, Move move)
{
    // Add move to history before updating state
    addMoveToHistory(state, move);

    int row = getRow(move.position);
    int col = getCol(move.position);

    if (state->currentPhase == PLACEMENT_PHASE)
    {
        // Check if player has marks left to place
        int playerIndex = (move.player == PLAYER_O) ? 0 : 1;
        if (state->marksPlaced[playerIndex] >= MAX_MARKS_PER_PLAYER)
        {
            printf("Player %d has no more marks to place!\n", move.player);
            return;
        }

        // Place the mark
        state->board[row][col] = move.player;
        state->marksPlaced[playerIndex]++;
    }
    else // MOVEMENT_PHASE
    {
        // Get source position
        int srcRow = getRow(move.position);
        int srcCol = getCol(move.position);

        // Validate source position
        if (state->board[srcRow][srcCol] != move.player)
        {
            printf("Wrong pick, try again.\n");
            return;
        }

        // Calculate destination based on direction
        int destRow = srcRow;
        int destCol = srcCol;

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

        // Validate destination
        if (destRow < 0 || destRow >= BOARD_SIZE || destCol < 0 || destCol >= BOARD_SIZE)
        {
            printf("Out of bound, try again.\n");
            return;
        }

        if (state->board[destRow][destCol] != EMPTY)
        {
            printf("Occupied, try again.\n");
            return;
        }

        // Move the mark
        state->board[srcRow][srcCol] = EMPTY;
        state->board[destRow][destCol] = move.player;
    }

    // Switch player
    state->currentPlayer = (state->currentPlayer == PLAYER_O) ? PLAYER_X : PLAYER_O;

    // Update timer
    state->timer.moveStartTime = time(NULL);
}

int isGameOver(GameState *state)
{
    return !state->isGameActive;
}

void transitionPhase(GameState *state)
{
    if (state->currentPhase == PLACEMENT_PHASE)
    {
        // Check if all pieces are placed (8 pieces total)
        int piecesPlaced = 0;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (state->board[i][j] != EMPTY)
                {
                    piecesPlaced++;
                }
            }
        }

        if (piecesPlaced == 8)
        {
            state->currentPhase = MOVEMENT_PHASE;
            printf("\nAll pieces placed! Moving to movement phase.\n");
        }
    }
}

// Add new function to check if player has valid moves
int hasValidMoves(GameState *state, int player)
{
    // Check each position for player's marks
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (state->board[i][j] == player)
            {
                // Check all 8 directions
                for (int dir = 1; dir <= 8; dir++)
                {
                    int destRow = i;
                    int destCol = j;

                    switch (dir)
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

                    // Check if move is valid
                    if (destRow >= 0 && destRow < BOARD_SIZE &&
                        destCol >= 0 && destCol < BOARD_SIZE &&
                        state->board[destRow][destCol] == EMPTY)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}