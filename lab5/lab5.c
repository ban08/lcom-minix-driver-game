// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (vg_init(mode) == NULL) { // Initialize video mode
    printf("Failed to set video mode 0x%03x\n", mode);
    return 1;
  }

  tickdelay(micros_to_ticks(delay * 1000000)); // Wait for the specified delay
  vg_exit(); // Return to text mode
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if (vg_init(mode) == NULL) { // Initialize video mode
    printf("Failed to set video mode 0x%03x\n", mode);
    return 1;
  }

  for (uint16_t i = 0; i < height; i++) { // Draw the rectangle row by row
    for (uint16_t j = 0; j < width; j++) {
      vg_draw_pixel(x + j, y + i, color);
    }
  }

  vg_exit(); // Return to text mode
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (vg_init(mode) == NULL) { // Initialize video mode
    printf("Failed to set video mode 0x%03x\n", mode);
    return 1;
  }

  uint16_t rect_width = get_hres() / no_rectangles;
  uint16_t rect_height = get_vres() / no_rectangles;

  for (uint8_t row = 0; row < no_rectangles; row++) {
    for (uint8_t col = 0; col < no_rectangles; col++) {
      uint32_t color = (first + (row * no_rectangles + col) * step) % (1 << get_bits_per_pixel());
      vg_draw_rectangle(col * rect_width, row * rect_height, rect_width, rect_height, color);
    }
  }

  vg_exit(); // Return to text mode
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (vg_init(VBE_MODE) == NULL) { // Initialize video mode
    printf("Failed to set video mode\n");
    return 1;
  }

  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, XPM_8_8_8, &img); // Load XPM image
  if (sprite == NULL) {
    printf("Failed to load XPM\n");
    vg_exit();
    return 1;
  }

  vg_draw_xpm(sprite, img, x, y); // Draw the XPM image
  vg_exit(); // Return to text mode
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  if (vg_init(VBE_MODE) == NULL) { // Initialize video mode
    printf("Failed to set video mode\n");
    return 1;
  }

  xpm_image_t img;
  uint8_t *sprite = xpm_load(xpm, XPM_8_8_8, &img); // Load XPM image
  if (sprite == NULL) {
    printf("Failed to load XPM\n");
    vg_exit();
    return 1;
  }

  int16_t dx = (xf - xi) / abs(speed);
  int16_t dy = (yf - yi) / abs(speed);

  uint16_t x = xi, y = yi;
  while ((x != xf || y != yf) && speed != 0) {
    vg_draw_xpm(sprite, img, x, y); // Draw the XPM image at the current position
    tickdelay(micros_to_ticks(1000000 / fr_rate)); // Wait for the frame rate
    vg_clear_xpm(sprite, img, x, y); // Clear the previous position
    x += dx;
    y += dy;
  }

  vg_exit(); // Return to text mode
  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}