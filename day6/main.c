#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ORIENTATION_CHARS "^>v<"

typedef int ORIENTATION_t;
enum ORIENTATION {
    ORIENTATION_UP,
    ORIENTATION_RIGHT,
    ORIENTATION_DOWN,
    ORIENTATION_LEFT
};

typedef struct square{
    int x,y;
    int activated, obstacle;
} square;

struct guard{
    int pos_x, pos_y;
    ORIENTATION_t orientation;
};

typedef struct board{
    int cols, rows;
    square* squares;
    struct guard guard;
} board;

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
        char c = s[i];
        switch(c){
            case '#':
            square* s = board_get_square(b,col,row);
            s->obstacle = 1;
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
        char c =(b->guard.pos_x == s->x && b->guard.pos_y == s->y) ? ORIENTATION_CHARS[b->guard.orientation] : ' ';
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
    board_set_elements(b,s,sl);
}

int guard_step(board *b){
    int retries = 0;
    for(int i=0; i<2; i++){
        int nx = 0;
        int ny = 0;
        switch(b->guard.orientation){
            case ORIENTATION_UP:    ny = -1;  break;
            case ORIENTATION_DOWN:  ny =  1;  break;
            case ORIENTATION_RIGHT: nx =  1;  break;
            case ORIENTATION_LEFT:  nx = -1;  break;
        }

        square* n_square = board_get_square(b,(b->guard.pos_x)+nx,(b->guard.pos_y)+ny);
        if(n_square == NULL || n_square->obstacle == 1){
            b->guard.orientation++;
            if(b->guard.orientation > 3) b->guard.orientation = 0;
            if(retries == 0){
                retries++;
                continue;
            }
            
            return -1;
        }
        
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

int main(int argsc, char** args){
    if(argsc != 2){
        return 1;
    }
    char* input_file_path = args[1];

    char* input_file;
    int input_file_l = read_input_file(input_file_path,&input_file);

    board b;
    board_init(&b,input_file,input_file_l);

    square* s = board_get_square(&b,b.guard.pos_x,b.guard.pos_y);
    s->activated = 1;
    while (guard_step(&b) == 0){
        s = board_get_square(&b,b.guard.pos_x,b.guard.pos_y);
        s->activated = 1;

        int total=0;
        for(int i=0; i<b.cols*b.rows; i++){
            total += (b.squares+i)->activated;
        }

        sleep(1);
        printf("%i\n",total);
    }

    /*int total=0;
    for(int i=0; i<b.cols*b.rows; i++){
        total += (b.squares+i)->activated;
    }

    printf("PART1 ANSWER:%i\n",total);
    board_free(&b);*/
    free((void*) input_file);
    return 0;
}