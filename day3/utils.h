#include <stdio.h>

typedef struct mul_pattern{
    int A, B;
} mul_pattern;

int is_it_a_number(char n);
size_t read_input_file(char* path, char** output);
int get_mul_patterns(char* text, size_t text_len, mul_pattern* output, int enable_do_control);
