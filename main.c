#include "classes.h"
//notes:
/*set up snake segement, set up framrate control (so vertical looks consistent w/ horiz)
need to also set up snake eat logic, and maybe add a frame buffer for better rendering.
Wall toggle?? would be cool
Apple blink
Death screen + restart option
input buffering
score/ speedup
do not allow opposite directions sequentially, i.e. no up to down
*/
char seq; //vars for listen(), didn't wanna redeclare each time.
char seq1;
char seq2;
Direction lastdirection;

int main (){
    Point** board = InitalizeBoard(HEIGHT, WIDTH);
    Snake* snake = InitalizeSnake();

    struct termios* original = (struct termios*)malloc(sizeof(struct termios*));
    tcgetattr(0, original);
    TermoisSetNonBlocking(original);
    NonBlocking();
    int test, tick = 0, debug = 1, applehit = 0;
    CollisionType collide;
    //printf("\033[2J"); //clrscrn
    printf("\033[H"); //cursor top left
    printf("\033[?25l"); //hide cursor
    //printf("\n");

    while(seq != 'q'){
        usleep(100000);
        snake->direction[0] = listen();
        move(snake, board);
        draw(board);
        if(debug == 1){
        printf("Collide Type: %u\n", collide);
        printf("X-Axis: %i\n", snake->body[0].x);
        printf("Y-Axis: %i\n", snake->body[0].y);
        printf("Direction: %u\n", snake->direction[0]);
        printf("Apple Hits: %i\n", applehit);
        }

        SpawnApple(board);
        collide = CollideCheck(snake, board);
        switch(collide){
            case W:
            printf("You lose\n");
            seq = 'q';
            break;
            case S:
            printf("You lose\n");
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