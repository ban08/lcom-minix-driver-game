// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/shared/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/shared/lab5/output.txt");

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
  
  // switch the video adapter to the graphics mode specified
  // in the argument using VBE interface and back to the default
  // text mode

  // uses VBE INT 0x10 interface

  reg86_t r86;

  memset(&r86, 0, sizeof(r86));

  r86.intno = 0x10;
  r86.ah = 0x4F;
  r86.al = 0x02;
  r86.bx = mode | BIT(14);


  if (sys_int86(&r86) != 0){
    printf("Error on video_test_init on %s\n", __func__);
    return 1;
  }

  sleep(delay);

  vg_exit();
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  //1) map video memory to the process' address space
  


  //2) change video mode to the one on the argument

  vbe_mode_info_t vmi_p;
  memset(&vmi_p, 0, sizeof(vmi_p));

  // should use VBE function 0x01 - Return VBE Mode Information
  if (vbe_get_mode_info(mode, &vmi_p) != 0){
    printf("Error on vbe_get_mode_info call on %s\n", __func__);
    return 1;
  }
  

  //


  if (vg_draw_hline(x, y, width, color) != 0){
    printf("Error on vg_draw_hline call on %s\n", __func__);
    return 1;
  }

  //3) draw rectangle
   
  /*for (int i = 0; i <= height; i++){
    vg_draw_hline(x, y, width, color);
  }*/

  //4) reset the video mode to minix default's text mode and return after reeiving break code of the ESC key (0x81)


  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
