#ifndef _GAME_H_
#define _GAME_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

/// @brief Enum representing the player's vertical movement state.
typedef enum {
    UP,   ///< Player is jumping up.
    DOWN, ///< Player is falling down.
    BASE  ///< Player is on the ground.
} movement_states;


/// @brief Struct representing an area of the screen to be cleared/redrawn.
typedef struct {
    int x_1; ///< Start X coordinate.
    int x_2; ///< End X coordinate.
    int y_1; ///< Start Y coordinate.
    int y_2; ///< End Y coordinate.
} to_delete;


/**
 * @brief Handles the vertical jump logic for the player.
 */
void game_jump();
/**
 * @brief Updates game state (movement, timing, etc.).
 *
 * @note This may be used inside a timer interrupt handler or main loop.
 */
void game_update();

/**
 * @brief Starts the game and draws the initial scene.
 *
 * This function initializes the game state and background for the first time.
 */
void game_start();

/**
 * @brief Redraws the entire frame, including player, enemies, and background.
 */
void game_draw();

/**
 * @brief Changes the current movement state of the player.
 * 
 * @param change New movement state to set.
 */
void update_movement_state(movement_states change);

/**
 * @brief Draws the player character (dinosaur) at the specified position.
 *
 * @param xpm XPM image to draw.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @return 0 on success, non-zero on error.
 */
int draw_dino(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Draws all active enemies currently visible on screen.
 *
 * Also handles collision detection with the player.
 */
void draw_enemies();



void draw_menu_screen();

#endif
