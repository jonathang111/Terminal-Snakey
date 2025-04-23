#include "classes.h"

void TermoisSetNonBlocking(){
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW,&term);
}

void NonBlocking(){
    int flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, flags | O_NONBLOCK); //set non blocking char
}

Direction listen(){
    seq = getchar();
    if(seq == '\033'){
        seq1 = getchar();
        seq2 = getchar();
        if(seq1 == '['){
            switch(seq2){
                case 'A': printf("Up\n"); lastdirection = UP; return UP;
                case 'B': printf("Down\n"); lastdirection = DOWN; return DOWN;
                case 'C': printf("Right\n"); lastdirection = RIGHT; return RIGHT;
                case 'D': printf("Left\n"); lastdirection = LEFT; return LEFT;
            }
        }
    }
    return MAINTAIN;
}

void draw(Point** board){ //maybe don't need snake for draw
    //printf("\033[H"); //clrsrn
    for(int i = 0; i < HEIGHT+1; i++)
    {
        for(int p = 0; p < WIDTH; p++){
            if((i == 0 || i == HEIGHT)
            || (p == 0 || p == WIDTH-1))
                printf("█");
            else
            switch(board[i][p]){
                case NONE:
                printf(" ");
                break;
                case SNAKE:
                printf("$");
                break;
                case SNAKEHEAD:
                printf("^");
                break;
                case APPLE:
                printf("@");
            }
        }
        printf("\n");
    }
    fflush(stdout);//flush output to prevent visual glitch
}

void move(Snake* snake, Point** board){
    if(snake->direction == START)
        return;
    if(snake->direction == MAINTAIN)
        snake->direction = lastdirection;
    for(int i = 0; i < snake->length; i++){
        ChangeBoard(snake->body[i].x, snake->body[i].y, board, NONE);
        switch(snake->direction){
            case UP:
            snake->body[i].y = snake->body[i].y + 1;
            break;
            case DOWN:
            snake->body[i].y = snake->body[i].y - 1;
            break;
            case RIGHT:
            snake->body[i].x = snake->body[i].x + 1;
            break;
            case LEFT:
            snake->body[i].x = snake->body[i].x - 1;
            break;
            default: break; //makes compiler shut up
        }
        if(i == 0)
            ChangeBoard(snake->body[i].x, snake->body[i].y, board, SNAKEHEAD);
        else
        ChangeBoard(snake->body[i].x, snake->body[i].y, board, SNAKE);
    }
}

CollisionType CollideCheck(Snake* snake, Point** board){
    if(OutOfBounds) //most important thing
        return W;
    
    Point type = board[(HEIGHT - snake->body[0].y) - 1][snake->body[0].x - 1];
    switch(type){
        case SNAKE:
        return S;
        case APPLE:
        return A;
        default: break; //also make compiler shut up
    }
    return N;
}

Snake* InitalizeSnake(){
    Snake* temp = malloc(sizeof(Snake));
    temp->direction = START;
    temp->length = 1;
    temp->body[0].x = 20;
    temp->body[0].y = 10;
    return temp;
}

void ChangeBoard(int x, int y, Point** board, Point type){
    y = HEIGHT+1 - y;
    board[y-1][x] = type;
}

Point** InitalizeBoard(int height , int width){
    Point** temp = malloc((height+2) * sizeof(Point*));
    for(int i = 0; i < height+2; i++){
        temp[i] = malloc((width+2) * sizeof(Point));
    }
    return temp;
}