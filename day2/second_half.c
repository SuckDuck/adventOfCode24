#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include "utils.h"

int main(int argsc, char** args){
    int reports_len = get_reports(INPUT_FILE_PATH,NULL);
    report reports[reports_len];
    get_reports(INPUT_FILE_PATH,reports);

    int valid_reports = 0;
    for(int i=0; i<reports_len; i++){
        for(int o=0; o<reports[i].len; o++){
            validate_report(reports+i,o);
            if(reports[i].is_valid) break;
        }

        valid_reports += reports[i].is_valid;
    }

    print_report(reports,reports_len);
    free_reports(reports,reports_len);
    printf("%i\n",valid_reports);
    return 0;
}