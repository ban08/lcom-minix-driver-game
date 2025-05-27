#ifndef VIDEO_CARD_H
#define VIDEO_CARD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <lcom/lab5.h>

extern vbe_mode_info_t mode_info;
extern uint8_t* frame_buffer;
//int vg_exit();
//int vbe_get_mode_info(uint16_t mode, vbe_mode_info_t *vmi_p);
int (set_graphic_mode)(uint16_t s_mode);
int (set_text_mode)();
int (set_frame_buffer)(uint16_t mode);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);
int normalize_color(uint32_t color, uint32_t *new_color);
uint32_t get_direct_color(uint32_t red, uint32_t green, uint32_t blue);
uint32_t get_indexed_color(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
uint32_t get_red_component(unsigned col_index, uint8_t step, uint32_t first_color);
uint32_t get_green_component(unsigned row_index, uint8_t step, uint32_t first_color);
uint32_t get_blue_component(unsigned col_index, unsigned row_index, uint8_t step, uint32_t first_color);
uint32_t extract_red(uint32_t color);
uint32_t extract_green(uint32_t color);
uint32_t extract_blue(uint32_t color);

#endif
