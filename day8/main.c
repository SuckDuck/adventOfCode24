#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct spot{
    int x,y;
    char anthenna_type;
    int antinode;
} spot;

typedef struct map{
    int cols, rows;
    int spots_c;
    spot* spots;
} map;

spot* map_get_spot(map* m, int x, int y){
    if(x >= m->cols || y >= m->rows || x<0 || y<0) return NULL;
    return m->spots +  y*m->rows+x;
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
        spot* s = (m->spots)+i;
        s->x = x;
        s->y = y;

        x++;
        if(x > m->cols-1){
            x=0;
            y++;
        }

        s->anthenna_type = '.';
        s->antinode = 0;
    }

    // set the anthenas location
    x = 0;
    y = 0;
    for(int i=0; i<s_l; i++){
        char c = s[i];
        if(c == '\n') continue;

        if(c != '.'){
            spot* s = map_get_spot(m,x,y);
            s->anthenna_type = c;
        }

        x++;
        if(x > m->cols-1){
            x=0;
            y++;
        }
    }

}

void map_print(map* m){
    printf("COLUMNS:%i  ROWS:%i\n",m->cols,m->rows);
    for(int y=0; y<m->rows; y++){
        for(int x=0; x<m->cols; x++){
            spot* s = map_get_spot(m,x,y);
            printf("[%c] ",s->anthenna_type);
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

int main(int argsc, char** args){
    if(argsc != 2) return 1;
    char* input_file_path = args[1];

    char* input_string;
    int input_string_l = read_input_file(input_file_path,&input_string);

    map m;
    map_init(&m,input_string,input_string_l);
    map_print(&m);

    free(m.spots);
    return 0;
}