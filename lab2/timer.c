#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  // using read-back command -> read programmed mode,
  // count value and current state of the OUT line of 
  // the selected timer(s)
  
  // read-back command is written to the control register
  // bits
  // 6,7 -> distinguish between control word and read-back command
  // bit mask 1-3 -> selection of counters
  // bit mask 4-5 -> read either the count value or the programmed mode or both

  /* timer's STATUS byte (bit 4 of the read-back command) ->
  7 -> output
  6 -> null count
  5-4 -> type of access
  3-1 -> programmed mode
  0 -> BCD
  */

  /*int port = 0;
  if (timer == 0) {
    port = TIMER_0;
  } else if (timer == 1){
    port = TIMER_1;
  } else if (timer == 2){
    port = TIMER_2;
  } else {
    printf("Error: timer not in range 0-2! (Inside %s)\n", __func__);
  }*/

  printf("Inside %s!\n", __func__);

  int port = 0;
  util_sys_inb(port, st);
  
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("Inside %s!\n", __func__);

  return 0;
}
