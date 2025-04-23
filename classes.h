#ifndef CLASSES
#define CLASSES
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define HEIGHT 20
#define WIDTH 40
#define MAX_LENGTH 100
#define OutOfBounds (snake->body[0].y >= HEIGHT || snake->body[0].y <= 0)\
|| (snake->body[0].x <= 0 || snake->body[0].x >= WIDTH)

typedef enum {
    NONE = 0,
    SNAKE = 1,
    APPLE = 3
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
    N = 3
} CollisionType;

typedef struct {
    int x;
    int y;
} Body;

typedef struct {
    Body body[MAX_LENGTH];
    Direction direction;
    int length;
} Snake;

//not in logic loop
Point** InitalizeBoard(int, int);
Snake* InitalizeSnake();
void ChangeBoard(int x, int y, Point** board, Point);

//in logic loop
void draw(Snake*, Point**);
void move(Snake*, Point**);
CollisionType CollideCheck(Snake*, Point**);
Direction listen();

//terminal reflagging
void NonBlocking();
void TermoisSetNonBlocking();

extern char seq, seq1, seq2;
extern Direction lastdirection;
#endif