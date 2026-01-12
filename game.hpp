#include<iostream>

class snake_t{
public:
    unsigned int tail_row;
    unsigned int tail_col;
    unsigned int head_row;
    unsigned int head_col;

    bool live;

    static char body_to_tail(char c);
    static char head_to_body(char c);
    static void update_head(game_t *game, unsigned int snum);
};

class game_t{
public:
  unsigned int num_rows;
  unsigned int num_columns;
  char **board;

  unsigned int num_snakes;
  snake_t *snakes;
  char get_board_at(game_t *game, unsigned int row, unsigned int col);
};
game_t *create_default_game(int rows,int columns);
void free_game(game_t *game);
void print_board(game_t *game, FILE *fp);
void save_board(game_t *game, char *filename);
void update_game(game_t *game, int (*add_food)(game_t *game));

game_t *initialize_snakes(game_t *game);
game_t *load_board(FILE *fp);