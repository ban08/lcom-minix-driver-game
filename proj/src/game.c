//#include "main.c"
#include "game.h"
//#include "sprite.h"
#include "video_card.h"
#include "utils.c"

extern int x_position;
extern int y_position;


extern movement_states movement_state;


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
    //vg_clear();
static const char *dino[] = {
  "15 15 3 1",
  "  c None",
  ". c black",
  "X c #EAD012",
  "       ....... ",
  "      ...X.... ",
  "      ........ ",
  "      ....     ",
  "      .......  ",
  "     .....     ",
  " .  ......     ",
  " ...........   ",
  " ......... .   ",
  " .........     ",
  "  .......      ",
  "   .....       ",
  "    .  .       ",
  "    .  ..      ",
  "    ..         "
};

    
        draw_dino((xpm_map_t)dino, 100, 500);

}

void update_movement_state(movement_states change){
    movement_state = change;

}

int draw_dino(xpm_map_t xpm, uint16_t x, uint16_t y){
  if (//set_frame_buffer(0x105) == 0 &&
  //set_graphic_mode(0x105) == 0 &&
  print_xpm(xpm, x, y) == 0 //Y&&
  //was_ESC_pressed() == 0 &&
  //vg_exit() == 0) 
  )
  {
    return 0;
  }

  return 1;

}
