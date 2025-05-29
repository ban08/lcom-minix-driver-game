//#include "main.c"
#include "game.h"
#include "sprite.h"
#include "video_card.h"
#include "utils.c"

/// @brief Global X coordinate of the player sprite.
extern int x_position;

/// @brief Global Y coordinate of the player sprite.
extern int y_position;

/// @brief Game running flag.
extern bool running;

/// @brief Whether the player is currently going up in a jump.
extern int is_going_up;

/// @brief Represents the current movement state.
extern movement_states movement_state;


/// @brief Flag indicating whether the game has started.
int has_started = 0;

/// @brief Sprite representing the start state.
Sprite* start_sprite = NULL;

int janela_min = 0;
int janela_max = 800;


/// @brief X-coordinates for enemy objects (e.g., spikes).
int enemy_x_coordinates[] = {700, 1200, 1450, 1900, 2900, 3100, 3500, 3700};


/**
 * @brief Starts and runs one frame of the game loop.
 *
 * Initializes the background, draws the player and enemies,
 * and handles vertical movement and collision.
 */
void game_start(){
    if (has_started == 0){
        has_started = 1;
        vg_draw_rectangle(0,0, mode_info.XResolution, 500, 0xFF7DC7C4);
        
        //Sun
        vg_draw_rectangle(620,70, 80, 80, 0xFFE0C25C);
        vg_draw_rectangle(630,80, 60, 60, 0xFFD1A513);
        
        //Cloud
        vg_draw_rectangle(80,80, 450, 140, 0XFF969696);
        vg_draw_rectangle(100,100, 220, 100, 0XFFBABABA);
        vg_draw_rectangle(350,110, 110, 90, 0XFFBABABA);
        
        // Ground
        vg_draw_rectangle(0,500, mode_info.XResolution, 25, 0xFFBD9773);
        vg_draw_rectangle(0,525, mode_info.XResolution, 25, 0xFF9E7E60);
        vg_draw_rectangle(0,550, mode_info.XResolution, 25, 0xFF735C46);
        vg_draw_rectangle(0,575, mode_info.XResolution, 25, 0xFF45372A);

    }
    
    game_jump();

    // Clear old player pixels
    for (int y = 325; y < 500; y++) {
        for (int x = 40; x < 70; x++) {
            vg_draw_pixel(x, y, 0xFF7DC7C4);
        }
    }

    // Clear bottom line
    for (int y = 488; y < 500; y++) {
        for (int x = 0; x < mode_info.XResolution; x++) {
            vg_draw_pixel(x, y, 0xFF7DC7C4);
        }
    }

    // Draw player character
    vg_draw_rectangle(x_position, y_position, 30, 75, 0XFF105902);
    //vg_draw_hline(0, 500, 800, 0xFFFF0000);

    // Draw obstacles
    draw_enemies();

    janela_min = janela_min + 5;
    janela_max = janela_max + 5;

    if (janela_min > 3700){
    janela_min = -200;
    janela_max = 600;
}

    usleep(16000); // aprox 60 FPS --> parece me ser o sweet spot. + frames, + tearing
}

/**
 * @brief Draws all enemies currently within the visible window.
 *
 * Also handles collision detection with the player.
 */
void draw_enemies(){
    for (unsigned int i = 0; i < 8; i++){
        if ((enemy_x_coordinates[i] > janela_min) && (enemy_x_coordinates[i] < janela_max)){
            vg_draw_rectangle(enemy_x_coordinates[i] - janela_min, 488, 4, 12, 0xFFE37610);
            if ((enemy_x_coordinates[i] - janela_min > 40) && (enemy_x_coordinates[i] - janela_min < 70) && (y_position > 425 - 12)){
                running = false; //se colidir com os espinhos, sai
                printf("You scored %d points!", enemy_x_coordinates[i]);
            }
        }
    }

}


/**
 * @brief Calculates which pixels need to be deleted/redrawn between frames.
 *
 * @param new_x New X coordinate.
 * @param old_x Old X coordinate.
 * @param new_y New Y coordinate.
 * @param old_y Old Y coordinate.
 * @param type Object type: 0 for player, 1 for enemy.
 * @return Structure with coordinates to delete.
 */
//afinal não é preciso. basat estimar os pontos em que a imagem pode mudar, nao precisamos de os calcular
to_delete calculate_to_delete (int new_x, int old_x, int new_y, int old_y, int type){
    //type 0 -> dino
    //type 1 -> cacto
    //largura dino -> 30, altura -> 75
    to_delete output;

    if (type == 1){
        output.x_1 = 40;
        output.x_2 = 70;

        if (new_y < old_y){
            output.y_1 = old_y;
            output.y_2 = new_y;
        }
    }
    return output;
}

/**
 * @brief Handles player vertical movement (jumping and falling).
 */
void game_jump(){
    if (is_going_up && y_position > 325){
        y_position = y_position - 5;
    }
    else if (is_going_up && y_position <= 325){
        is_going_up = 0;
    }
    else if (!is_going_up && y_position < 425){
        y_position = y_position + 5;
    }
}
