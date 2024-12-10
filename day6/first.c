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

    square* s = board_get_square(&b,b.guard.pos_x,b.guard.pos_y);
    s->value = 1;
    while (guard_step(&b) == 0){
        s = board_get_square(&b,b.guard.pos_x,b.guard.pos_y);
        s->value = 1;

        if(strcmp(debug_printing,"true") == 0){
            board_print(&b);
            usleep(100000);
        }
    }

    int total=0;
    for(int i=0; i<b.cols*b.rows; i++){
        total += (b.squares+i)->value;
    }

    printf("PART-1 ANSWER:%i\n",total);
    board_free(&b);
    free((void*) input_file);
    return 0;
}