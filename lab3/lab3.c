#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"
#include "kb_controller.c"
#include "timer.c"
#include "i8254.h"
#include "i8042.h"



extern uint8_t scan_code;

extern uint32_t counter_kb_controller;

extern uint32_t cnt;



int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  message message;

  int ipc_stat;

  uint8_t irq_mask;

  if (keyboard_subscribe_interruptions(&irq_mask) != 0) 
  {

    return 1;

  }

  while (scan_code != BREAK_CODE_ESCAPE) { //como especificado no guião, "The kbd_test_scan() function should return when the user releases the Esc key"

    if (driver_receive(ANY, &message, &ipc_stat) != 0)
    {
      printf("Error");

      continue; //passa para a proxima iteration

    }

    if (is_ipc_notify(ipc_stat)) 
    {
      if (_ENDPOINT_P(message.m_source) == HARDWARE)
      {
          if (message.m_notify.interrupts & irq_mask) 
          {

            kbc_ih();

            bool is_make = ((scan_code & MAKE_CODE) == 0);

            uint8_t size;

            if (scan_code == TWO_BYTES_MASK) {

              size = 2;

          
            } else {

              size = 1;

            }            
            kbd_print_scancode(is_make, size, &scan_code);          
          }
      }
    }
  }

  if (((kbd_print_no_sysinb(counter_kb_controller) != 0) || (keyboard_unsubscribe_interruptions() != 0))) 
  {
    return 1;
  }


  return 0;
  
}

int(kbd_test_poll)() {
  while (scan_code != BREAK_CODE_ESCAPE) 
  { //como referido na documentation do lab3, "The kbd_test_scan() function should return when the user releases the Esc key"

    if (readKBControlleroutput(KBC_OUT_COMMAND, &scan_code, 0) == 0) 
    {

      bool is_make_code = (scan_code & MAKE_CODE) == 0;
      uint8_t size;
      
      if (scan_code == TWO_BYTES_MASK) 
      {

        size = 2;

      } else 
      {

        size = 1;

      }
      
      kbd_print_scancode(is_make_code, size, &scan_code);      
    }   
  }

  
  return keyboard_restore();
}

int(kbd_test_timed_scan)(uint8_t n) {
  message message;

  int ipc_stat;


  uint8_t irq_set_timer, irq_set_kb_controller;


  int seconds = 0;

  if ((timer_subscribe_int(&irq_set_timer) != 0) || (keyboard_subscribe_interruptions(&irq_set_kb_controller) != 0)) 
  {
    return 1;
  }

  while (scan_code != BREAK_CODE_ESCAPE && seconds < n)
  {

    if(driver_receive(ANY, &message, &ipc_stat) != 0 )
    {
      printf("Error!");
      continue; //próxima iteration do loop

    }

    if(is_ipc_notify(ipc_stat) && _ENDPOINT_P(message.m_source) == HARDWARE) 
    {
      if (message.m_notify.interrupts & irq_set_kb_controller) 
      {
        kbc_ih();

        bool is_make = (scan_code & MAKE_CODE) == 0;

        uint8_t size;
        
        if (scan_code == TWO_BYTES_MASK) 
        {
          size = 2;
        } else 
        {
          size = 1;
        }
        
        kbd_print_scancode(is_make, size, &scan_code);
                
        seconds = 0;
        cnt = 0;
      }
      if (message.m_notify.interrupts & irq_set_timer) 
      {
        timer_int_handler();
        if (cnt % 60 == 0) 
        {
          seconds++;
        }
      }
    }
  }

  if ((timer_unsubscribe_int() != 0) || (keyboard_unsubscribe_interruptions() != 0) || (kbd_print_no_sysinb(counter_kb_controller) != 0))
  {
    return 1;
  }

  return 0;
}

