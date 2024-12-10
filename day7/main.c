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

int convert_to_pot(int n, int p){
    int result=0;
    for(int i=0; 1; i++){
        int position=1;
        for(int p=0; p<i; p++) position *= 10;

        int r = n % p;
        result += r*position;

        n /= p;
        if(n <= 0) break;
    }

    return result;
}

int get_digit(int n, int b, int p){
    int ogn = n;
    int og = (n>>b)&1;
    
    n = convert_to_pot(n,p);
    int a=n;
    for(int i=0;i<=b;i++) a /= 10;
    for(int i=0;i<=b;i++) a *= 10;
    n = n-a;
    for(int i=0;i<b;i++) n /= 10;

    if(og != n){
        printf("%i->%i  %i  %i\n",ogn, b, og, n);
    }

    return n;
}

uint64_t concatenate(uint64_t x, uint64_t y) {
    uint64_t pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}

int main(int argsc, char** args){
    if(argsc < 2 || argsc > 3) return 1;
    char* input_file_path = args[1];
    char* puzzle_part = argsc > 2 ? args[2] : "first";
    int pot = strcmp("second",puzzle_part) == 0 ? 3 : 2;

    char* input_string;
    int input_string_l = read_input_file(input_file_path,&input_string);
    
    int ecuations_c = parse_ecuations(input_string,input_string_l,NULL);
    ecuation* ecuations = (ecuation*) malloc(ecuations_c * sizeof(ecuation));
    parse_ecuations(input_string,input_string_l,ecuations);

    // for each ecuation
    for(int i=0; i<ecuations_c; i++){
        ecuation* e = ecuations+i;
        int max_count = custom_pow(pot,e->parametes_c-1);

        // for each operator ( represented as binary/ternary )
        for(int c=0; c<max_count; c++){
            uint64_t result = e->parameters[0];
            for(int b=0; b<e->parametes_c-1; b++){
                int operator = get_digit(c,b,pot);
                if(operator == ADD) result += e->parameters[b+1];
                if(operator == MUL) result *= e->parameters[b+1];
                //if(operator == CAT) result = concatenate(result,e->parameters[b+1]);

            }
            
            if(result == e->result) e->is_valid = 1;
        }
    }

    uint64_t calibration_value = 0;
    for(int i=0; i<ecuations_c; i++){
        ecuation* e = ecuations+i;
        if(e->is_valid){        
            calibration_value += e->result;
            //ecuation_print(ecuations+i);
        }
    }

    printf("CALIBRATION VALUE = %lld\n",calibration_value);
    free((void*) input_string);
    return 0;
}