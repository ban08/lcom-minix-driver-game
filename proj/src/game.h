#ifndef _GAME_H_
#define _GAME_H_


#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

typedef enum {
    UP,
    DOWN,
    BASE
} movement_states;



typedef struct
{
    int x_1;
    int x_2;
    int y_1;
    int y_2;
} to_delete;


void game_jump();
void game_update();
void game_start();
void game_draw();
void update_movement_state(movement_states change);
int draw_dino(xpm_map_t xpm, uint16_t x, uint16_t y);
void draw_enemies();



#endif
