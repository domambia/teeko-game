#include "teeko.h"
#include <string.h>

void printMenu()
{
    printf("\n=== Teeko Game ===\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("3. View Statistics\n");
    printf("4. Quit\n");
    printf("Enter your choice: ");
}

void printDifficultyMenu()
{
    printf("\nSelect AI Difficulty:\n");
    printf("1. Easy (Random moves)\n");
    printf("2. Medium (Smart moves)\n");
    printf("3. Hard (Look-ahead AI)\n");
    printf("Enter your choice: ");
}

int main()
{
    int choice;
    char input[10];
    GameStats stats;
    loadGameStats(&stats, "teeko_stats.txt");

    while (1)
    {
        printMenu();

        if (fgets(input, sizeof(input), stdin) != NULL)
        {
            // Clear input buffer if needed
            if (strchr(input, '\n') == NULL)
            {
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
            }

            if (sscanf(input, "%d", &choice) == 1)
            {
                if (choice == 1)
                {
                    GameState state;
                    int gameMode;
                    int difficulty = DIFFICULTY_MEDIUM; // Default difficulty

                    // Get game mode
                    do
                    {
                        printf("How many human players [1-2]? ");
                        if (fgets(input, sizeof(input), stdin) != NULL)
                        {
                            if (strchr(input, '\n') == NULL)
                            {
                                int c;
                                while ((c = getchar()) != '\n' && c != EOF)
                                    ;
                            }

                            if (sscanf(input, "%d", &gameMode) == 1)
                            {
                                if (gameMode >= 1 && gameMode <= 2)
                                {
                                    break;
                                }
                            }
                        }
                        printf("Invalid input. Please enter 1 or 2.\n");
                    } while (1);

                    // Get difficulty if playing against computer
                    if (gameMode == 1)
                    {
                        printDifficultyMenu();
                        if (fgets(input, sizeof(input), stdin) != NULL)
                        {
                            if (strchr(input, '\n') == NULL)
                            {
                                int c;
                                while ((c = getchar()) != '\n' && c != EOF)
                                    ;
                            }

                            if (sscanf(input, "%d", &difficulty) == 1)
                            {
                                if (difficulty < 1 || difficulty > 3)
                                {
                                    difficulty = DIFFICULTY_MEDIUM;
                                }
                            }
                        }
                    }

                    // Initialize game
                    initGameState(&state, gameMode, difficulty);

                    // Main game loop
                    while (!isGameOver(&state))
                    {
                        displayBoard(&state);
                        displayTimeRemaining(&state.timer);

                        // Check for time up
                        updateGameTimer(&state.timer);
                        if (isTimeUp(&state.timer))
                        {
                            printf("\nTime's up! Game Over!\n");
                            state.isGameActive = 0;
                            break;
                        }

                        Move move;
                        if (state.currentPlayer == PLAYER_O || gameMode == 2)
                        {
                            printf("Enter 'u' to undo last move or press Enter to continue: ");
                            if (fgets(input, sizeof(input), stdin) != NULL)
                            {
                                if (input[0] == 'u' || input[0] == 'U')
                                {
                                    if (undoLastMove(&state))
                                    {
                                        printf("Move undone.\n");
                                        continue;
                                    }
                                    else
                                    {
                                        printf("No moves to undo.\n");
                                    }
                                }
                            }
                            move = getHumanMove(&state);
                        }
                        else
                        {
                            move = getComputerMove(&state);
                            printf("Computer places the mark at position %d\n", move.position);
                        }

                        executeMove(&state, move);
                    }

                    // Update statistics
                    int winner = checkWinCondition(&state);
                    updateGameStats(&stats, winner);
                    saveGameStats(&stats, "teeko_stats.txt");

                    // Save final game state
                    saveGameState(&state, "gameboard.txt");
                }
                else if (choice == 2)
                {
                    GameState state;
                    loadGameState(&state, "gameboard.txt");

                    if (state.isGameActive)
                    {
                        // Main game loop
                        while (!isGameOver(&state))
                        {
                            displayBoard(&state);
                            displayTimeRemaining(&state.timer);

                            // Check for time up
                            updateGameTimer(&state.timer);
                            if (isTimeUp(&state.timer))
                            {
                                printf("\nTime's up! Game Over!\n");
                                state.isGameActive = 0;
                                break;
                            }

                            Move move;
                            if (state.currentPlayer == PLAYER_O || state.gameMode == 2)
                            {
                                printf("Enter 'u' to undo last move or press Enter to continue: ");
                                if (fgets(input, sizeof(input), stdin) != NULL)
                                {
                                    if (input[0] == 'u' || input[0] == 'U')
                                    {
                                        if (undoLastMove(&state))
                                        {
                                            printf("Move undone.\n");
                                            continue;
                                        }
                                        else
                                        {
                                            printf("No moves to undo.\n");
                                        }
                                    }
                                }
                                move = getHumanMove(&state);
                            }
                            else
                            {
                                move = getComputerMove(&state);
                                printf("Computer places the mark at position %d\n", move.position);
                            }

                            executeMove(&state, move);
                        }

                        // Update statistics
                        int winner = checkWinCondition(&state);
                        updateGameStats(&stats, winner);
                        saveGameStats(&stats, "teeko_stats.txt");
                    }
                    else
                    {
                        printf("No saved game found or game is already finished.\n");
                    }
                }
                else if (choice == 3)
                {
                    displayGameStats(&stats);
                }
                else if (choice == 4)
                {
                    printf("Thanks for playing!\n");
                    break;
                }
                else
                {
                    printf("Invalid choice. Please try again.\n");
                }
            }
            else
            {
                printf("Please enter a valid number.\n");
            }
        }
    }

    return 0;
}