#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define ADD 0
#define MUL 1
#define CAT 2

typedef struct ecuation{
    uint64_t result;
    int parametes_c;
    int* parameters;
    int is_valid;
} ecuation;

int split_str(char* s, int l, char d, char** output){
    int tokens=0;
    int last_token_index=0;
    for(int i=0; i<l; i++){
        char c=s[i];
        if(c==d){
            if(output != NULL){
                s[i]='\0';
                output[tokens] = s+last_token_index;
                last_token_index = i+1;
            }
            tokens++;
        }
    }

    if(tokens>0){
        if(output != NULL)
            output[tokens] = s+last_token_index;
        tokens++;
    }
    return tokens;
}

int parse_ecuations(char* s, int l, ecuation* out){
    int lines_c = split_str(s,l,'\n',NULL);
    if(out == NULL) return lines_c;
    
    char** lines_p = (char**) malloc(lines_c * sizeof(char**));
    split_str(s,l,'\n',lines_p);

    // for each line
    for(int i=0; i<lines_c; i++){
        char* line = lines_p[i];
        int line_l = strlen(line);
        
        int tokens_c = split_str(line,line_l,':',NULL);
        char* tokens[tokens_c];
        split_str(line,line_l,':',(char**) tokens);

        (out+i)->result = atoll(tokens[0]);

        char* parameters_str = tokens[1]+1;
        int parametes_str_l = line_l-strlen(tokens[0])-2;
        (out+i)->parametes_c = split_str(parameters_str, parametes_str_l, ' ',NULL);
        (out+i)->parameters = (int*) malloc((out+i)->parametes_c * sizeof(int));
        char * parameters[(out+i)->parametes_c];
        split_str(parameters_str, parametes_str_l, ' ',(char**) parameters);

        // for each parameter in ecuation
        for(int o=0; o<(out+i)->parametes_c; o++){
            (out+i)->parameters[o] = atoi(parameters[o]);
        }

        (out+i)->is_valid = 0;
    }

    free((void*) lines_p);
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

void ecuation_print(ecuation* e){
    printf("%lld -> ",e->result);
    for(int i=0; i<e->parametes_c; i++){
        printf("[%i] ",e->parameters[i]);
    }
    printf("\n");
}

int custom_pow(int A, int B){
    int result=1;
    for(int i=0; i<B; i++){
        result =result*A;
    }
    return result;
}

uint64_t concatenate(uint64_t x, uint64_t y) {
    uint64_t pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}

void next_op(int* op, int b, int call, int max_call){
    *op+=1;
    if(*op >= b){
        *op=0;
        if(call < max_call)
            next_op(op+1, b, call+1, max_call);
    }
}

int main(int argsc, char** args){
    if(argsc != 3 || (strcmp("first_part",args[2]) != 0 && strcmp("second_part",args[2]) != 0)){
        printf("usage: day7 [input file path] [ first_part | second_part ]\n");
        return 1;
    }

    char* input_file_path = args[1];
    char* puzzle_part = args[2];
    int pot = strcmp("second_part",puzzle_part) == 0 ? 3 : 2;

    char* input_string;
    int input_string_l = read_input_file(input_file_path,&input_string);
    
    int ecuations_c = parse_ecuations(input_string,input_string_l,NULL);
    ecuation* ecuations = (ecuation*) malloc(ecuations_c * sizeof(ecuation));
    parse_ecuations(input_string,input_string_l,ecuations);

    // for each ecuation
    for(int i=0; i<ecuations_c; i++){
        ecuation* e = ecuations+i;

        int op_c = e->parametes_c-1;
        int op[op_c];
        for(int o=0; o<op_c; o++) op[o]=0;

        
        // for each possibility
        int possibilities=custom_pow(pot,op_c);
        for(int p=0; p<possibilities; p++){
            next_op(op,pot,0,op_c-1);
            uint64_t result = e->parameters[0];

            // for each operator
            for(int b=0; b<op_c; b++){
                int operator = op[b];
                if(operator == ADD) result += e->parameters[b+1];
                if(operator == MUL) result *= e->parameters[b+1];
                if(operator == CAT) result = concatenate(result,e->parameters[b+1]);

            }
            
            if(result == e->result) e->is_valid = 1;
        }

        free((void*) e->parameters);
    }


    uint64_t calibration_value = 0;
    for(int i=0; i<ecuations_c; i++){
        ecuation* e = ecuations+i;
        if(e->is_valid) calibration_value += e->result;
    }

    printf("CALIBRATION VALUE = %lld\n",calibration_value);
    free((void*) input_string);
    free((void*) ecuations);
    return 0;
}