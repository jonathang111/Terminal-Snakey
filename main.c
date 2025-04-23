#include "classes.h"
//notes:
/*Only need to read the head to find crash or to find whether apple was eaten
    right now the problem is that the y axis is resulting in seg faults. Need to 
    investigate the move function, probably isolate the problem in the change board type.
*/
char seq; //vars for listen(), didn't wanna redeclare each time.
char seq1;
char seq2;
Direction lastdirection;

int main (){
    Point** board = InitalizeBoard(HEIGHT, WIDTH);
    Snake* snake = InitalizeSnake();

    struct termios* original;
    tcgetattr(0, original);
    TermoisSetNonBlocking(original);
    NonBlocking();
    int test, tick = 0, debug = 1;
    CollisionType collide;
    //printf("\033[2J"); //clrscrn
    //printf("\033[H"); //cursor top left
    //printf("\033[?25l"); //hide cursor
    //printf("\n");

    while(seq != 'q'){
        usleep(100000);
        snake->direction = listen();
        move(snake, board);
        draw(board);
        if(debug == 1){
        printf("Collide Type: %u\n", collide);
        printf("X-Axis: %i\n", snake->body[0].x);
        printf("Y-Axis: %i\n", snake->body[0].y);
        printf("Direction: %u\n", snake->direction);
        }
        collide = CollideCheck(snake, board);
        if(collide == W){
            printf("You lose\n");
            break;
        }
        tick++;
    }
    printf("\033[?25h"); //restore cursor
    printf("%i\n", tick);
    TermoisRestore(original);
}