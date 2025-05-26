#include "main.c"
#include "game.h"


extern x_position;
extern y_position;



extern movement_state;


void game_jump(){
    update_movement_state(UP);
    
}

void game_update() {
/*
    if (is_jumping) {
        dino_y += dino_velocity_y;
        dino_velocity_y += 1;

        if (dino_y >= GROUND_Y) {
            dino_y = GROUND_Y;
            dino_velocity_y = 0;
            is_jumping = false;
        }
    }

*/



}

void game_draw() {
    vg_clear();
    draw_dino();   // Draw a static dino at fixed position
    // draw_cactus(); etc
}

void update_movement_state(movement_states change){
    movement_state = change;

}

void draw_dino()