// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "keyboard.h"
#include "video_card.h"
#include "timer.c"
#include "utils.c"

extern vbe_mode_info_t mode_info;
extern uint8_t scan_code;
extern int counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/lab5/output.txt");

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

  if (set_graphic_mode(mode) != 0) 
  {

    return 1;
  }

  sleep(delay);

  if (vg_exit() == 0) 
  {
    
    return 0;
  }

  return 1;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  uint32_t new_color;

  if ((set_graphic_mode(mode) == 0)&&
  (set_frame_buffer(mode) == 0)&&
  (normalize_color(color, &new_color) == 0)&&
  (vg_draw_rectangle(x, y, width, height, new_color) == 0)&&
  (was_ESC_pressed() == 0)&&
  (vg_exit() == 0)) 
  {
    return 0; //tudo certo, posso retornar success
  }

  return 1; //algumas das condições anteriores retornou 1, fail
}




int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first_color, uint8_t step)
{
  if ((set_frame_buffer(mode) != 0) || (set_graphic_mode(mode) != 0))
  {

    return 1;
  }

  int rect_height = mode_info.YResolution / no_rectangles;

  int rect_width = mode_info.XResolution / no_rectangles;

  for (int row = 0 ; row < no_rectangles ; ++row)
  {

    for (int column = 0 ; column < no_rectangles ; ++column)
    {

      uint32_t color;

      if (mode_info.MemoryModel == 0x06)
      {

        //verifica se esta no modo direct color, ou se estamos a usar indexed color (uma palete de cores, no fundo)
        uint32_t r = get_red_component(column, step, first_color);

        uint32_t g = get_green_component(row, step, first_color);

        uint32_t b = get_blue_component(column, row, step, first_color);


        color = get_direct_color(r, g, b);
      }
      else
      {

        color = get_indexed_color(column, row, step, first_color, no_rectangles);
      }
      int x_pos = column * rect_width;
      int y_pos = row * rect_height;

      if (vg_draw_rectangle(x_pos, y_pos, rect_width, rect_height, color))
      {

        return 1;
      }
    }
  }

  if ((was_ESC_pressed() == 0) && (vg_exit() == 0))
  {

    return 0;
  }

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  

  if (set_frame_buffer(0x105) == 0 &&
  set_graphic_mode(0x105) == 0 &&
  print_xpm(xpm, x, y) == 0 &&
  was_ESC_pressed() == 0 &&
  vg_exit() == 0) 
  {
    return 0;
  }

  return 1;


}
int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  int ipc_stat;

  message message;



  uint8_t timer_irq;

  uint8_t kb_irq;


  uint8_t horizontal;

  if (xi == xf && yi < yf) 
  {

    horizontal = 0;
  }
  else if (yi == yf && xi < xf) 
  {

    horizontal = 1;
  }
  else 
  {

    return 1;
  }

  if (keyboard_subscribe_interruptions(&kb_irq) != 0 ||
      timer_subscribe_int(&timer_irq) != 0 ||
      timer_set_frequency(0, fr_rate) != 0 ||
      set_frame_buffer(0x105) != 0 ||
      set_graphic_mode(0x105) != 0 ||
      print_xpm(xpm, xi, yi) != 0) 
      {
        return 1;

      }

  while (scan_code != BREAK_CODE_ESCAPE && (xi < xf || yi < yf)) {
    if (driver_receive(ANY, &message, &ipc_stat) != 0) 
    {      
      continue;
    }

    if (is_ipc_notify(ipc_stat)) {
      if (_ENDPOINT_P(message.m_source) == HARDWARE) 
      {
        if (message.m_notify.interrupts & kb_irq)
        {
          kbc_ih();
        }

        if (message.m_notify.interrupts & timer_irq) {
          if (vg_draw_rectangle(xi, yi, 100, 100, 0xFFFFFF) != 0) 
          {
            return 1;
          }

          if (horizontal) 
          {
            xi += speed;
            if (xi > xf) 
            {
              xi = xf;
            }
          } else 
          {
            yi += speed;
            if (yi > yf) yi = yf;
          }

          if (print_xpm(xpm, xi, yi) != 0) 
          {
            return 1;
          }
        }
      }
    }
  }

  if (vg_exit() == 0 && timer_unsubscribe_int() == 0 && keyboard_unsubscribe_interruptions() == 0) 
  {
    return 0;
  }

  return 1;
}






//NAO TENHO DE IMPLEMENTAR

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}

//NAO TENHO DE IMPLEMENTAR

