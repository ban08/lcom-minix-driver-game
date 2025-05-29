#ifndef _TIMER_H_
#define _TIMER_H_

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"

/**
 * @brief Sets the frequency of the specified timer.
 * 
 * @param timer Timer number (0, 1, or 2)
 * @param freq Desired frequency in Hz
 * @return 0 on success, non-zero otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts.
 * 
 * @param bit_no Pointer to store the bit number for the interrupt mask
 * @return 0 on success, non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts.
 * 
 * @return 0 on success, non-zero otherwise
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler. Increments global counter.
 */
void (timer_int_handler)();

/**
 * @brief Retrieves the configuration byte of a given timer.
 * 
 * @param timer Timer number (0, 1, or 2)
 * @param st Pointer to variable to store the status byte
 * @return 0 on success, non-zero otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Interprets and displays the configuration of a timer.
 * 
 * @param timer Timer number (0, 1, or 2)
 * @param st Status byte of the timer
 * @param field Enum specifying which field to interpret
 * @return 0 on success, non-zero otherwise
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif /* _TIMER_H_ */
