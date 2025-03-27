# Missing Components in Teeko Game Implementation

This document outlines the key components that are currently missing or need improvement in the Teeko game implementation.

## 1. Game Phase Transition

- Missing clear implementation of transition from Placement Phase to Movement Phase
- Need to implement logic to detect when all 8 marks are placed and switch to Movement Phase

## 2. Win Condition Checking

- Need to verify and implement both required win conditions:
  - A row of 4 (horizontally, vertically, or diagonally)
  - A 2x2 square block of marks
- Verify proper implementation in `checkWinCondition` function

## 3. Movement Phase Implementation

- Missing or incomplete implementation of:
  - Selecting an existing mark (1-16)
  - Moving in one of 8 directions
  - Validating moves are within bounds and to empty spaces
  - Proper error messages for invalid moves

## 4. Error Messages

- Missing specific error messages as per specification:
  - "Wrong pick, try again" for selecting opponent's mark or empty space
  - "Out of bound, try again" for moving off the board
  - "Occupied, try again" for moving into an occupied space

## 5. End Game Messages

- Need to implement proper end game messages:
  - "Player 1 wins! Congratulations!"
  - "Player 2 wins! Congratulations!"
  - "Computer wins!"
  - "Draw game!"

## 6. Input Validation

- Need to ensure proper validation for:
  - Position selection (1-16)
  - Direction selection (1-8)
  - Back option (0) during movement phase

## 7. File Handling

- While save/load functions exist, need to verify:
  - Proper format of gameboard.txt (0, 1, 2 values)
  - Correct handling of file operations

## 8. Game State Management

- Need to ensure proper tracking of:
  - Number of marks placed by each player
  - Current phase
  - Valid moves available

## Priority Order for Implementation

1. Game Phase Transition (Critical for game flow)
2. Win Condition Checking (Critical for game rules)
3. Movement Phase Implementation (Core gameplay)
4. Error Messages and Input Validation (User experience)
5. End Game Messages (User feedback)
6. File Handling (Game state persistence)
7. Game State Management (Game logic integrity)
