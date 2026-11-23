
# 🐍 Enhanced Nokia 3310 Snake Game

## 👥 Group Members

**National University of Computer & Emerging Sciences (FAST-NUCES), Karachi Campus**

**Programming Fundamentals – Section BCS-1B**

1. 26K-0584 – Sudheer Mantharani
2. 26K-0573 – Abdul Rafay Irfan
3. 26K-0572 – Darpan Kumar

---

## 📌 Project Description

A feature-rich Nokia-inspired Snake Game developed in C, featuring dynamic mazes, bonus rewards, adaptive speed, and high-score tracking.

## 🎮 Features

- Three difficulty levels with different maze designs.
- Keyboard-controlled snake movement.
- Normal food and timed bonus food.
- Dynamic scoring and snake growth.
- Increasing game speed based on score.
- Collision detection.
- Pause, resume, restart, and quit options.
- Automatic high-score saving.

## 🕹️ Controls

| Key | Action |
|---|---|
| Arrow Keys / W A S D | Move the snake |
| P | Pause / Resume |
| Q | Quit the game |
| R | Restart |
| M | Return to Main Menu |

## 🛠️ Technologies Used

- Programming Language: C
- Platform: Windows Console
- Libraries: `stdio.h`, `stdlib.h`, `conio.h`, `windows.h`, `time.h`, `string.h`

## Concepts Used

- Functions and function parameters
- Return values
- Structures
- Arrays, including a two-dimensional maze array
- Pointers
- Loops and conditional statements
- Random number generation
- File handling for high-score storage
Recursion is not used in the current implementation.

## ⚙️ Compilation and Execution

Compile using GCC:

```bash
gcc snake_game.c -o snake.exe
```

Run the game:

```bash
snake.exe
```

**Note:** This project is designed for Windows Console.

---

## 🧪 Test Cases

The following test cases were performed to verify the functionality of the Enhanced Nokia 3310 Snake Game.

### Test Case 1: Start the Game

**Input:**
```text
Enter your name: Darpan
Choose an option: 1
Choose starting maze: 1
```

**Expected Output:**
```text
====================================
       ENHANCED NOKIA SNAKE GAME
====================================

Welcome, Darpan!
High Score: 0 (by None)

    1. Play
    2. Instructions
    3. Exit

Choose an option: 1

========== SELECT STARTING MAZE ==========

1. Maze 1 - Open field (easy)
2. Maze 2 - Walls and bars (medium)
3. Maze 3 - Tough maze (hard)

Choose 1, 2 or 3: 1

Game Started!
```

**Result:** PASS

---

### Test Case 2: Snake Movement

**Input:**
```text
W / A / S / D
Arrow Keys
```

**Expected Output:**
```text
Snake moves in the selected direction.
The snake's head and body positions update.
```

**Result:** PASS

---

### Test Case 3: Eat Normal Food

**Input:**
```text
Move the snake towards normal food (*).
```

**Expected Output:**
```text
Score: 10
Length: 4

Normal food collected!
Snake length increases by 1.
```

**Result:** PASS

---

### Test Case 4: Eat Bonus Food

**Input:**
```text
Eat 5 normal foods.
Move the snake towards bonus food (+).
```

**Expected Output:**
```text
Bonus food appears!

Bonus food collected!
Score increases by 30 points.
Bonus food disappears after its timer expires.
```

**Result:** PASS

---

### Test Case 5: Pause and Resume Game

**Input:**
```text
P
P
```

**Expected Output:**
```text
*** PAUSED - press P to resume ***

Game Resumed!
Snake movement continues.
```

**Result:** PASS

---

### Test Case 6: Wall Collision

**Input:**
```text
Move the snake towards the outer boundary.
```

**Expected Output:**
```text
GAME OVER!

Final Score: 0
R = Restart     M = Main menu
```

**Result:** PASS

---

### Test Case 7: Maze Collision

**Input:**
```text
Select Maze 2 or Maze 3.
Move the snake towards an obstacle (#).
```

**Expected Output:**
```text
GAME OVER!

The snake collided with a maze obstacle.
```

**Result:** PASS

---

### Test Case 8: Self Collision

**Input:**
```text
Move the snake into its own body.
```

**Expected Output:**
```text
GAME OVER!

The snake collided with itself.
```

**Result:** PASS

---

### Test Case 9: Level Progression

**Input:**
```text
Collect normal and bonus food until the score reaches 50.
```

**Expected Output:**
```text
LEVEL UP! New maze...

Maze 2 of 3
```

**Result:** PASS

---

### Test Case 10: Increase Snake Speed

**Input:**
```text
Continue collecting food and increase the score.
```

**Expected Output:**
```text
Score increases.
Snake movement becomes faster.
Minimum movement delay: 60 ms.
```

**Result:** PASS

---

### Test Case 11: Save High Score

**Input:**
```text
Achieve a score higher than the previous high score.
```

**Expected Output:**
```text
GAME OVER!

Final Score: 100
*** NEW HIGH SCORE! ***
```

**File Output (highscore.txt):**
```text
Darpan 100
```

**Result:** PASS

---

### Test Case 12: Restart Game

**Input:**
```text
R
```

**Expected Output:**
```text
Game restarts.
Score resets to 0.
Snake returns to its initial position.
Selected starting maze is loaded again.
```

**Result:** PASS

---

### Test Case 13: Return to Main Menu

**Input:**
```text
M
```

**Expected Output:**
```text
====================================
       ENHANCED NOKIA SNAKE GAME
====================================

Welcome, Darpan!
High Score: 100 (by Darpan)

1. Play
2. Instructions
3. Exit
```

**Result:** PASS

---

### Test Case 14: Quit the Game

**Input:**
```text
Q
```

**Expected Output:**
```text
GAME OVER!

R = Restart     M = Main menu
```

After returning to the main menu and selecting Exit:

```text
Thank you for playing Snake Game!

Developed by Darpan, Sudheer and Rafay
(BCS-1B), FAST NUCES Karachi
```

**Result:** PASS

---

## 📊 Test Summary

| Description | Total |
|---|---:|
| Total Test Cases | 14 |
| Passed | 14 |
| Failed | 0 |

**Conclusion:**

All 14 test cases are designed to verify the major functionalities of the Enhanced Nokia 3310 Snake Game, including snake movement, food collection, bonus scoring, collision detection, level progression, speed adjustment, pause/resume, high-score saving, and game navigation.

*Note: These are expected manual test scenarios. Actual results should be confirmed by running the game. Random food positions and scores may differ between runs.*


**Developed as a Programming Fundamentals Group Project.**
