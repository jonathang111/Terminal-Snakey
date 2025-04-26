#include "classes.h"
//notes:
/*set up snake segement, set up framrate control (so vertical looks consistent w/ horiz)
need to also set up snake eat logic, and maybe add a frame buffer for better rendering.
Wall toggle?? would be cool
Apple blink
Death screen + restart option
input buffering
score/ speedup
*/
char seq, seq1 , seq2;
int apple_x, apple_y;
Direction lastdirection;

int main (int argc, char* argv[]){
    srand(time(NULL)); //for apple spawn
    Point** board = InitalizeBoard(HEIGHT, WIDTH);
    Snake* snake = InitalizeSnake();
    Pivots* pivots = (Pivots*)malloc(sizeof(Pivots));
    pivots->size = 0;
    int test, tick = 0, debug = argc-1, applehit = 0;
    CollisionType collide;

    struct termios* original = (struct termios*)malloc(sizeof(struct termios*));
    tcgetattr(0, original);
    TermoisSetNonBlocking(original);
    NonBlocking();

    //printf("\033[2J"); //clrscrn
    printf("\033[H"); //cursor top left
    printf("\033[?25l"); //hide cursor
    //printf("\n");

    SpawnApple(board);
    //main logic loop
    while(seq != 'q'){
        usleep(100000);
        snake->direction[0] = listen();
        move(snake, board, pivots);
        draw(board);
        if(debug == 1){
        printf("Collide Type: %u\n", collide);
        printf("X-Axis: %i\n", snake->body[0].x);
        printf("Y-Axis: %i\n", snake->body[0].y);
        printf("Snake Size: %i\n", snake->length);
        printf("Direction: %u\n", snake->direction[0]);
        printf("Apple Hits: %i\n", applehit);
        printf("Apple Coords: x: %i  y: %i\n", apple_x, apple_y);
        printf("Pivots: %i\n", pivots->size);
        printf("Pivot 1: x: %i  y: %i\n",pivots->data[0].x, pivots->data[0].y);
        //printf("Pivot 2: x: %i  y: %i\n",pivots->data[1].x, pivots->data[1].y);
        }
        collide = CollideCheck(snake, board);
        switch(collide){ //make function instead?
            case W:
            printf("You lose\n");
            seq = 'q';
            break;
            case S:
            printf("You lose\n");
            seq = 'q';
            break;
            case A:
            applehit++;
            GrowSnake(snake);
            SpawnApple(board);
            break;
            default: break;
        }
        tick++;
    }
    printf("\033[?25h"); //restore cursor
    printf("Ticks: %i\n", tick);
    TermoisRestore(original);
}