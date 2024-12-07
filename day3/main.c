#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utils.h"

int main(int argsc, char** args){
    if(argsc != 3){
        fprintf(stderr,"no arguments!:  [challenge part: (first_half or second_half)]  [input file name]\n");
        return 1;
    }

    int do_control = strcmp(args[1],"second_half") == 0;
    char* input_c;
    int input_l = read_input_file(args[2],&input_c);
    int patterns_c = get_mul_patterns(input_c,input_l,NULL,do_control);
    mul_pattern patterns[patterns_c];
    get_mul_patterns(input_c,input_l,patterns,do_control);
    
    unsigned long result = 0;
    for(int i=0; i<patterns_c; i++){
        result+=patterns[i].A * patterns[i].B;
    }

    printf("RESULT: %lu    LEN: %i\n",result,patterns_c);

    free(input_c);
    return 0;
}