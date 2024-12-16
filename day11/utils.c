#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

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

uint64_t custom_pow(uint64_t A, uint64_t B){
    uint64_t result=1;
    for(int i=0; i<B; i++){
        result =result*A;
    }
    return result;
}

int get_digits_count(uint64_t n){
    int digits=1;
    for(uint64_t i=10; 1; i *= 10){
        if(n < i) return digits;
        digits+=1;
    }
    
}