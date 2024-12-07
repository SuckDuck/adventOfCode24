
#define INPUT_FILE_PATH "_input"

typedef int level_t;
typedef struct report{
    int len;
    level_t* levels;
    int is_valid;
} report;

void print_report(report* r, int len);
int get_reports(char* path,report* reports);
void free_reports(report* r, int len);
void validate_report(report* r, int ignore_index);