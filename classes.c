#include "classes.h"

void TermoisSetNonBlocking(struct termios* original){
    struct termios term;
    term = *original;
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

void TermoisRestore(struct termios* original){
    tcsetattr(0, TCSANOW, original);

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

void draw(Point** board){
    char *frame[HEIGHT+2][WIDTH+2];
    for(int i = 0; i < HEIGHT+1; i++)
    {
        for(int p = 0; p < WIDTH; p++){
            if((i == 0 || i == HEIGHT)
            || (p == 0 || p == WIDTH-1))
                frame[i][p] = "❚";
            else
            switch(board[i][p]){
                case NONE:
                frame[i][p] = " ";
                break;
                case SNAKE:
                frame[i][p] = "$";
                break;
                case SNAKEHEAD:
                frame[i][p] = "⬤";
                break;
                case APPLE:
                frame[i][p] = "@";
            }
        }
        //printf("index: %i ", i);
    }
    const char *symbol;
    int offset = 0;
    size_t size = 0;
    char framebuff[FRAMEBUFF_SIZE];
    for(int i = 0; i < HEIGHT+1; i++){
        for(int p = 0; p < WIDTH; p++){
            symbol = frame[i][p];
            size = strlen(symbol);
            memcpy(&framebuff[offset], symbol, size);
            offset += size;
        }
        framebuff[offset++] = '\n';
    }
    //system("clear");
    printf("\033c"); //use this
    fwrite(framebuff, 1, offset, stdout);
    //printf("\n");
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

void SpawnApple(Point** board){
    board[10][24] = APPLE;
}

CollisionType CollideCheck(Snake* snake, Point** board){
    if(OutOfBounds) //most important thing
        return W;
    
    Point type = board[(HEIGHT - snake->body[0].y)][snake->body[0].x];
    switch(type){
        case SNAKE:
        return S;
        case APPLE:
        return A;
        default: break; //also make compiler shut up
    }
    return N;
}

void GrowSnake(Snake* snake){
    int length = snake->length-1;
    int prev_y = snake.body[length].y;
    int prev_x = snake.body[length].x;

    
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
}

Snake* InitalizeSnake(){
    Snake* temp = (Snake*)malloc(sizeof(Snake));
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
    Point** temp = (Point**)malloc((height+2) * sizeof(Point*));
    for(int i = 0; i < height+2; i++){
        temp[i] = (Point*)malloc((width+2) * sizeof(Point));
    }
    return temp;
}