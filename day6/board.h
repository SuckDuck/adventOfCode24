#ifndef board_h
#define board_h

#define ORIENTATION_CHARS "^>v<"

typedef int ORIENTATION_t;

enum ORIENTATION {
    ORIENTATION_UP,
    ORIENTATION_RIGHT,
    ORIENTATION_DOWN,
    ORIENTATION_LEFT
};

typedef struct square{
    int x,y;
    int value, obstacle;
} square;

struct guard{
    int pos_x, pos_y;
    ORIENTATION_t orientation;
    int total_dissplacement;
};

typedef struct board{
    int cols, rows;
    square* squares;
    struct guard guard;
} board;

square* board_get_square(board* b, int x, int y);
void board_set_dimensions(board* b, char* s, int sl);
void board_set_elements(board* b, char* s, int sl);
void board_print(board* b);
void board_free(board* b);
void board_init(board* b, char* s, int sl);
int guard_step(board *b);
int read_input_file(char* path, char** output);

#endif