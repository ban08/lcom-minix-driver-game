#include "main.c"
#include "game.h"


extern x_position;
extern y_position;



extern movement_state;




void game_update() {
    // TODO: move cactus, apply gravity, etc
}

void game_draw() {
    vg_clear();
    draw_dino();   // Draw a static dino at fixed position
    // draw_cactus(); etc
}

void update_movement_state(){
    if (y_position = 10)


}