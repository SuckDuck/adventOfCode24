#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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

int get_full_disk_size(char* s, int l){
    int size=0;
    for(int i=0; i<l; i++){
        char c[] = {s[i],'\0'};
        size += atoi(c);
    }
    return size;
}

void print_full_disk(int* p, int l){
    for(int i=0; i<l; i++){
        printf("%c",p[i] == -1 ? '.':p[i]+'0');
    }
    printf("\n");
}

int char_to_int(char c){
    switch(c){
        case '0': return 0;
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
        case '4': return 4;
        case '5': return 5;
        case '6': return 6;
        case '7': return 7;
        case '8': return 8;
        case '9': return 9;
        default:  return -1;
    }
}

int get_free_space(int* p,int* end){
    if(p == end) return 0;

    int c = 0;
    if(*p == -1){
        c += (1 + get_free_space(p+1,end));
    }

    return c;

}

int main(int argsc, char** args){
    if(argsc != 3 || (strcmp("first_part",args[2]) != 0 && strcmp("second_part",args[2]) != 0)){
        printf("usage: day9 [input file path] [ first_part | second_part ]\n");
        return 1;
    }
    char* input_file_path = args[1];
    int puzzle_part = strcmp("second_part",args[2]) == 0 ? 2 : 1;

    char* input_string;
    int input_len = read_input_file(input_file_path,&input_string);

    int disk_len = get_full_disk_size(input_string,input_len);
    int disk[disk_len];
    for(int i=0; i<disk_len; i++) disk[i] = -1;

    
    // for each number in the input file
    int is_used = 1;
    int block_c = 0;
    int block_id = 0;
    for(int i=0; i<input_len; i++){
        char c[] =  {input_string[i],'\0'};
        int cn = atoi(c);
        
        // for the time that number says
        for(int o=0; o<cn; o++){
            disk[block_c] = is_used == 1? block_id:-1;
            block_c++;
        }

        is_used = 1 - is_used;
        if(is_used == 0){
            block_id++;
        }
    }

    // for each number in the disk array (backwards)
    for(int i=disk_len-1; i>=0; i--){

        int block = disk[i];
        int block_size = char_to_int(input_string[block*2]);

        if(block != -1){
            if(puzzle_part == 2) i -= (block_size-1);

            // for each number in the disk array again (forward)
            for(int o=0; o<disk_len; o++){
                int free_space = disk[o];
                if(o >= i) break;

                if(free_space == -1){
                    int free_space_size = get_free_space(disk+o,disk+(disk_len-1));
                    if(puzzle_part == 2) o+=free_space_size;
                    
                    if(puzzle_part == 2){
                        if(free_space_size >= block_size){
                            for(int fs=0; fs<block_size; fs++){
                                *(disk+(o-free_space_size)+fs) = block;
                                *(disk+i+fs) = -1;
                            }

                            break;
                        }
                    }

                    else{
                        disk[i] = -1;
                        disk[o] = block;
                        break;
                    }
                }
            }
        }
    }
    

    // for each number in the disk array
    uint64_t result = 0;
    for(int i=0; i<disk_len; i++){
        int c=disk[i];
        if(c <= -1) continue;
        result += i*c;
    }

    printf("RESULT: %llu\n",result);
    free(input_string);
    return 0;
}