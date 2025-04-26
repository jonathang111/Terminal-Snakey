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
                case 'A': 
                    if(lastdirection == DOWN) break;
                    lastdirection = UP; return UP;
                case 'B': 
                    if(lastdirection == UP) break;
                    lastdirection = DOWN; return DOWN;
                case 'C': 
                    if(lastdirection == LEFT) break;
                    lastdirection = RIGHT; return RIGHT;
                case 'D':
                    if(lastdirection == RIGHT) break;
                    lastdirection = LEFT; return LEFT;
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
                frame[i][p] = "#";//❚; flicker caused by these special chars
            else
            switch(board[i][p]){
                case NONE:      frame[i][p] = " "; break;
                case SNAKE:     frame[i][p] = "$"; break;
                case SNAKEHEAD: frame[i][p] = "O"; break;
                case APPLE:     frame[i][p] = "@";
            }
        }
        //printf("index: %i ", i);
    }
    //write to buffer
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
    //printf("\033c"); //use this; no ):<
    printf("\033[H");  //cursor top left
    printf("\033[J"); //clearscreen
    fwrite(framebuff, 1, offset, stdout);
    //printf("\n");
}

Direction PivotCheck(int x, int y, Pivots* pivots){
    for(int p = 0; p < pivots->size; p++)
        if(x == pivots->data[p].x && y == pivots->data[p].y){
            return pivots->data[p].direction;
        } 
    return MAINTAIN;
}

void RemoveOldestPivot(Pivots* pivots){ //shift pivots left
    for(int i = 0; i < pivots->size; i++)
        pivots->data[i] = pivots->data[i+1];
    pivots->size--;
}

void move(Snake* snake, Point** board, Pivots* pivots){
    //for head logic only
    Direction temp = MAINTAIN;
    if(snake->direction[0] == START)
        return;
    if(snake->direction[0] == MAINTAIN)
        snake->direction[0] = lastdirection;
    else
        SpawnPivot(pivots, snake->direction[0], snake->body[0].x, snake->body[0].y); //pivots->size +1 in func, +2 once return?

    for(int i = 0; i < snake->length; i++){
        ChangeBoard(snake->body[i].x, snake->body[i].y, board, NONE); //maybe only update head/tail
        //pivot logic.
        temp = PivotCheck(snake->body[i].x, snake->body[i].y, pivots);
        if(temp != MAINTAIN)
                snake->direction[i] = temp;
        if(i == snake->length - 1 && temp != MAINTAIN)
            RemoveOldestPivot(pivots);

        //move update
        switch(snake->direction[i]){
            case UP: snake->body[i].y = snake->body[i].y + 1;
            break;
            case DOWN: snake->body[i].y = snake->body[i].y - 1;
            break;
            case RIGHT: snake->body[i].x = snake->body[i].x + 1;
            break;
            case LEFT: snake->body[i].x = snake->body[i].x - 1;
            break;
            default: break; //makes compiler shut up
        }
        //update for draw()
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
    if(OutOfBounds) //most important thing, maybe add toggle
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
    int prev_y = snake->body[length].y;
    int prev_x = snake->body[length].x;

    switch(snake->direction[length]){
        case UP:
        snake->body[length+1].y = prev_y - 1;
        snake->body[length+1].x = prev_x;
        snake->direction[length+1] = UP;
        break;
        case DOWN:
        snake->body[length+1].y = prev_y + 1;
        snake->body[length+1].x = prev_x;
        snake->direction[length+1] = DOWN;
        break;
        case RIGHT:
        snake->body[length+1].x = prev_x - 1;
        snake->body[length+1].y = prev_y;
        snake->direction[length+1] = RIGHT;
        break;
        case LEFT:
        snake->body[length+1].x = prev_x + 1;
        snake->body[length+1].y = prev_y;
        snake->direction[length+1] = LEFT;
        break;
        default: break;
    }
    snake->length++;
}

Snake* InitalizeSnake(){
    Snake* temp = (Snake*)malloc(sizeof(Snake));
    temp->direction[0] = START;
    temp->length = 1;
    temp->body[0].x = 20;
    temp->body[0].y = 10;
    return temp;
}

void SpawnPivot(Pivots* pivots, Direction direction, int x, int y){
    if(pivots->size == MAX_PIVOT)
        return;
    Pivot temp;
    temp.direction = direction;
    temp.x = x;
    temp.y = y;
    pivots->data[pivots->size] = temp;
    pivots->size++;
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