#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"


uint32_t cnt=0;

int h_id=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19) {

    return 1;
  }

  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) {
    return 1;
  }

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB; 

  uint32_t initialValue = TIMER_FREQ / freq;

  uint8_t MSB;
  uint8_t LSB;

  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  uint8_t selectedTimer;      
  if (timer == 0) {
    controlWord |= TIMER_SEL0;
    selectedTimer = TIMER_0;
  } 
  else if (timer == 1) {
    controlWord |= TIMER_SEL1;
    selectedTimer = TIMER_1;
  } 
  else if (timer == 2) {
    controlWord |= TIMER_SEL2;
    selectedTimer = TIMER_2;
  } 
  else {
    return 1;
  }

  if (sys_outb(TIMER_CTRL, controlWord) != 0) {
    return 1;
  }
  if (sys_outb(selectedTimer, LSB) != 0) {
    return 1;
  }
  if (sys_outb(selectedTimer, MSB) != 0) {
    return 1;
  }

  return 0;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {

      return 1;
  }

  *bit_no = BIT(h_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &h_id) != 0) {

      return 1;
  }

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&h_id) != 0) {

      return 1;
  }

  return 0;
}

void (timer_int_handler)() {
  
  cnt = cnt + 1;
}


int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer < 0 || timer > 2){
    return 1;
  }

  uint8_t RBC = (TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ | TIMER_RB_CMD);

  if (sys_outb(TIMER_CTRL, RBC) != 0)
  { 
    return 1;
  }

  if (util_sys_inb(TIMER_0 + timer, st)){ 
    return 1; 
  }


  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val data;

  if (field == tsf_all) { 
    data.byte = st; 
  }
  else if (field == tsf_initial) {                                       
      st = (st >> 4) & 0x03;
  
      if (st == 1)
      {
        data.in_mode = LSB_only;
      }
      else if (st == 2)
      {
        data.in_mode = MSB_only;
      }
      else if (st == 3) {
        data.in_mode = MSB_after_LSB;
      }
      else {
        data.in_mode = INVAL_val;
      }
  }

  else if (field == tsf_mode) {
      st = (st >> 1) & 0x07;
  
      if (st == 6)
      {
      data.count_mode = 2;
      }
      else if (st == 7){ 
        data.count_mode = 3;
      }
      else {
        data.count_mode = st;
      }
  }

  else if (field == tsf_base) {
      data.bcd = st & TIMER_BCD;
  }

  else {
      return 1;
  }

  if (timer_print_config(timer, field, data) != 0) 
  {
    return 1;
  }


  return 0;
}
