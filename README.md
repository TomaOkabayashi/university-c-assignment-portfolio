# C-Uni-Assignment

# Traffic Game

This project is a simple console-based traffic game implemented in C. The game involves navigating a player character through a maze of moving cars to reach the goal.

## Instructions

To play the game, follow these steps:

1. Have C compiler, GCC.
2. Compile the source code using the provided Makefile.

`make`

3. Run the executable named traffic.

`./traffic game_[n].txt`

4. To remove the compiled executable and object files, run:

`make clean`

## Instructions

Avoid colliding with the moving cars (<, >, ^, v) while reaching the goal (G).

## Controls

- Press w to move up.
- Press s to move down.
- Press a to move left.
- Press d to move right.
- Press u to undo the last move.

## Files

- main.c: Contains the main function responsible for running the game loop and handling user input.
- file_io.c: Implements functions for reading the game map from a file.
- game_logic.c: Implements the game logic, including player movement and car behavior.
- file_io.h: Header file for file I/O functions.
- game_logic.h: Header file for game logic functions.
- Makefile: Defines rules for compiling the source code.

## Dependencies

This project requires a C compiler such as GCC.

## Context

First C assignment given by C unit in university. Grade was 90% due to penalty for not C89 format being -10%.
It is not really a game, however it does demonstrate all the concepts learnt during the semester. Very fun language to use, though difficult.
