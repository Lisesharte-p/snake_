#ifndef _SNK_SNAKE_UTILS_H
#define _SNK_SNAKE_UTILS_H

#include "game.hpp"
#include <stdint.h>

/* Define key presses for interactive mode. */
#define KEY_MOVEUP 0x77
#define KEY_MOVERIGHT 0x64
#define KEY_MOVEDOWN 0x73
#define KEY_MOVELEFT 0x61
#define KEY_QUIT 0x71

/* A simple deterministic random function */
unsigned int det_rand(unsigned int *game);

/* Deterministically generates food on the board. */
int deterministic_food(game_t *game);

/* Generates food in the top-left corner of the board. */
int corner_food(game_t *game);

/* Changes the direction of the player-controlled snake. */
void redirect_snake(game_t *game, char input_direction);

/* Randomly causes the chosen snake to turn left or right. */
void random_turn(game_t *game, int snum);

#endif