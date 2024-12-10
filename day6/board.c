#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "board.h"

square* board_get_square(board* b, int x, int y){
    if(x >= b->cols || y >= b->rows || x<0 || y<0) return NULL;
    return b->squares +  y*b->rows+x;
}

void board_set_dimensions(board* b, char* s, int sl){
    b->cols=0;
    for(int i=0; i<sl; i++){
        if(s[i] == '\n') break;
        b->cols += 1;
    }

    b->rows=0;
    for(int i=0; i<sl; i++){
        if(s[i] == '\n') b->rows += 1;
    }

    if(b->rows > 0) b->rows += 1;
}

void board_set_elements(board* b, char* s, int sl){
    int col=0;
    int row=0;
    for(int i=0; i<sl; i++){
        square* sq = board_get_square(b,col,row);
        char c = s[i];
        switch(c){
            case '#':
            sq->obstacle = 1;
            break;

            case '.':
            sq->obstacle = 0;
            break;

            case '^':
            b->guard.pos_x = col;
            b->guard.pos_y = row;
            break;

            case '\n':
            col=-1;
            row++;
            break;
        }
        
        col++;
    }
}

void board_print(board* b){
    printf("COLS:%i  ROWS:%i",b->cols,b->rows);
    for(int i=0; i<b->cols * b->rows; i++){
        square* s = b->squares+i;
        if(s->x == 0) printf("\n");
        char c = s->value>0 ? 'X' : ' ';
        c =(b->guard.pos_x == s->x && b->guard.pos_y == s->y) ? ORIENTATION_CHARS[b->guard.orientation] : c;
        c = s->obstacle == 1 ? '#':c;
        printf("[%c] ",c);
    }
    printf("\n");
}

void board_free(board* b){
    free(b->squares);
}

void board_init(board* b, char* s, int sl){
    board_set_dimensions(b,s,sl);
    b->squares = (square*) malloc(sizeof(square) * (b->cols*b->rows));
    int x, y = 0;
    for(int i=0; i<(b->cols * b->rows); i++){
        b->squares[i].x = x;
        b->squares[i].y = y;

        x++;
        if(x > b->cols-1){
            x=0;
            y++;
        }
    }

    b->guard.orientation=ORIENTATION_UP;
    b->guard.pos_x = 0;
    b->guard.pos_y = 0;
    b->guard.total_dissplacement = 0;
    board_set_elements(b,s,sl);
}

int guard_step(board *b){
    while(1){
        int nx = 0;
        int ny = 0;
        switch(b->guard.orientation){
            case ORIENTATION_UP:    ny = -1;  break;
            case ORIENTATION_DOWN:  ny =  1;  break;
            case ORIENTATION_RIGHT: nx =  1;  break;
            case ORIENTATION_LEFT:  nx = -1;  break;
        }

        square* n_square = board_get_square(b,(b->guard.pos_x)+nx,(b->guard.pos_y)+ny);
        if(n_square == NULL) return -1;
        if(n_square->obstacle == 1){
            b->guard.orientation++;
            if(b->guard.orientation > 3) b->guard.orientation = 0;
            continue;
            
        }
        
        b->guard.total_dissplacement += 1;
        b->guard.pos_x = n_square->x;
        b->guard.pos_y = n_square->y;
        return 0;
    }
    
}

int read_input_file(char* path, char** output){
    struct stat st;
    stat(path,&st);
    int fd = open(path,O_RDONLY);
    
    *output = (char*) malloc(st.st_size);
    int bytes_read=0;
    while (bytes_read < st.st_size){
        bytes_read += read(fd,(void*)((*output)+bytes_read),st.st_size-bytes_read);
    }

    close(fd);
    return st.st_size;
}
