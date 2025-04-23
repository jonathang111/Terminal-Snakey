#include "classes.h"
//notes:
/*Only need to read the head to find crash or to find whether apple was eaten
Where should i place the crash logic? move or draw, either or it would be simple.
Will place into own function for now since i can return for apple or wall crash or none.

x starts at 0 too? or should it start at 1 for simplicity and handle that within ChangeBoard()?
Logic for y being upside down will be handeled within ChangeBoard() function
*/
char seq; //vars for listen(), didn't wanna redeclare each time.
char seq1;
char seq2;
Direction lastdirection;

int main (){
    Point** board = InitalizeBoard(HEIGHT, WIDTH);
    Snake* snake = InitalizeSnake();
    //use usleep() for wait? si.
    TermoisSetNonBlocking();
    NonBlocking();
    int test, tick = 0;
    CollisionType collide;
    while(seq != 'q'){
        usleep(100000);
        snake->direction = listen();
        move(snake, board);
        collide = CollideCheck(snake, board);
        if(collide == W){
            printf("You lose\n");
            break;
        }
        draw(snake, board);
        printf("Collide Type: %u\n", collide);
        printf("Y-Axis: %i\n", snake->body[0].y);
        printf("Direction: %u\n", snake->direction);
        tick++;
    }
    printf("%i", tick);
}