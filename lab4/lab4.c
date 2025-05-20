// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "i8042.h"

// Any header files included below this line should have been created by you
#include "mouse.h"
#include "s_machine.h"
#include "timer.c"
#include "kb_controller.h"
extern struct packet m_pckt;
extern uint8_t byte_idx;
extern uint32_t cnt;
extern Possible_States curr_state;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  message mensagem;

  int ipc_stat;

  uint8_t msk;

  if ((mouse_subscribe_int(&msk) != 0) || (mouse_write(EN_DATA_REP) != 0))
  {

    return 1;
  }


  while (cnt != 0)
  {

    if (driver_receive(ANY, &mensagem, &ipc_stat) != 0)
    {
      printf("*!"); //temp. para dar debug apenas
      continue; //proxima iteration

    }

    if (is_ipc_notify(ipc_stat) && _ENDPOINT_P(mensagem.m_source) == HARDWARE && (mensagem.m_notify.interrupts & msk)) 
    {
    
      mouse_ih();

      m_sync_pckt();

  
      if (byte_idx == 3) 
      {
        mouse_ctrler_bytes_to_packet();

        mouse_print_packet(&m_pckt);

        byte_idx = 0;

        cnt = cnt - 1;
      }
    }
  }
    
  
  
  if ((mouse_write(DIS_DATA_REP) != 0) || (mouse_unsubscribe_int() != 0)) 
  {

    return 1;

  }



 
  return 0; //success


}

int (mouse_test_async)(uint8_t idle_time) {

  message mensagem;

  int ipc_stat;

  uint8_t m_msk = 0; //mask do mouse ps2

  uint8_t t_msk = 0; //mask do timer

  uint16_t timer_frequency = sys_hz();
  
  uint8_t seconds = 0;


  if ((timer_subscribe_int(&t_msk) != 0) || (mouse_subscribe_int(&m_msk) != 0)  || (mouse_write(EN_DATA_REP) != 0)) 
  {
    return 1;
  }


  while (seconds < idle_time) 
  { 

    if (driver_receive(ANY, &mensagem, &ipc_stat) != 0)
    {
      printf("**!"); //temp. para dar debug apenas
      continue;
    }

    if ((is_ipc_notify(ipc_stat)) && (_ENDPOINT_P(mensagem.m_source) == HARDWARE)) 
    {

      if (mensagem.m_notify.interrupts & t_msk) 
      {
        timer_int_handler();


        if (cnt % timer_frequency == 0) 
        {
          seconds = seconds + 1;
        }
      }
    
      if (mensagem.m_notify.interrupts & m_msk) 
      {
        mouse_ih();

        m_sync_pckt();

        if (byte_idx == 3) 
        {
          mouse_ctrler_bytes_to_packet();

          mouse_print_packet(&m_pckt);

          byte_idx = 0;
        }
        seconds = 0;

        cnt = 0;
      }
    }
    
  }

  if (mouse_write(DIS_DATA_REP) != 0 ||  timer_unsubscribe_int() != 0 ||  mouse_unsubscribe_int() != 0) return 1;


  return 0;

}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  
  message mensagem;

  int ipc_stat;

  uint8_t m_msk;

  if ((mouse_subscribe_int(&m_msk) != 0) || (mouse_write(EN_DATA_REP) != 0)) 
  {
  
    return 1;
  }

  while (curr_state != END) {

    if (driver_receive(ANY, &mensagem, &ipc_stat) != 0)
    {
      printf("***!"); //temp. para dar debug apenas
      continue;
    }

    if (is_ipc_notify(ipc_stat) && _ENDPOINT_P(mensagem.m_source) == HARDWARE) 
    {

      if (mensagem.m_notify.interrupts & m_msk) 
      {

        mouse_ih();

        m_sync_pckt();

        if (byte_idx == 3) 
        {

          mouse_ctrler_bytes_to_packet();

          update_state_machine(tolerance);

          byte_idx = 0;

        }
      }
    }
  }



  if ((mouse_write(DIS_DATA_REP) != 0) || (mouse_unsubscribe_int()) != 0)
  {

    return 1;
  }



  return 0;
}




















//isto não tenho de implementar
int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
//isto não tenho de implementar

