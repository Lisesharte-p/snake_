#ifndef GAME_HPP
#define GAME_HPP
#include<iostream>

class snake_t{
public:
    unsigned int tail_row;
    unsigned int tail_col;
    unsigned int head_row;
    unsigned int head_col;

    bool live;


    
};

class game_t{
public:
  unsigned int num_rows;
  unsigned int num_columns;
  char **board;

  unsigned int num_snakes;
  snake_t *snakes;
  game_t(int rows,int columns);
  game_t(const game_t& other); 
  game_t();
  ~game_t();
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


static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);

game_t *initialize_snakes(game_t *game);
game_t *load_board(FILE *fp);
game_t *create_default_game(int rows, int columns);
#endif