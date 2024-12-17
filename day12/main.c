#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

const int DIRECTIONS[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

typedef struct spot spot;
typedef struct group group;
typedef struct map map;

typedef struct spot{
    int x,y;
    char plant;
    bool in_group_flag;
} spot;

typedef struct group{
    int spots_c;
    spot** spots;
    int perimeter;
    int area;
} group;

typedef struct map{
    int cols, rows;
    
    int spots_c;
    spot* spots;
    
    int groups_c;
    group* groups;
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

    }

    // set the plant per spot
    int cc=0;
    for(int i=0; i<s_l; i++){
        char c = s[i];
        if(c == '\n') continue;
        (m->spots+cc)->plant = c;
        (m->spots+cc)->in_group_flag = 0;
        cc++;
    }

}

void map_reset_flags(map* m){
    for(int i=0; i<m->spots_c; i++){
        spot* s = m->spots+i;

        s->in_group_flag = false;
    }
}

void map_print(map* m){
    printf("COLUMNS:%i  ROWS:%i\n",m->cols,m->rows);
    printf("GROUPS: %i\n",m->groups_c);
    for(int i=0; i<m->groups_c; i++){
        group* g = m->groups+i;
        printf("  |__ SPOTS: %i\n",g->spots_c);
        for(int o=0; o<g->spots_c; o++){
            int x = m->groups[i].spots[o]->x;
            int y = m->groups[i].spots[o]->y;
            printf("  %c   |__ X:%i Y:%i\n",i==m->groups_c-1?' ':'|',x,y);
        }
        printf("  %c\n",i==m->groups_c-1?' ':'|');
    }
    

    for(int y=0; y<m->rows; y++){
        for(int x=0; x<m->cols; x++){
            spot* s = map_get_spot(m,x,y);
            printf("%c",s->plant);
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

int explore_group(map* m, spot* s){
    int c=0;
    s->in_group_flag = true;
    
    // for each of the 4 spots around the current spot
    for(int i=0; i<4; i++){
        spot* ns = map_get_spot(m,s->x+DIRECTIONS[i][0],s->y+DIRECTIONS[i][1]);
        if(ns != NULL){
            if(s->plant == ns->plant && ns->in_group_flag == false){
                c += 1 + explore_group(m,ns);
            }
        }        
    }

    return c;
}

int get_groups(map* m, group* groups){
    int groups_c=0;

    // for each spot in the map
    for(int i=0; i<m->spots_c; i++){
        spot* s=m->spots+i;

        if(s->in_group_flag == false){            
            int group_len = explore_group(m,s);
            group_len++; //compensation

            if(groups != NULL){
                group* g = groups+groups_c;
                g->spots_c = group_len;
                g->spots = (spot**) malloc(group_len * sizeof(spot*));

                
                // TEMP SOLUTION
                for(int o=0; o<group_len; o++){
                    *(g->spots+o) = m->spots+1;
                }
            }

            groups_c++;
        }
    }

    return groups_c;
}

int main(int argsc, char** args){
    if(argsc != 2){
        printf("usage: day12 [input file path]\n");
        return 1;
    }
    char* input_file_path = args[1];

    char* input_str;
    int input_len = read_input_file(input_file_path,&input_str);

    map m;
    map_init(&m,input_str,input_len);

    
    m.groups_c = get_groups(&m,NULL);
    m.groups = (group*) malloc(m.groups_c * sizeof(group));
    map_reset_flags(&m);
    get_groups(&m,m.groups);

    map_print(&m);
    map_free(&m);
    free(input_str);
    return 0;
}