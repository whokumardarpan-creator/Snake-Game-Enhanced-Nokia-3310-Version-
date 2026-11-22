/*
 * Snake Game (Enhanced Nokia 3310 Version) 
 * National University of Computer & Emerging Sciences, Karachi Campus
 * Programming Fundamentals - (BCS-1B)
 *
 * Group Members:
 *   26K-0584 Sudheer Mantharani
 *   26K-0573 Abdul Rafay Irfan
 *   26K-0572 Darpan Kumar
 *
 * Platform : Windows Console
 * Libraries: stdio.h, stdlib.h, conio.h, windows.h, time.h, string.h
 * Compile  : gcc snake_improved.c -o snake.exe
 *
 * Controls : Arrow keys / WASD = move, P = pause, Q = quit
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <string.h>

/* ----------------------------- Constants ----------------------------- */
#define WIDTH   40
#define HEIGHT  20
#define MAX_LENGTH (WIDTH * HEIGHT)

#define RIGHT 0
#define LEFT  1
#define UP    2
#define DOWN  3          /* note: dir ^ 1 gives the opposite direction */

#define MAX_LEVEL        3
#define POINTS_PER_LEVEL 50
#define POINTS_NORMAL    10
#define POINTS_BONUS     30
#define BONUS_EVERY      5      /* bonus food after every 5 normal foods */
#define BONUS_DURATION   70     /* ticks before bonus food disappears    */
#define SCORE_FILE       "highscore.txt"

/* screen layout (rows) - everything fits in a 30-line console */
#define GRID_TOP   3
#define ROW_TITLE  0
#define ROW_STATS  1
#define ROW_BONUS  2
#define ROW_HELP1  25
#define ROW_HELP2  26
#define ROW_MSG    27

/* console colours */
#define COL_DEFAULT 7
#define COL_BORDER  7
#define COL_WALL    6
#define COL_HEAD    10
#define COL_BODY    2
#define COL_FOOD    12
#define COL_BONUS   14
#define COL_TITLE   11

/* ---------------------------- Structures ----------------------------- */
typedef struct
{
    int x;
    int y;
} Segment;

typedef struct
{
    char name[30];
    char highScoreName[30];
    int score;
    int highScore;
    int level;
} Player;

/* --------------------------- Global variables ------------------------ */
Segment snake[MAX_LENGTH];
int maze[HEIGHT][WIDTH];

int length;
int direction;      /* direction the player wants to go            */
int moveDir;        /* direction the snake actually moved last tick */
int gameOver;
int gameWon;
int paused;
int startLevel = 1;  /* maze chosen by the player (1 to MAX_LEVEL) */

int foodX, foodY;
int bonusX, bonusY;
int foodActive;
int bonusActive;
int bonusTimer;
int foodsEaten;

HANDLE hOut;

/* ------------------------ Function declarations ---------------------- */
void gotoXY(int x, int y);
void setColor(int color);
void hideCursor(int hide);
void printAt(int row, int color, const char *text);
void putCell(CHAR_INFO *buf, int x, int y, char ch, int color);

Segment nextPosition(const Segment *head, int dir);
int  isSnakeAt(int x, int y);
void buildMaze(int level);
int  placeFood(void);
int  placeBonusFood(void);
int  getDelay(int score);

void setupGame(Player *p);
void drawGame(const Player *p);
void turn(int newDir);
void input(void);
void checkLevel(Player *p);
void crash(void);
void updateGame(Player *p);

void loadHighScore(Player *p);
void saveHighScore(const Player *p);
void askName(Player *p);
int  showMenu(const Player *p);
void showInstructions(void);
int  chooseMaze(void);
int  playGame(Player *p);

/* ---------------------------- Console helpers ------------------------ */
void gotoXY(int x, int y)
{
    COORD pos;
    fflush(stdout);
    pos.X = (SHORT)x;
    pos.Y = (SHORT)y;
    SetConsoleCursorPosition(hOut, pos);
}

void setColor(int color)
{
    fflush(stdout);
    SetConsoleTextAttribute(hOut, (WORD)color);
}

void hideCursor(int hide)
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 10;
    info.bVisible = hide ? FALSE : TRUE;
    SetConsoleCursorInfo(hOut, &info);
}

/* print a line of text at a fixed row (padded so old text is erased) */
void printAt(int row, int color, const char *text)
{
    gotoXY(0, row);
    setColor(color);
    printf("%-78s", text);
    fflush(stdout);
}

void putCell(CHAR_INFO *buf, int x, int y, char ch, int color)
{
    CHAR_INFO *c = &buf[y * (WIDTH + 2) + x];
    c->Char.AsciiChar = ch;
    c->Attributes = (WORD)color;
}

/* -------------------------- Snake / maze logic ----------------------- */
/* uses a pointer so the head itself is not modified */
Segment nextPosition(const Segment *head, int dir)
{
    Segment n = *head;

    if (dir == RIGHT)      n.x++;
    else if (dir == LEFT)  n.x--;
    else if (dir == UP)    n.y--;
    else if (dir == DOWN)  n.y++;

    return n;
}

int isSnakeAt(int x, int y)
{
    int i;

    for (i = 0; i < length; i++)
    {
        if (snake[i].x == x && snake[i].y == y)
            return 1;
    }
    return 0;
}

void buildMaze(int level)
{
    int x, y, i, k;
    Segment s;

    for (y = 0; y < HEIGHT; y++)
        for (x = 0; x < WIDTH; x++)
            maze[y][x] = 0;

    if (level == 2)
    {
        for (y = 2; y < 8; y++)   { maze[y][10] = 1; maze[y][29] = 1; }
        for (y = 12; y < 18; y++) { maze[y][10] = 1; maze[y][29] = 1; }
        for (x = 15; x < 25; x++) { maze[5][x] = 1; maze[14][x] = 1; }
    }
    else if (level == 3)
    {
        for (y = 2; y < 8; y++)
        {
            maze[y][10] = 1; maze[y][29] = 1; maze[y][20] = 1;
        }
        for (y = 12; y < 18; y++)
        {
            maze[y][10] = 1; maze[y][29] = 1; maze[y][20] = 1;
        }
        for (x = 5; x < 14; x++)  maze[4][x] = 1;
        for (x = 26; x < 35; x++) maze[15][x] = 1;
        for (x = 15; x < 25; x++) { maze[5][x] = 1; maze[14][x] = 1; }
    }

    /* never put a wall on the snake itself */
    for (i = 0; i < length; i++)
        maze[snake[i].y][snake[i].x] = 0;

    /* ...or in the 3 cells directly in front of the head (fair level change) */
    s = snake[0];
    for (k = 0; k < 3; k++)
    {
        s = nextPosition(&s, direction);
        if (s.x >= 0 && s.x < WIDTH && s.y >= 0 && s.y < HEIGHT)
            maze[s.y][s.x] = 0;
    }
}

int placeFood(void)
{
    int x, y, attempts;

    foodActive = 0;

    for (attempts = 0; attempts < MAX_LENGTH; attempts++)
    {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;

        if (maze[y][x] == 0 && !isSnakeAt(x, y) &&
            !(bonusActive && bonusX == x && bonusY == y))
        {
            foodX = x;
            foodY = y;
            foodActive = 1;
            return 1;
        }
    }

    /* random search failed: scan the whole board for a free cell */
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            if (maze[y][x] == 0 && !isSnakeAt(x, y) &&
                !(bonusActive && bonusX == x && bonusY == y))
            {
                foodX = x;
                foodY = y;
                foodActive = 1;
                return 1;
            }
        }
    }
    return 0;
}

int placeBonusFood(void)
{
    int x, y, attempts;

    for (attempts = 0; attempts < MAX_LENGTH; attempts++)
    {
        x = rand() % WIDTH;
        y = rand() % HEIGHT;

        if (maze[y][x] == 0 && !isSnakeAt(x, y) &&
            !(foodActive && foodX == x && foodY == y))
        {
            bonusX = x;
            bonusY = y;
            bonusActive = 1;
            bonusTimer = BONUS_DURATION;
            return 1;
        }
    }
    return 0;
}

/* speed increases with score: 150 ms at start, never faster than 60 ms */
int getDelay(int score)
{
    int delay = 150 - (score / 20) * 10;

    if (delay < 60)
        delay = 60;
    return delay;
}

void setupGame(Player *p)
{
    length = 3;
    p->score = 0;
    p->level = startLevel;

    gameOver = 0;
    gameWon = 0;
    paused = 0;
    foodsEaten = 0;

    direction = RIGHT;
    moveDir = RIGHT;

    bonusActive = 0;
    bonusTimer = 0;

    snake[0].x = WIDTH / 2;     snake[0].y = HEIGHT / 2;
    snake[1].x = WIDTH / 2 - 1; snake[1].y = HEIGHT / 2;
    snake[2].x = WIDTH / 2 - 2; snake[2].y = HEIGHT / 2;

    buildMaze(startLevel);
    placeFood();
}

/* ------------------------------- Drawing ----------------------------- */
/* Flicker-free: the whole board is built in memory and written to the
   console in one call, instead of clearing the screen every frame.     */
void drawGame(const Player *p)
{
    CHAR_INFO buf[(HEIGHT + 2) * (WIDTH + 2)];
    COORD size, zero;
    SMALL_RECT rect;
    char line[128];
    char bar[16];
    int x, y, i, filled;

    for (y = 0; y < HEIGHT + 2; y++)
    {
        for (x = 0; x < WIDTH + 2; x++)
        {
            if (y == 0 || y == HEIGHT + 1 || x == 0 || x == WIDTH + 1)
                putCell(buf, x, y, '#', COL_BORDER);
            else if (maze[y - 1][x - 1] == 1)
                putCell(buf, x, y, '#', COL_WALL);
            else
                putCell(buf, x, y, ' ', COL_DEFAULT);
        }
    }

    if (foodActive)
        putCell(buf, foodX + 1, foodY + 1, '*', COL_FOOD);
    if (bonusActive)
        putCell(buf, bonusX + 1, bonusY + 1, '+', COL_BONUS);

    for (i = length - 1; i >= 1; i--)
        putCell(buf, snake[i].x + 1, snake[i].y + 1, 'o', COL_BODY);
    putCell(buf, snake[0].x + 1, snake[0].y + 1, 'O', COL_HEAD);

    size.X = WIDTH + 2;
    size.Y = HEIGHT + 2;
    zero.X = 0;
    zero.Y = 0;
    rect.Left = 0;
    rect.Top = GRID_TOP;
    rect.Right = WIDTH + 1;
    rect.Bottom = GRID_TOP + HEIGHT + 1;
    WriteConsoleOutputA(hOut, buf, size, zero, &rect);

    /* text panels */
    snprintf(line, sizeof(line), " ENHANCED NOKIA SNAKE            Maze %d of %d",
             p->level, MAX_LEVEL);
    printAt(ROW_TITLE, COL_TITLE, line);

    snprintf(line, sizeof(line),
             " Player: %.12s   Score: %d   High: %d (%.12s)   Length: %d",
             p->name, p->score, p->highScore, p->highScoreName, length);
    printAt(ROW_STATS, COL_DEFAULT, line);

    if (bonusActive)
    {
        filled = (bonusTimer * 10 + BONUS_DURATION - 1) / BONUS_DURATION;
        for (i = 0; i < 10; i++)
            bar[i] = (i < filled) ? '#' : '.';
        bar[10] = '\0';
        snprintf(line, sizeof(line), " Bonus food! [%s]", bar);
        printAt(ROW_BONUS, COL_BONUS, line);
    }
    else
    {
        printAt(ROW_BONUS, COL_DEFAULT, "");
    }

    printAt(ROW_HELP1, COL_DEFAULT, " Arrows/WASD: Move   P: Pause   Q: Quit");
    printAt(ROW_HELP2, COL_DEFAULT,
            " * = 10 pts   + = 30 pts (timed bonus)   New maze every 50 pts");
    printAt(ROW_MSG, COL_DEFAULT, "");
}

/* -------------------------------- Input ------------------------------ */
/* dir ^ 1 is the opposite direction; compare with the direction actually
   moved last tick, so two quick key presses can never reverse the snake. */
void turn(int newDir)
{
    if (newDir != (moveDir ^ 1))
        direction = newDir;
}

void input(void)
{
    int key;

    while (_kbhit())
    {
        key = _getch();

        if (key == 0 || key == 224)
        {
            key = _getch();
            switch (key)
            {
                case 72: turn(UP);    break;
                case 80: turn(DOWN);  break;
                case 75: turn(LEFT);  break;
                case 77: turn(RIGHT); break;
            }
        }
        else
        {
            switch (key)
            {
                case 'w': case 'W': turn(UP);    break;
                case 's': case 'S': turn(DOWN);  break;
                case 'a': case 'A': turn(LEFT);  break;
                case 'd': case 'D': turn(RIGHT); break;
                case 'p': case 'P': paused = !paused; break;
                case 'q': case 'Q': gameOver = 1; break;
            }
        }
    }
}

/* ----------------------------- Game update --------------------------- */
void checkLevel(Player *p)
{
    int newLevel = (p->score / POINTS_PER_LEVEL) + 1;

    if (newLevel < startLevel)      /* never go back to an easier maze */
        newLevel = startLevel;
    if (newLevel > MAX_LEVEL)
        newLevel = MAX_LEVEL;

    if (newLevel != p->level)
    {
        p->level = newLevel;
        buildMaze(p->level);

        if (foodActive && maze[foodY][foodX] == 1)
        {
            if (!placeFood())
            {
                gameWon = 1;
                gameOver = 1;
            }
        }
        if (bonusActive && maze[bonusY][bonusX] == 1)
            bonusActive = 0;

        /* short pause so the player can see the new maze */
        drawGame(p);
        printAt(ROW_MSG, COL_TITLE, " LEVEL UP!  New maze...");
        Beep(1000, 150);
        Sleep(700);
    }
}
void crash(void)
{
    gameOver = 1;
    Beep(500, 150);
    Beep(400, 150);
    Beep(300, 300);
}

void updateGame(Player *p)
{
    Segment newHead, tail;
    int ateFood, ateBonus, limit, i;

    moveDir = direction;
    newHead = nextPosition(&snake[0], direction);

    /* wall / maze collision */
    if (newHead.x < 0 || newHead.x >= WIDTH ||
        newHead.y < 0 || newHead.y >= HEIGHT ||
        maze[newHead.y][newHead.x] == 1)
    {
        crash();
        return;
    }

    ateFood  = (foodActive  && newHead.x == foodX  && newHead.y == foodY);
    ateBonus = (bonusActive && newHead.x == bonusX && newHead.y == bonusY);

    /* self collision (the tail cell is free unless the snake is growing) */
    limit = ateFood ? length : length - 1;
    for (i = 0; i < limit; i++)
    {
        if (snake[i].x == newHead.x && snake[i].y == newHead.y)
        {
            crash();
            return;
        }
    }
    /* move the snake (and grow if it ate) */
    tail = snake[length - 1];
    for (i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];
    snake[0] = newHead;

    if (ateFood && length < MAX_LENGTH)
    {
        snake[length] = tail;
        length++;
    }

    if (ateFood)
    {
        p->score += POINTS_NORMAL;
        foodsEaten++;
        Beep(700, 60);

        if (!placeFood())
        {
            gameWon = 1;
            gameOver = 1;
            return;
        }
        if (foodsEaten % BONUS_EVERY == 0 && !bonusActive)
            placeBonusFood();
    }

    if (ateBonus)
    {
        p->score += POINTS_BONUS;
        bonusActive = 0;
        Beep(1200, 120);
    }

    /* bonus food countdown */
    if (bonusActive)
    {
        bonusTimer--;
        if (bonusTimer <= 0)
            bonusActive = 0;
    }

    if (ateFood || ateBonus)
        checkLevel(p);
}
/* ------------------------------ File handling ------------------------ */
void loadHighScore(Player *p)
{
    FILE *file;

    p->highScore = 0;
    strcpy(p->highScoreName, "None");

    file = fopen(SCORE_FILE, "r");
    if (file != NULL)
    {
        if (fscanf(file, "%29s %d", p->highScoreName, &p->highScore) != 2)
        {
            p->highScore = 0;
            strcpy(p->highScoreName, "None");
        }
        fclose(file);
    }
}
void saveHighScore(const Player *p)
{
    FILE *file = fopen(SCORE_FILE, "w");

    if (file != NULL)
    {
        fprintf(file, "%s %d\n", p->highScoreName, p->highScore);
        fclose(file);
    }
}

/* ------------------------------ Screens ------------------------------ */
void askName(Player *p)
{
    int i;

    system("cls");
    setColor(COL_TITLE);
    printf("\n  ====================================\n");
    printf("        ENHANCED NOKIA SNAKE GAME\n");
    printf("  ====================================\n");
    setColor(COL_DEFAULT);
    printf("\n  Enter your name: ");

    if (fgets(p->name, sizeof(p->name), stdin) == NULL)
        p->name[0] = '\0';
    p->name[strcspn(p->name, "\n")] = '\0';

    /* spaces would break the highscore file format, so replace them */
    for (i = 0; p->name[i] != '\0'; i++)
        if (p->name[i] == ' ')
            p->name[i] = '_';

    if (p->name[0] == '\0')
        strcpy(p->name, "Player");
}
int showMenu(const Player *p)
{
    int key;

    system("cls");
    setColor(COL_TITLE);
    printf("\n  ====================================\n");
    printf("        ENHANCED NOKIA SNAKE GAME\n");
    printf("  ====================================\n\n");
    setColor(COL_DEFAULT);
    printf("  Welcome, %s!\n", p->name);
    printf("  High Score: %d  (by %s)\n\n", p->highScore, p->highScoreName);
    printf("    1. Play\n");
    printf("    2. Instructions\n");
    printf("    3. Exit\n\n");
    printf("  Choose an option: ");

    for (;;)
    {
        key = _getch();
        if (key == '1') return 1;
        if (key == '2') return 2;
        if (key == '3') return 3;
    }
}
void showInstructions(void)
{
    system("cls");
    setColor(COL_TITLE);
    printf("\n  ============== HOW TO PLAY ==============\n\n");
    setColor(COL_DEFAULT);
    printf("  Move      : Arrow keys or W A S D\n");
    printf("  Pause     : P        Quit round: Q\n\n");
    printf("  *  normal food  = 10 points, snake grows\n");
    printf("  +  bonus food   = 30 points (appears after every\n");
    printf("                    5 normal foods, disappears quickly)\n\n");
    printf("  - Do not hit the walls, the maze, or your own body.\n");
    printf("  - The snake gets faster as your score grows.\n");
    printf("  - You choose the starting maze (1 to 3) before playing.\n");
    printf("  - The next, harder maze appears every 50 points.\n");
    printf("  - Your best score is saved in highscore.txt.\n\n");
    printf("  Press any key to return to the menu...");
    _getch();
}
/* lets the player pick which maze to start in */
int chooseMaze(void)
{
    int key;

    system("cls");
    setColor(COL_TITLE);
    printf("\n  ========== SELECT STARTING MAZE ==========\n\n");
    setColor(COL_DEFAULT);
    printf("    1. Maze 1 - Open field      (easy)\n");
    printf("    2. Maze 2 - Walls and bars  (medium)\n");
    printf("    3. Maze 3 - Tough maze      (hard)\n\n");
    printf("  The maze also changes every 50 points (never to an\n");
    printf("  easier one than you chose).\n\n");
    printf("  Choose 1, 2 or 3: ");

    for (;;)
    {
        key = _getch();
        if (key >= '1' && key <= '0' + MAX_LEVEL)
            return key - '0';
    }
}
/* returns 1 = play again, 0 = back to main menu */
int playGame(Player *p)
{
    int newRecord = 0;
    int key;
    char line[128];

    setupGame(p);
    system("cls");
    hideCursor(1);
    drawGame(p);

    while (!gameOver)
    {
        input();

        if (gameOver)
            break;

        if (paused)
        {
            printAt(ROW_MSG, COL_TITLE, " *** PAUSED - press P to resume ***");
            Sleep(60);
            continue;
        }

        updateGame(p);
        drawGame(p);
        Sleep(getDelay(p->score));
    }
    drawGame(p);

    if (p->score > p->highScore)
    {
        p->highScore = p->score;
        strcpy(p->highScoreName, p->name);
        saveHighScore(p);
        newRecord = 1;
    }

    printAt(ROW_HELP1, COL_TITLE,
            gameWon ? " YOU WON! There is no space left for food!"
                    : " GAME OVER!");
    snprintf(line, sizeof(line), " Final Score: %d%s", p->score,
             newRecord ? "    *** NEW HIGH SCORE! ***" : "");
    printAt(ROW_HELP2, COL_DEFAULT, line);
    printAt(ROW_MSG, COL_DEFAULT, " R = Restart     M = Main menu");

    for (;;)
    {
        key = _getch();
        if (key == 'r' || key == 'R') return 1;
        if (key == 'm' || key == 'M') return 0;
    }
}
/* -------------------------------- main ------------------------------- */
int main(void)
{
    Player player;
    int choice;
    int running = 1;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    system("mode con: cols=80 lines=30");
    srand((unsigned int)time(NULL));

    loadHighScore(&player);
    askName(&player);

    while (running)
    {
        hideCursor(0);
        choice = showMenu(&player);

        if (choice == 1)
        {
            startLevel = chooseMaze();
            while (playGame(&player))
                ;
        }
        else if (choice == 2)
        {
            showInstructions();
        }
        else
        {
            running = 0;
        }
    }
    hideCursor(0);
    setColor(COL_DEFAULT);
    system("cls");
    printf("\n  Thank you for playing Snake Game!\n");
    printf("  Developed by Darpan, Sudheer and Rafay (BCS-1B), FAST NUCES Karachi\n\n");

    return 0;
}