#ifndef utils
#define utils
#include <stdint.h>

int split_str(char* s, int l, char d, char** output);
int read_input_file(char* path, char** output);
uint64_t custom_pow(uint64_t A, uint64_t B);
int get_digits_count(uint64_t n);

#endif