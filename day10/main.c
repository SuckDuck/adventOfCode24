#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct spot{
    int x,y;
    int height;
    int counted;
} spot;

typedef struct map{
    int cols, rows;
    int spots_c;
    spot* spots;
} map;

int char_to_int(char c){
    switch(c){
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default:  return -1;
    }
}

spot* map_get_spot(map* m, int x, int y){
    if(x >= m->cols || y >= m->rows || x<0 || y<0) return NULL;
    return m->spots +  y*m->cols+x;
}

void map_init(map* m, char* s, int s_l){
    
    // get the columns and rows count
    m->rows = 1;
    m->cols = 0;
    for(int i=0; i<s_l; i++){
        char c = s[i];
        if(c == '\n'){
            if(m->cols == 0) m->cols = i;
            m->rows += 1;
        }
    }


    // create the spots and initialize them
    m->spots_c = m->cols * m->rows;
    m->spots = (spot*) malloc(m->spots_c * sizeof(spot));

    int x = 0;
    int y = 0;
    for(int i=0; i<m->spots_c; i++){
        spot* s = m->spots+i;
        s->x = x;
        s->y = y;


        x++;
        if(x > m->cols-1){
            x=0;
            y++;
        }

    }

    // set the anthenas location
    int cc=0;
    for(int i=0; i<s_l; i++){
        char c = s[i];
        if(c == '\n') continue;
        (m->spots+cc)->height = char_to_int(c);
        cc++;
    }

}

void map_reset_counted(map* m){
    for(int i=0; i<m->spots_c; i++){
        spot* s=m->spots+i;
        s->counted=0;
    }
}

void map_print(map* m){
    printf("COLUMNS:%i  ROWS:%i\n",m->cols,m->rows);
    for(int y=0; y<m->rows; y++){
        for(int x=0; x<m->cols; x++){
            spot* s = map_get_spot(m,x,y);
            printf("%i ",s->height);
        }
        printf("\n");
    }
}

void map_free(map* m){
    free(m->spots);
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

void reach_to_nine(map* m,spot* s, int* score, int* rating){
    if(s->height == 9){
        *rating += 1;
        if(s->counted == 0){
            s->counted = 1;
            *score += 1;
        }
    }
    
    int rs=0;
    int directions[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    // for each direction
    for(int i=0; i<4; i++){
        spot* next_spot = map_get_spot(m,(s->x)+directions[i][0],(s->y)+directions[i][1]);
        if(next_spot != NULL && next_spot->height == (s->height)+1){
            reach_to_nine(m,next_spot,score,rating);
        }
    }

}

int main(int argsc, char** args){
    if(argsc != 2){
        printf("usage: day10 [input file path]\n");
        return 1;
    }
    char* input_file_path = args[1];

    char* input_str;
    int input_len = read_input_file(input_file_path,&input_str);

    map m;
    map_init(&m,input_str,input_len);

    // for each spot
    int total_score=0;
    int total_rating=0;
    for(int i=0; i<m.spots_c; i++){
        map_reset_counted(&m);
        spot* s = m.spots+i;
        if(s->height == 0){
            int score=0;
            int rating=0;
            reach_to_nine(&m,s,&score,&rating);

            total_score  += score;
            total_rating += rating;
        }
    }

    printf("TOTAL SCORE  (part#1): %i\n",total_score);
    printf("TOTAL RATING (part#2): %i\n",total_rating);

    map_free(&m);
    free(input_str);
    return 0;
}