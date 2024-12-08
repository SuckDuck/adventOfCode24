#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define PATTERN "XMAS"
int pattern_len;

int read_input_file(char* path, char* output){
    struct stat st;
    stat(path,&st);
    if(output == NULL){
        return st.st_size / sizeof(char);
    }

    int fd = open(path,O_RDONLY);
    int bytes_read = 0;
    while(bytes_read < st.st_size){
        bytes_read += read(fd,(void*)(output+bytes_read),st.st_size-bytes_read);
    }
}

void get_rows_n_colums(char* input, int input_len, int* rows, int* columns){
    *rows=1;
    *columns=0;
    for(int i=0; i<input_len; i++){
        char c=input[i];
        if(c=='\n') (*rows)++;
        if(*rows == 1) (*columns)++;

    }
}

int main(int argsc, char** args){
    // ARGS PARSING
    if(argsc != 2){
        fprintf(stderr,"Usage: first [input file path]\n");
        return 1;
    }

    // INPUT FILE READING
    char* input_path = *(args+1);
    int input_len = read_input_file(input_path,NULL);
    char input[input_len];
    read_input_file(input_path,input);

    // GETTING COLUMNS AND ROWS
    int rows, columns = 0;
    get_rows_n_colums(input,input_len,&rows,&columns);

    pattern_len = strlen(PATTERN);
    int current_column = 0;
    int current_row = 0;
    int matches = 0;
    for(int i=0; i<input_len; i++){
        #pragma region // -------- LOOP INIT -------- //
        if(input[i] == '\n') continue;
        #pragma endregion
        
        // HORIZONTAL RIGHT
        if(current_column+(pattern_len-1) < columns){
            int score = 0;
            for(int r=0; r<pattern_len; r++){
                if(input[i+r] == PATTERN[r]) score++;
            }

            if(score == pattern_len) matches++;
        }
        
        // HORIZONTAL LEFT
        if(current_column >= pattern_len-1){
            int score = 0;
            for(int r=0; r<pattern_len; r++){
                if(input[i-r] == PATTERN[r]) score++;
            }

            if(score == pattern_len) matches++;
        }

        // VERTICAL DOWN
        if(current_row <= rows-pattern_len){
            int score=0;
            for(int c=0; c<pattern_len; c++){
                if(input[i+(c*(columns+1))] == PATTERN[c]) score++;
            }

            if(score == pattern_len) matches++;
        }

        // VERTICAL UP
        if(current_row >= pattern_len-1){
            int score=0;
            for(int c=0; c<pattern_len; c++){
                if(input[i-(c*(columns+1))] == PATTERN[c]) score++;
            }

            if(score == pattern_len) matches++;
        }

        // DIAGONAL RIGHT DOWN
        if(current_column+(pattern_len-1) < columns){
            if(current_row <= rows-pattern_len){
                int score=0;
                for(int c=0; c<pattern_len; c++){
                    if(input[ i + (c*(columns+1)) + c ] == PATTERN[c]) score++;   
                }

                if(score == pattern_len) matches++;
            }
        }

        // DIAGONAL LEFT DOWN
        if(current_column >= pattern_len-1){
            if(current_row <= rows-pattern_len){
                int score=0;
                for(int c=0; c<pattern_len; c++){
                    if(input[ i + (c*(columns+1)) - c] == PATTERN[c]) score++;   
                }
                
                if(score == pattern_len) matches++;
            }
        }

        // DIAGONAL RIGHT UP
        if(current_column+(pattern_len-1) < columns){
            if(current_row >= pattern_len-1){
                int score=0;
                for(int c=0; c<pattern_len; c++){
                    if(input[ i - (c*(columns+1)) + c ] == PATTERN[c]) score++;
                }

                if(score == pattern_len) matches++;
            }
        }

        // DIAGONAL LEFT UP
        if(current_column >= pattern_len-1){
            if(current_row >= pattern_len-1){
                int score=0;
                for(int c=0; c<pattern_len; c++){
                    if(input[ i - (c*(columns+1)) - c] == PATTERN[c]) score++;   
                }
                
                if(score == pattern_len) matches++;
            }
        }

        #pragma region // -------- LOOP END -------- //
        current_column ++;
        if(current_column >= columns){
            current_column = 0;
            current_row ++;
        }
        #pragma endregion
    }

    printf("%i\n",matches);
    return 0;
}