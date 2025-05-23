#ifndef CLASSES
#define CLASSES
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 40
#define MAX_LENGTH 100
#define MAX_FPS 30
#define MAX_PIVOT 64
#define FRAME_DELAY 1000000 / MAX_FPS
#define OutOfBounds (snake->body[0].y >= HEIGHT || snake->body[0].y == 0)\
|| (snake->body[0].x == 0 || snake->body[0].x >= WIDTH)
#define FRAMEBUFF_SIZE ((HEIGHT+2)*(WIDTH+2)) * 4 + 64
typedef enum {
    NONE = 0,
    SNAKE = 1,
    APPLE = 3, //may not need apple. Do need for drawing
    SNAKEHEAD = 4,
} Point;

typedef enum {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    MAINTAIN,
    START
} Direction;

typedef enum {
    S = 0,
    W = 1,
    A = 2,
    N = 3,
    P = 4
} CollisionType;

typedef struct {
    int x;
    int y;
} Body;

typedef struct {
    int x;
    int y;
    Direction direction;
} Pivot;

typedef struct {
    int size;
    Pivot data[MAX_PIVOT]; //data holds pivot info, x, y, direction
} Pivots;

typedef struct { //need to add segment logic
    Body body[MAX_LENGTH];
    Direction direction[MAX_LENGTH];
    int length;
} Snake;

//not in logic loop
Point** InitalizeBoard(int, int);
Snake* InitalizeSnake();
void ChangeBoard(int x, int y, Point** board, Point);
void SpawnApple(Point**);
void SpawnPivot(Pivots*, Direction, int, int);
Direction PivotCheck(int x, int y, Pivots*);
void RemoveOldestPivot(Pivots*);

//in logic loop
void draw(Point**);
void move(Snake*, Point**, Pivots*);
CollisionType CollideCheck(Snake*, Point**);
Direction listen();
void GrowSnake(Snake*);

//terminal reflagging
void NonBlocking();
void TermoisSetNonBlocking(struct termios*);
void TermoisRestore(struct termios*);

extern char seq, seq1, seq2;
extern int apple_x, apple_y;
extern Direction lastdirection;
#endif