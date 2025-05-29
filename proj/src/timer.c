#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include "i8254.h"

/// Global tick counter (incremented every timer interrupt)
uint32_t cnt = 0;

/// Timer interrupt hook ID
int h_id = 0;

/**
 * @brief Sets the frequency of the specified timer.
 *
 * This function changes the timer configuration to output interrupts at a
 * specified frequency using the LSB followed by MSB mode.
 *
 * @param timer Timer number (0, 1, or 2)
 * @param freq Desired frequency in Hz
 * @return 0 on success, 1 on failure
 */
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

  uint8_t MSB, LSB;
  util_get_MSB(initialValue, &MSB);
  util_get_LSB(initialValue, &LSB);

  uint8_t selectedTimer;
  if (timer == 0) {
    controlWord |= TIMER_SEL0;
    selectedTimer = TIMER_0;
  } else if (timer == 1) {
    controlWord |= TIMER_SEL1;
    selectedTimer = TIMER_1;
  } else if (timer == 2) {
    controlWord |= TIMER_SEL2;
    selectedTimer = TIMER_2;
  } else {
    return 1;
  }

  if (sys_outb(TIMER_CTRL, controlWord) != 0 ||
      sys_outb(selectedTimer, LSB) != 0 ||
      sys_outb(selectedTimer, MSB) != 0) {
    return 1;
  }

  return 0;
}

/**
 * @brief Subscribes to timer interrupts.
 *
 * Sets a policy to receive timer interrupts using IRQ_REENABLE.
 *
 * @param bit_no Address of variable to store the interrupt bitmask
 * @return 0 on success, 1 on failure
 */
int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    return 1;
  }

  *bit_no = BIT(h_id);
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &h_id);
}

/**
 * @brief Unsubscribes from timer interrupts.
 *
 * Removes the interrupt policy.
 *
 * @return 0 on success, 1 on failure
 */
int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&h_id);
}

/**
 * @brief Timer interrupt handler.
 *
 * Increments the global tick counter.
 */
void (timer_int_handler)() {
  cnt++;
}

/**
 * @brief Retrieves the current configuration of a given timer.
 *
 * @param timer Timer number (0, 1, or 2)
 * @param st Pointer to store the timer's status byte
 * @return 0 on success, 1 on failure
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || timer > 2) {
    return 1;
  }

  uint8_t rb_cmd = TIMER_RB_SEL(timer) | TIMER_RB_COUNT_ | TIMER_RB_CMD;

  if (sys_outb(TIMER_CTRL, rb_cmd) != 0 ||
      util_sys_inb(TIMER_0 + timer, st) != 0) {
    return 1;
  }

  return 0;
}

/**
 * @brief Displays a specific field of the timer configuration.
 *
 * Extracts and prints a requested field from a timer's status byte.
 *
 * @param timer Timer number (0, 1, or 2)
 * @param st Timer status byte
 * @param field Field to interpret (tsf_all, tsf_initial, tsf_mode, tsf_base)
 * @return 0 on success, 1 on failure
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;

  switch (field) {
    case tsf_all:
      data.byte = st;
      break;

    case tsf_initial: {
      uint8_t init_mode = (st >> 4) & 0x03;
      switch (init_mode) {
        case 1: data.in_mode = LSB_only; break;
        case 2: data.in_mode = MSB_only; break;
        case 3: data.in_mode = MSB_after_LSB; break;
        default: data.in_mode = INVAL_val;
      }
      break;
    }

    case tsf_mode: {
      uint8_t mode = (st >> 1) & 0x07;
      if (mode == 6) data.count_mode = 2;
      else if (mode == 7) data.count_mode = 3;
      else data.count_mode = mode;
      break;
    }

    case tsf_base:
      data.bcd = st & TIMER_BCD;
      break;

    default:
      return 1;
  }

  return timer_print_config(timer, field, data);
}
