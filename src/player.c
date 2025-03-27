#include "teeko.h"
#include <string.h>

static int evaluateMove(GameState *state, Move move)
{
    int score = 0;
    int row = getRow(move.position);
    int col = getCol(move.position);

    // If it's a winning move, give highest score
    GameState tempState = *state;
    updateGameState(&tempState, move);
    if (checkWinCondition(&tempState) == move.player)
    {
        return 1000;
    }

    // If it blocks opponent's winning move, give high score
    tempState = *state;
    tempState.currentPlayer = (move.player == PLAYER_O) ? PLAYER_X : PLAYER_O;
    updateGameState(&tempState, move);
    if (checkWinCondition(&tempState) == tempState.currentPlayer)
    {
        return 900;
    }

    // Prefer center positions
    if ((row == 1 || row == 2) && (col == 1 || col == 2))
    {
        score += 10;
    }

    // Prefer positions that create potential winning opportunities
    for (int dir = 1; dir <= 8; dir++)
    {
        int count = 1;
        int r = row;
        int c = col;

        // Count consecutive marks in each direction
        for (int i = 0; i < 3; i++)
        {
            switch (dir)
            {
            case 1:
                r--;
                c--;
                break;
            case 2:
                r--;
                break;
            case 3:
                r--;
                c++;
                break;
            case 4:
                c--;
                break;
            case 5:
                c++;
                break;
            case 6:
                r++;
                c--;
                break;
            case 7:
                r++;
                break;
            case 8:
                r++;
                c++;
                break;
            }

            if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE &&
                state->board[r][c] == move.player)
            {
                count++;
            }
            else
            {
                break;
            }
        }

        score += count * 5;
    }

    return score;
}

Move getHumanMove(GameState *state)
{
    Move move;
    int valid = 0;
    char input[10]; // Buffer for input

    while (!valid)
    {
        if (state->currentPhase == PLACEMENT_PHASE)
        {
            printf("Player %d, please place your mark [1-16]: ", state->currentPlayer);
            if (fgets(input, sizeof(input), stdin) != NULL)
            {
                // Clear input buffer if needed
                if (strchr(input, '\n') == NULL)
                {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;
                }

                if (sscanf(input, "%d", &move.position) == 1)
                {
                    move.player = state->currentPlayer;
                    valid = isValidMove(state, move);

                    if (!valid)
                    {
                        printf("Invalid move, try again.\n");
                    }
                }
                else
                {
                    printf("Please enter a valid number.\n");
                }
            }
        }
        else // MOVEMENT_PHASE
        {
            // Check if player has valid moves
            if (!hasValidMoves(state, state->currentPlayer))
            {
                printf("Player %d has no valid moves available!\n", state->currentPlayer);
                move.position = 0; // Signal no valid moves
                move.player = state->currentPlayer;
                return move;
            }

            printf("Player %d, please select your mark [1-16] or back [0]: ", state->currentPlayer);
            if (fgets(input, sizeof(input), stdin) != NULL)
            {
                // Clear input buffer if needed
                if (strchr(input, '\n') == NULL)
                {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF)
                        ;
                }

                if (sscanf(input, "%d", &move.position) == 1)
                {
                    if (move.position == 0)
                    {
                        // Handle back option
                        valid = 1;
                        move.player = state->currentPlayer;
                        return move;
                    }

                    move.player = state->currentPlayer;

                    // Validate mark selection
                    int row = getRow(move.position);
                    int col = getCol(move.position);

                    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
                    {
                        printf("Invalid position, try again.\n");
                        continue;
                    }

                    if (state->board[row][col] != move.player)
                    {
                        printf("Wrong pick, try again.\n");
                        continue;
                    }

                    // Get direction
                    printf("Please select moving direction [1-8]: ");
                    if (fgets(input, sizeof(input), stdin) != NULL)
                    {
                        // Clear input buffer if needed
                        if (strchr(input, '\n') == NULL)
                        {
                            int c;
                            while ((c = getchar()) != '\n' && c != EOF)
                                ;
                        }

                        if (sscanf(input, "%d", &move.direction) == 1)
                        {
                            if (move.direction < 1 || move.direction > 8)
                            {
                                printf("Invalid direction, try again.\n");
                                continue;
                            }
                            valid = 1;
                        }
                        else
                        {
                            printf("Please enter a valid number.\n");
                        }
                    }
                }
                else
                {
                    printf("Please enter a valid number.\n");
                }
            }
        }
    }

    return move;
}

Move getComputerMove(GameState *state)
{
    Move move;
    int valid = 0;
    int bestScore = -1;
    Move bestMove = {0, 0, state->currentPlayer};

    if (state->currentPhase == PLACEMENT_PHASE)
    {
        // Try all possible positions
        for (int pos = 1; pos <= BOARD_SIZE * BOARD_SIZE; pos++)
        {
            move.position = pos;
            move.player = state->currentPlayer;

            if (isValidMove(state, move))
            {
                int score = evaluateMove(state, move);
                if (score > bestScore)
                {
                    bestScore = score;
                    bestMove = move;
                    valid = 1;
                }
            }
        }
    }
    else // MOVEMENT_PHASE
    {
        // Check if computer has valid moves
        if (!hasValidMoves(state, state->currentPlayer))
        {
            move.position = 0; // Signal no valid moves
            move.player = state->currentPlayer;
            return move;
        }

        // Try all possible moves
        for (int pos = 1; pos <= BOARD_SIZE * BOARD_SIZE; pos++)
        {
            int row = getRow(pos);
            int col = getCol(pos);

            if (state->board[row][col] == state->currentPlayer)
            {
                // Try each direction
                for (int dir = 1; dir <= 8; dir++)
                {
                    move.position = pos;
                    move.direction = dir;
                    move.player = state->currentPlayer;

                    if (isValidMove(state, move))
                    {
                        int score = evaluateMove(state, move);
                        if (score > bestScore)
                        {
                            bestScore = score;
                            bestMove = move;
                            valid = 1;
                        }
                    }
                }
            }
        }
    }

    return valid ? bestMove : (Move){0, 0, state->currentPlayer};
}

void executeMove(GameState *state, Move move)
{
    // Handle no valid moves
    if (move.position == 0)
    {
        printf("Player %d has no valid moves available!\n", move.player);
        state->isGameActive = 0;
        printf("Game Over!\n");
        return;
    }

    // Handle back option in movement phase
    if (state->currentPhase == MOVEMENT_PHASE && move.position == 0)
    {
        return;
    }

    updateGameState(state, move);

    // Check for winner
    int winner = checkWinCondition(state);
    if (winner)
    {
        state->isGameActive = 0;
        if (winner == PLAYER_O)
        {
            printf("\nPlayer 1 wins! Congratulations!\n");
        }
        else if (winner == PLAYER_X)
        {
            if (state->gameMode == 2)
            {
                printf("\nComputer wins!\n");
            }
            else
            {
                printf("\nPlayer 2 wins! Congratulations!\n");
            }
        }
        return;
    }

    // Check for phase transition
    transitionPhase(state);

    // Check for draw (all pieces placed and no winner)
    if (state->currentPhase == MOVEMENT_PHASE && !state->isGameActive)
    {
        printf("\nDraw game!\n");
    }
}