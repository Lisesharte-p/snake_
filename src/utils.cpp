#include "utils.hpp"
#include "game.hpp"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include<string>
unsigned int det_rand(unsigned int *game) {
  if (*game == 0) {
    *game = 1;
  }
  if (*game & 1) {
    *game = *game >> 1 ^ 0x80000057;
  } else {
    *game = *game >> 1;
  }
  return *game;
}

unsigned int seed = 1;

unsigned int get_num_cols(game_t *game, unsigned int row) {
  unsigned int num_cols = (unsigned int)strlen(game->board[row]);
  while (num_cols > 0 && game->board[row][num_cols - 1] == '\n') {
    num_cols--;
  }
  return num_cols;
}

int deterministic_food(game_t *game) {
  unsigned int row = det_rand(&seed) % game->num_rows;
  unsigned int col = det_rand(&seed) % get_num_cols(game, row);

  while (game->board[row][col] != ' ') {
    row = det_rand(&seed) % game->num_rows;
    col = det_rand(&seed) % get_num_cols(game, row);
  }
  game->board[row][col] = '*';

  return 1;
}

int corner_food(game_t *game) {
  game->board[1][1] = '*';
  return 1;
}

void redirect_snake(game_t *game, char input_direction) {
  unsigned int row;
  unsigned int col;
  row = game->snakes->head_row;
  col = game->snakes->head_col;

  if (game->snakes->live) {

  

  if (input_direction == 'w') {
    game->board[row][col] = 'W';
  } else if (input_direction == 'a') {
    game->board[row][col] = 'A';
  } else if (input_direction == 's') {
    game->board[row][col] = 'S';
  } else if (input_direction == 'd') {
    game->board[row][col] = 'D';
  }}
  if(game->num_snakes>=2)
{  row = (game->snakes+1)->head_row;
  col = (game->snakes+1)->head_col;
  if (!(game->snakes+1)->live) {
    return;
  }
    if (input_direction == 'i') {
    game->board[row][col] = 'W';
  } else if (input_direction == 'j') {
    game->board[row][col] = 'A';
  } else if (input_direction == 'k') {
    game->board[row][col] = 'S';
  } else if (input_direction == 'l') {
    game->board[row][col] = 'D';
  }}
}

unsigned int snake_seed = 1;
bool is_snake1(char c) {
  const char* snk="wasd^<v>WASDx";
  for (int i=0;i<12;i++) {
    if (*(snk+i)==c) {
      return true;
    }
  }
  return false;
}
void random_turn(game_t *game, int snum) {
  snake_t *snake = &(game->snakes[snum]);
  char cur_head = game->board[snake->head_row][snake->head_col];
  if((is_snake1(game->next_square(snum))||game->next_square(snum)=='#')){
    std::string heads = "WASD";
    int i;
    for (i = 0; i < 4; ++i) {
      if (heads[i] == cur_head)
        break;
    }
    if (det_rand(&snake_seed) % 2 == 0) {
      i += 1;
    } else {
      i -= 1;
    }
    i = i % 4;
    i=abs(i);
    game->board[snake->head_row][snake->head_col] = heads[i];
  }

}