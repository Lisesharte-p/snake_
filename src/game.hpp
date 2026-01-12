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
  game_t(int rows,int columns);
  char get_board_at(unsigned int row, unsigned int col);
  void set_board_at(unsigned int row, unsigned int col, char ch);
  void print_board( FILE *fp);
  void save_board( char *filename);
  char next_square(unsigned int snum);
  void update_tail(unsigned int snum);
  void update_head(unsigned int snum);
  void update_game(int (*add_food)(game_t *game));
  void find_head(unsigned int snum);
};



game_t *initialize_snakes(game_t *game);
game_t *load_board(FILE *fp);