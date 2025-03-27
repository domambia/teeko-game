# Teeko Game Implementation Specification

## 1. Introduction

This document provides a detailed technical specification for implementing the game **Teeko** in **C**. The game consists of two phases:

- **Placement Phase** (Players place their marks on a 4x4 board)
- **Movement Phase** (Players move their marks to achieve a winning condition)

The implementation must follow strict constraints regarding input handling, function definitions, and game logic.

---

## 2. Game Rules

### 2.1 Board Setup

- The game board is a **4x4 grid** initialized as empty.
- Players take turns placing or moving their **four marks**.
- The first player (Player 1) uses **Circle (O)**, and the second player (Player 2) uses **Cross (X)**.
- If playing against a **computer opponent**, it plays as **Player 2 (X)**.

### 2.2 Winning Conditions

A player wins if they form any of the following patterns:

1. **A row of 4** (horizontally, vertically, or diagonally)
2. **A 2x2 square block** of their marks

### 2.3 Game Phases

#### **Phase 1: Placement Phase**

- Players take turns **placing** their marks on **unoccupied spaces**.
- If a winning condition is met, the game ends immediately.
- If all **8 marks** are placed and no winner is determined, the game transitions to **Phase 2 (Movement Phase)**.

#### **Phase 2: Movement Phase**

- Players take turns **moving** one of their marks into an **empty adjacent space**.
- Movement is allowed in **eight directions**:
  1. **Up-Left (1)**
  2. **Up (2)**
  3. **Up-Right (3)**
  4. **Left (4)**
  5. **Right (5)**
  6. **Down-Left (6)**
  7. **Down (7)**
  8. **Down-Right (8)**
- The game ends as soon as a player achieves a winning condition.

---

## 3. Function Specifications

### 3.1 `void initGameBoard(int board[4][4])`

- Initializes the game board with **all empty spaces** (`EMPTY = 0`).
- Should be called at the beginning of the game.

### 3.2 `void printGameBoard(int board[4][4])`

- Displays the **4x4 board**:
  - Empty spaces are represented by **numbers 1-16**.
  - Circle (`O`) and Cross (`X`) marks represent player moves.
- Example:

```
================
|13||14||15||16|
| 9|| O|| O||12|
| X|| X|| X|| O|
| X|| 2|| 3|| 4|
================
```

### 3.3 `void placeMarkByHumanPlayer(int board[4][4], int player)`

- **Prompts** the player for a position (`1-16`).
- Updates the board **only if the chosen position is empty**.
- If invalid input is given, prompt again.

### 3.4 `int hasWinner(int board[4][4])`

- Checks if there is a **winner** (either `O` or `X`).
- Returns:
  - `1` if Player 1 (O) wins
  - `2` if Player 2 (X) wins
  - `0` if there is no winner yet

### 3.5 `void placeMarkByComputerPlayer(int board[4][4])`

- Places `X` at the **first available empty spot** (scanning from 1 to 16).

### 3.6 `void moveMarkByHumanPlayer(int board[4][4], int player)`

- Asks the player to **select a mark to move (1-16)**.
- Prompts for a **direction (1-8)**.
- Ensures the **move is valid** (within bounds and to an empty space).
- If invalid, prints an error message and prompts again.

### 3.7 `void moveMarkByComputerPlayer(int board[4][4])`

- Moves the **first available X mark** (scanning from 16 to 1).
- Chooses the **first valid direction** (scanning from 8 to 1).

---

## 4. User Input Handling

### 4.1 Game Start

- Prompt: `How many human players [1-2]?`
- If `2`: Both players are human.
- If `1`: Player 2 is controlled by the **computer**.

### 4.2 Player Actions

- **Placing Marks**:
  - `Player 1, please place your mark [1-16]:`
  - `Player 2, please place your mark [1-16]:`
  - `Computer places the mark:`
- **Moving Marks**:
  - `Player 1, please select your mark [1-16]:`
  - `Player 2, please select your mark [1-16]:`
  - `Please select moving direction [1-8] or back [0]:`
- **Invalid Input Handling**:
  - `Wrong pick, try again.` (if selecting an opponent’s mark or empty space)
  - `Out of bound, try again.` (if moving off the board)
  - `Occupied, try again.` (if moving into an occupied space)

### 4.3 End Game Messages

- `Player 1 wins! Congratulations!`
- `Player 2 wins! Congratulations!`
- `Computer wins!`
- `Draw game!`

---

## 5. File Handling

### **gameboard.txt** (Used in Phase 2)

- Contains a **4x4 grid of numbers (0, 1, 2)**:
  - `0` = EMPTY
  - `1` = CIRCLE (O)
  - `2` = CROSS (X)

---

## 6. Code Constraints

- Use **only** `<stdio.h>`.
- **No global variables** (use parameters and return values instead).
- Must follow **exact output format** (marks deducted otherwise).

---

## 7. Submission Requirements

- Submit `ProjectPart1_<studentID>.c` and `ProjectPart2_<studentID>.c`.
- Late submission penalties:
  - **Within 3 days**: 10% per day
  - **More than 3 days**: 50% penalty
  - **More than 7 days**: 0 marks (fail the course)

---

## 8. Development & Testing Environment

- **CLion IDE**
- **Windows 11 / macOS**
- **Gradescope Autograder** will check correctness.

---

## 9. Conclusion

This specification provides detailed instructions for implementing **Teeko** in C, including function breakdowns, input/output requirements, and constraints. The project must strictly follow these guidelines to ensure correctness and adherence to project grading criteria.
