#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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

        s->anthenna_type = '.';
        s->antinode = 0;
    }

    // set the anthenas location
    int cc=0;
    for(int i=0; i<s_l; i++){
        char c = s[i];
        if(c == '\n') continue;
        if(c != '.') (m->spots+cc)->anthenna_type = c;
        cc++;
    }

}

void map_print(map* m){
    printf("COLUMNS:%i  ROWS:%i\n",m->cols,m->rows);
    for(int y=0; y<m->rows; y++){
        for(int x=0; x<m->cols; x++){
            spot* s = map_get_spot(m,x,y);
            if(s->anthenna_type != '.')
                printf("%c",s->anthenna_type);

            else printf("%c",s->antinode > 0 ? '#': s->anthenna_type);
            
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

void calculate_antinodes_recursive(map* m, spot* s,int x, int y){
    spot* sn = map_get_spot(m,(s->x) + x, (s->y) + y);
    if(sn != NULL){
        sn->antinode = 1;
        calculate_antinodes_recursive(m,sn,x,y);
    }
}

int main(int argsc, char** args){
    if(argsc != 3 || (strcmp("first_part",args[2]) != 0 && strcmp("second_part",args[2]) != 0)){
        printf("usage: day8 [input file path] [ first_part | second_part ]\n");
        return 1;
    }
    char* input_file_path = args[1];
    int puzzle_part = strcmp("second_part",args[2]) == 0 ? 2 : 1;


    char* input_string;
    int input_string_l = read_input_file(input_file_path,&input_string);

    map m;
    map_init(&m,input_string,input_string_l);
    
    //for each spot in the map - first
    for(int i=0; i<m.spots_c; i++){
        spot* s0 =m.spots + i;
        if(s0->anthenna_type != '.'){
            // for each spot in the map - second
            for(int o=0; o<m.spots_c; o++){
                spot* s1 =m.spots + o;
                if(s1->anthenna_type == s0->anthenna_type && s0->x != s1->x && s0->y != s1->y){
                    int sx = s0->x - s1->x;
                    int sy = s0->y - s1->y;

                    if(puzzle_part == 1){
                        spot* sn = map_get_spot(&m,(s0->x) + sx, (s0->y) + sy);
                        if(sn != NULL) sn->antinode = 1;
                    }

                    else calculate_antinodes_recursive(&m,s0,sx,sy);
                    
                }
            }
        }
    }

    // for each spot in the map - again
    int result = 0;
    for(int i=0; i<m.spots_c; i++){
        spot* s =m.spots + i;
        if(s->antinode > 0 || s->anthenna_type != '.') result++;
    }

    
    map_print(&m);
    printf("ANSWER = %i\n",result);

    free(m.spots);
    free(input_string);
    return 0;
}