#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "utils.h"

int is_it_a_number(char n){
    switch (n){
        case('0'): return 1; break;
        case('1'): return 1; break;
        case('2'): return 1; break;
        case('3'): return 1; break;
        case('4'): return 1; break;
        case('5'): return 1; break;
        case('6'): return 1; break;
        case('7'): return 1; break;
        case('8'): return 1; break;
        case('9'): return 1; break;
        default  : return 0; break;
    }
}

size_t read_input_file(char* path, char** output){
    struct stat s;
    stat(path,&s);
    int fd = open(path,O_RDONLY);
    
    int bytes_read = 0;
    size_t total_bytes = s.st_size;
    *output = (char*) malloc(total_bytes);
    while (bytes_read < total_bytes){
        bytes_read += read(fd,*output+bytes_read,total_bytes-bytes_read);
    }

    return total_bytes;
}

int get_mul_patterns(char* text, size_t text_len, mul_pattern* output, int enable_do_control){
    char* pattern = "mul(";
    int pattern_count = 0;

    char* do_pattern = "do()";
    char* dont_pattern = "don't()";
    int mul_enable = 1;

    int p_i = 0;
    int do_p_i = 0;
    int dont_p_i = 0;
    for(int i=0; i<text_len; i++){
        char c=text[i];
        
        //DO PATTERN
        if(c == do_pattern[do_p_i] && enable_do_control == 1)  do_p_i++;
        else do_p_i = 0;
        if(do_p_i == 4){
            mul_enable = 1;
            p_i = dont_p_i = do_p_i = 0;
        }

        //DONT PATTERN
        if(c == dont_pattern[dont_p_i] && enable_do_control == 1) dont_p_i++;
        else dont_p_i = 0;
        if(dont_p_i == 7){
            mul_enable = p_i = dont_p_i = do_p_i = 0;
        }

        if(p_i<5 && mul_enable==1){ //FIRST PATTERN TEST
            if(c == pattern[p_i]) p_i++;
            else p_i = 0;
        }
                
        if(p_i == 4){
            //SECOND PATTERN TEST
            int n_c0=0;
            char second_pattern[4] = "\0\0\0\0"; 
            for(int o=1; o<4; o++){
                if(is_it_a_number(text[i+o]) == 1){
                    second_pattern[n_c0] = text[i+o];
                    n_c0++;
                    continue;
                }

                break;
            }

            if(n_c0 == 0) goto exit_;
            if(text[i+n_c0+1] != ',') goto exit_;

            //THIRD PATTERN TEST
            int n_c1=0;
            char third_pattern[4] = "\0\0\0\0"; 
            for(int o=1; o<4; o++){
                if(is_it_a_number(text[i+n_c0+o+1]) == 1){
                    third_pattern[n_c1] = text[i+n_c0+o+1];
                    n_c1++;
                    continue;
                }

                break;
            }

            if(n_c1 == 0) goto exit_;
            if(text[i+n_c0+n_c1+2] != ')') goto exit_;
            
            //SAVE THE COMPLETE PATTERN
            if(output != NULL){
                output[pattern_count].A = atoi(second_pattern);
                output[pattern_count].B = atoi(third_pattern);
            }

            pattern_count++;
            exit_:;
            p_i = 0; 

        }
    }

    return pattern_count;
}