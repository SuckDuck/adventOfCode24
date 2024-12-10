#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "board.h"


int main(int argsc, char** args){
    if(argsc < 2 || argsc > 3){
        return 1;
    }
    char* input_file_path = args[1];
    char* debug_printing = argsc > 2 ? args[2] : "false";

    char* input_file;
    int input_file_l = read_input_file(input_file_path,&input_file);

    board b;
    board_init(&b,input_file,input_file_l);
    board_get_square(&b,b.guard.pos_x,b.guard.pos_y)->value = 1;


    int total = 0;
    int gs_x = b.guard.pos_x;
    int gs_y = b.guard.pos_y;
    // for each square
    for(int i=0; i<(b.cols*b.rows); i++){
        square* s = b.squares+i;
        if(s->x == b.guard.pos_x && s->y == b.guard.pos_y) continue;
        s->obstacle = 1;

        while (guard_step(&b) == 0){
            square* gs = board_get_square(&b,b.guard.pos_x,b.guard.pos_y);
            if(strcmp(debug_printing,"true") == 0){
                board_print(&b);
                usleep(100000);
            }
        
            // if guard's square was already touched 5 times
            gs->value += 1;
            if(gs->value > 4){
                total++;
                break;
            }
        }

        // set all squares values to 0
        for(int o=0; o<(b.cols*b.rows); o++){
            (b.squares+o)->value = 0;
        }
        
        // set every square to it's original value
        board_set_elements(&b,input_file,input_file_l);
        b.guard.orientation = 0;
        
    }
    
    printf("PART-2 ANSWER:%i\n",total);
    board_free(&b);
    free((void*) input_file);
    return 0;
}