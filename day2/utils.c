#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"

void print_report(report* r, int len){
    for(int i=0; i<len; i++){
        printf("REPORT #%i:  LEN->%i  VALUES:",i,r[i].len);
        for(int o=0; o<r[i].len; o++){
            printf("%i ",r[i].levels[o]);
        }
        printf(" IS_VALID->%i\n",r[i].is_valid);
    }
}

int get_reports(char* path,report* reports){
    int rs;

    #pragma region // ------------------- READ_FILE ------------------- //
    struct stat fs;
    rs = stat(path,&fs);
    int fd = open(path, O_RDONLY);
    
    size_t bytes_wrote = 0;
    char file_content[fs.st_size+1];
    file_content[fs.st_size] = '\0';
    while(bytes_wrote < fs.st_size){
        bytes_wrote += read(fd,(void*) (file_content+bytes_wrote),fs.st_size);
    }
    close(fd);
    #pragma endregion

    #pragma region // ---------------- GET_REPORTS_LEN ---------------- //
    int reports_len = 1;
    for(int i=0; i<fs.st_size; i++){
        char c = file_content[i];        
        if(c == '\n'){
            reports_len ++;
        }
    }
    #pragma endregion

    #pragma region // ------------------ GET_REPORTS ------------------ //
    if(reports != NULL){
        //INITIALIZE EVERY REPORT
        for(int i=0; i<reports_len+1; i++){
            reports[i].len = 0;
            reports[i].is_valid = 0;
        }

        //GET EVERY REPORT LEN
        int current_report = 0;
        for(int i=0; i<fs.st_size; i++){
            char c = file_content[i];
            if(c==' ' || c=='\n' || i==fs.st_size-1){
                reports[current_report].len++;
            }

            if(c=='\n'){
                current_report++;
            }
        }

        //SAVE THE REPORT LEVELS
        for(int i=0; i<reports_len; i++){
            reports[i].levels = (level_t*) malloc(reports[i].len * sizeof(int));
        }

        int n_start = 0;
        int level_index = 0;
        current_report = 0;
        char r[1024];
        for(int i=0; i<fs.st_size; i++){
            char c = file_content[i];
            if(c==' ' || c=='\n' || i==fs.st_size-1){
                memset(r,'\0',1024);
                memcpy(r,file_content+n_start,i-n_start);
                reports[current_report].levels[level_index] = atoi(r);
                n_start=i;
                level_index++;
            }

            if(c=='\n'){
                current_report++;
                level_index = 0;
            }
        }

        reports[current_report].levels[reports[current_report].len-1] = atoi(file_content+(n_start-1));
    }
    #pragma endregion

    return reports_len;    
}

void free_reports(report* r, int len){
    for(int i=0; i<len; i++){
        free((void*) (r+i)->levels);
    }
}

void validate_report(report* r, int ignore_index){
    int decreasing = 0;
    int offset0 = ignore_index == 0 ? 1:0;
    int offset1 = (ignore_index == 1 ? 1:0) + offset0;
    if(r->levels[1+offset1] < r->levels[0+offset0]){
        decreasing = 1;
    }

    for(int i=1+offset0; i<r->len; i++){
        if(i == ignore_index) continue;
        int previous_level = i-1;
        if(previous_level == ignore_index) previous_level -= 1;

        int difference = abs(r->levels[i] - r->levels[previous_level]);
        if( difference < 0 || difference > 3 ) return;
        if( r->levels[i] == r->levels[previous_level] ) return;
        if( decreasing==1 && (r->levels[i] > r->levels[previous_level]) ) return;
        if( decreasing==0 && (r->levels[i] < r->levels[previous_level]) ) return;
    }

    r->is_valid = 1;
}