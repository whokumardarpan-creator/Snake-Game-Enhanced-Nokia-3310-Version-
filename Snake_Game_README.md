# Snake Game (Enhanced Nokia 3310 Version)

A console-based Snake game developed in C for the Windows console as a Programming Fundamentals project.

## Project Information
- **University:** National University of Computer & Emerging Sciences (FAST-NUCES), Karachi Campus
- **Course:** Programming Fundamentals
- **Section:** BCS-1B
- **Project title:** Snake Game (Enhanced Nokia 3310 Version)

## Group Members
- Sudheer Mantharani — 26K-0584
- Abdul Rafay Irfan — 26K-0573
- Darpan Kumar — 26K-0572

## Project Overview
An enhanced console version of the classic Nokia Snake game. The player controls a snake, collects food to earn points and grow, avoids collisions, and progresses through increasingly difficult mazes.

## Features
- Three selectable starting mazes with different difficulty levels
- Arrow-key and W/A/S/D movement controls
- Pause and quit controls
- Normal food worth 10 points
- Timed bonus food worth 30 points
- Increasing game speed as the score rises
- Maze progression at score milestones
- Collision detection for boundaries, maze walls, and the snake's body
- Console colours and sound effects
- High-score storage in `highscore.txt`
- Restart and return-to-menu options

## Requirements
- Windows operating system
- GCC compiler with Windows support, such as MinGW-w64
- Windows console support for `conio.h` and `windows.h`

**Note:** This program uses Windows-specific libraries and functions. It will not compile unchanged on Linux or macOS.

## Files
```text
Snake-Game/
├── snake_improved.c
└── README.md
```
The game creates or updates `highscore.txt` in its working directory when a new high score is saved.

## How to Compile and Run
Open Command Prompt or a terminal configured with GCC, navigate to the folder containing `snake_improved.c`, and run:
```bash
gcc snake_improved.c -o snake.exe
```
Then start the game:
```bash
snake.exe
```

## Controls
| Key | Action |
|---|---|
| Arrow keys | Move the snake |
| W / A / S / D | Move the snake |
| P | Pause or resume |
| Q | Quit the current round |
| R | Restart after game over |
| M | Return to the main menu |
| 1 / 2 / 3 | Choose a menu option or starting maze when prompted |

## Scoring and Levels
- Normal food (`*`): 10 points and the snake grows.
- Bonus food (`+`): 30 points and disappears after a limited time.
- Bonus food is introduced after every five normal foods when no bonus food is active.
- The snake moves faster as the score increases.
- The maze becomes more challenging at score milestones, up to Level 3.

## C Programming Concepts Used
- Functions and function parameters
- Return values
- Structures
- Arrays, including a two-dimensional maze array
- Pointers
- Loops and conditional statements
- Random number generation
- File handling for high-score storage

**Recursion is not used in the current implementation.**

## Academic Acknowledgment
This project was developed by the group members listed above for the Programming Fundamentals course. Any external resources or AI assistance used during development should be acknowledged truthfully in accordance with the instructor's academic-integrity and AI-use requirements.
