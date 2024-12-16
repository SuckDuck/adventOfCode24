#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "utils.h"

typedef struct stone{
    uint64_t value;
    uint64_t weight;
} stone;

int get_rocks_array(char* s, int l, stone* out){
    int rocks_c = split_str(s,l,' ',NULL);
    
    if(out != NULL){
        char* tokens[rocks_c];
        split_str(s,l,' ',(char**) &tokens);

        for(int i=0; i<rocks_c; i++){
            out[i].value =  atoi(tokens[i]);
            out[i].weight = 1;
        }
    }
    
    return  rocks_c;
}

void divide_stone(stone** r, int *l, int *buffer_len, int index, int digits){
    *l = l[0]+1;
    if(*l > *buffer_len){
        *buffer_len = *l;
        *r = realloc(*r,l[0] * sizeof(stone));
    }
    
    uint64_t pow = custom_pow(10,digits/2);
    uint64_t higher_half = (*r)[index].value/pow;
    uint64_t lower_half = (*r)[index].value - higher_half*pow;
    (*r)[index].value = higher_half;
    (*r)[l[0]-1].value = lower_half;
    (*r)[l[0]-1].weight = (*r)[index].weight;

}

void print_stones(stone* r, int l){
    for(int i=0; i<l; i++){
        printf("[%llu:%llu] ",r[i].value,r[i].weight);
    }
    printf("\n");
}

void reduction_phase(stone* stones, int* count){
    for(int i=0; i<(*count)-1; i++){ // first loop
        stone* s0 = stones+i;

        for(int o=i+1; o<*count; o++){ // second loop
            stone* s1 = stones+o;

            if(s0->value == s1->value){
                stone* last_stone = stones + count[0]-1;

                s0->weight += s1->weight;
                s1->value = last_stone->value;
                s1->weight = last_stone->weight;
                *count -= 1;

                break;
            }
        }
    }
}

void generation_phase(stone** stones, int* count, int* buffer_len){
    uint64_t tmp_c = *count;
    for(int i=0;  i<tmp_c; i++){
        stone* s = (*stones)+i;
        if(s->value == 0){
            s->value = 1;
            continue;
        }

        int stone_d = get_digits_count(s->value);
        if(stone_d % 2 == 0){
            divide_stone(stones,count,buffer_len,i,stone_d);
            continue;
        }

        s->value *= 2024;
    }
}

uint64_t get_total_weight(stone* stones, uint64_t count){
    uint64_t total=0;
    for(int i=0; i<count; i++){
        stone* s = stones+i;
        total += s->weight;
    }

    return total;
}

int main(int argsc, char** args){
    #pragma region // ------------------- ARGS PARSE AND INPUT DATA READ ------------------- //
    if(argsc != 3){
        printf("usage: day11 [input file path] [blinks]\n");
        return 1;
    }
    char* input_file_path = args[1];
    int blinks = atof(args[2]);

    char* input_str;
    int input_len = read_input_file(input_file_path,&input_str);
    #pragma endregion

    int stones_c = get_rocks_array(input_str,input_len,NULL);
    int stones_buffer_len = stones_c;

    
    stone* stones  = (stone*) malloc(stones_c * sizeof(stone));
    get_rocks_array(input_str,input_len,stones);

    // for each blink
    for(int i=0; i<blinks; i++){
        reduction_phase(stones,&stones_c);
        generation_phase(&stones,&stones_c,&stones_buffer_len);

    }

    printf("Number of stones: %llu\n",get_total_weight(stones,stones_c));
    free(stones);
    free(input_str);
    return 0;
}