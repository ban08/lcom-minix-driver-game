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

extern int menu;

/// @brief Flag indicating whether the game has started.
int has_started = 0;

/// @brief Sprite representing the start state.
Sprite* start_sprite = NULL;

int janela_min = 0;
int janela_max = 800;
extern int first_screen_appearance;

/// @brief X-coordinates for enemy objects
int enemy_x_coordinates[] = {700, 1200, 1400, 1900, 2900, 3100, 3500, 3700};


/**
 * @brief Starts and runs one frame of the game loop.
 *
 * Initializes the background, draws the player and enemies,
 * and handles vertical movement and collision.
 */
void game_start(){

    if (menu == 1){

        if (first_screen_appearance == 1){
            vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00000000);
            draw_menu_screen();
            first_screen_appearance = 0;
            printf("here");
            printf("%d", menu);

        }
        
        


    } else {
        printf("there");
        
    if (first_screen_appearance == 1){
        vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00000000);
        has_started = 0;
        first_screen_appearance = 0;

    }


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

void draw_menu_screen() {
    //vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, 0x00000000);

    // Draw a basic "MENU" blocky letters using rectangles
    // Letter M
    vg_draw_rectangle(490, 100, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(480, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(500, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(510, 100, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(520, 100, 10, 50, 0xFFFFFF00);
    
    // Letter E
    vg_draw_rectangle(540, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(550, 100, 20, 10, 0xFFFFFF00);
    vg_draw_rectangle(550, 125, 15, 10, 0xFFFFFF00);
    vg_draw_rectangle(550, 140, 20, 10, 0xFFFFFF00);
    
    // Letter N
    vg_draw_rectangle(580, 100, 10, 50, 0xFFFFFF00);
    vg_draw_rectangle(590, 110, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(600, 120, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(610, 130, 10, 10, 0xFFFFFF00);
    vg_draw_rectangle(620, 100, 10, 50, 0xFFFFFF00);
    
    // Letter U
    vg_draw_rectangle(640, 100, 10, 40, 0xFFFFFF00);
    vg_draw_rectangle(670, 100, 10, 40, 0xFFFFFF00);
    vg_draw_rectangle(650, 140, 20, 10, 0xFFFFFF00);
    

//DINO text

    int start_x = 100;
    int start_y = 100;
    int letter_w = 20;
    int letter_h = 50;
    int gap = 15;

    // Letter D
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00); // left
    vg_draw_rectangle(start_x + 5, start_y, 10, 5, 0xFFFFFF00);   // top
    vg_draw_rectangle(start_x + 15, start_y + 5, 5, 40, 0xFFFFFF00); // curve side
    vg_draw_rectangle(start_x + 5, start_y + 45, 10, 5, 0xFFFFFF00); // bottom

    // Letter I
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x + 5, start_y, 10, letter_h, 0xFFFFFF00);

    // Letter N
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 5, start_y + 10, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 10, start_y + 20, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 15, start_y + 30, 5, 5, 0xFFFFFF00);
    vg_draw_rectangle(start_x + 20, start_y, 5, letter_h, 0xFFFFFF00);

    // Letter O
    start_x += letter_w + gap;
    vg_draw_rectangle(start_x, start_y, 5, letter_h, 0xFFFFFF00);         // left
    vg_draw_rectangle(start_x + 20, start_y, 5, letter_h, 0xFFFFFF00);    // right
    vg_draw_rectangle(start_x + 5, start_y, 15, 5, 0xFFFFFF00);           // top
    vg_draw_rectangle(start_x + 5, start_y + 45, 15, 5, 0xFFFFFF00);      // bottom




    //palavra play
    int square_x = 100;
    int square_y = 400;
    int square_size = 30;

    // Draw green square
    vg_draw_rectangle(square_x, square_y, square_size, square_size, 0xFF00FF00);

// Draw "S" inside the square (block style)
// Top bar
vg_draw_rectangle(square_x + 5, square_y + 5, 20, 5, 0xFF000000);
// Upper vertical (left)
vg_draw_rectangle(square_x + 5, square_y + 5, 5, 10, 0xFF000000);
// Middle bar
vg_draw_rectangle(square_x + 5, square_y + 15, 20, 5, 0xFF000000);
// Lower vertical (right)
vg_draw_rectangle(square_x + 20, square_y + 15, 5, 10, 0xFF000000);
// Bottom bar
vg_draw_rectangle(square_x + 5, square_y + 25, 20, 5, 0xFF000000);

    // Draw "Play" text to the right (small block letters)
    int text_x = square_x + square_size + 10;
    int text_y = square_y + 5;

    // Letter P
    vg_draw_rectangle(text_x, text_y, 2, 10, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 4, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y + 2, 2, 2, 0xFFFFFFFF);

    // Letter L
    text_x += 12;
    vg_draw_rectangle(text_x, text_y, 2, 10, 0xFFFFFFFF);
    vg_draw_rectangle(text_x, text_y + 8, 6, 2, 0xFFFFFFFF);

    // Letter A
    text_x += 10;
    vg_draw_rectangle(text_x, text_y + 2, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y + 2, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 4, 4, 2, 0xFFFFFFFF);

    // Letter Y
    text_x += 12;
    vg_draw_rectangle(text_x, text_y, 2, 4, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 6, text_y, 2, 4, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 3, text_y + 4, 2, 6, 0xFFFFFFFF);



    //exit

    square_x = 100;
    square_y = 440;
    int square_width = 40;
    int square_height = 20;

    // Draw red rectangle
    vg_draw_rectangle(square_x, square_y, square_width, square_height, 0xFFFF0000);

    // Draw "Esc" inside (blocky approximation)
    int esc_x = square_x + 4;
    int esc_y = square_y + 4;

    // E
    vg_draw_rectangle(esc_x, esc_y, 2, 10, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 4, 4, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 8, 6, 2, 0xFF000000);

    // S
    esc_x += 10;
    vg_draw_rectangle(esc_x, esc_y, 8, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 4, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 8, 8, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 2, 2, 2, 0xFF000000);
    vg_draw_rectangle(esc_x + 6, esc_y + 6, 2, 2, 0xFF000000);

    // C
    esc_x += 10;
    vg_draw_rectangle(esc_x + 2, esc_y, 6, 2, 0xFF000000);
    vg_draw_rectangle(esc_x, esc_y + 2, 2, 6, 0xFF000000);
    vg_draw_rectangle(esc_x + 2, esc_y + 8, 6, 2, 0xFF000000);

    // Draw "Exit" to the right
    text_x = square_x + square_width + 10;
    text_y = square_y + 4;

    // E
    vg_draw_rectangle(text_x, text_y, 2, 8, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y, 4, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 3, 3, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 6, 4, 2, 0xFFFFFFFF);

    // X
    text_x += 8;
    vg_draw_rectangle(text_x, text_y, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 4, text_y, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 2, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x, text_y + 4, 2, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 4, text_y + 4, 2, 2, 0xFFFFFFFF);

    // I
    text_x += 8;
    vg_draw_rectangle(text_x + 2, text_y, 2, 8, 0xFFFFFFFF);

    // T
    text_x += 6;
    vg_draw_rectangle(text_x, text_y, 6, 2, 0xFFFFFFFF);
    vg_draw_rectangle(text_x + 2, text_y + 2, 2, 6, 0xFFFFFFFF);

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
