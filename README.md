# Teeko Game

A C implementation of the Teeko game, featuring both human-human and human-computer gameplay modes. Teeko is an engaging strategic board game that combines elements of Tic-tac-toe and Go.

## Overview

Teeko is played on a 4x4 grid where players take turns placing and moving their pieces (O and X) to form winning combinations. The game requires strategic thinking as players must balance between offensive and defensive moves.

## Features

- 🎮 Human vs Human gameplay
- 🤖 Human vs Computer gameplay with AI opponent
- 📊 Clear board display with ASCII graphics
- ✅ Input validation and error handling
- 💾 Game state saving/loading
- 🎯 Multiple winning conditions
- 🔄 Two-phase gameplay (placement and movement)

## Installation

### Prerequisites

- C compiler (gcc recommended)
- Make build system

### Building from Source

1. Clone the repository:

```bash
git clone https://github.com/domambia/teeko-game.git
cd teeko-game
```

2. Build the project:

```bash
make
```

3. Clean the build (if needed):

```bash
make clean
```

## Running the Game

After building, run the game from the build directory:

```bash
./build/teeko
```

## Game Rules

### Basic Rules

1. The game is played on a 4x4 grid
2. Players take turns placing their marks (O and X)
3. The game has two distinct phases:
   - **Placement Phase**: Players place their marks until all pieces are placed
   - **Movement Phase**: Players move their marks to adjacent empty spaces

### Winning Conditions

A player wins by achieving either of these conditions:

1. Forming a row of 4 marks (horizontally, vertically, or diagonally)
2. Forming a 2x2 square block of marks

### Example Game

Here's an example of a winning game:

```
  1 2 3 4
1 . . . .
2 . O O .
3 . O O .
4 . . . .
```

In this example, player O wins by forming a 2x2 square block.

## Game Controls

- Use numbers 1-4 for both row and column selection
- Format: `row column` (e.g., "2 3" for row 2, column 3)
- During movement phase, first select the piece to move, then select the destination
- Enter 'q' to quit the game
- Enter 's' to save the game state
- Enter 'l' to load a saved game

## Project Structure

```
teeko-game/
├── include/          # Header files
│   ├── board.h      # Board structure and functions
│   ├── game.h       # Game logic and state management
│   └── ai.h         # AI opponent implementation
├── src/             # Source files
│   ├── main.c       # Main game loop
│   ├── board.c      # Board operations
│   ├── game.c       # Game logic
│   └── ai.c         # AI implementation
├── build/           # Build output directory
├── Makefile         # Build configuration
└── README.md        # This file
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the classic Teeko game
- Built with C for performance and simplicity
