#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_

#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "kb_controller.h"

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads the scan code from the keyboard using the keyboard controller.
 */
void (kbc_ih)();

/**
 * @brief Subscribes to keyboard interrupts.
 *
 * @param bit_no Pointer to a variable where the hook bit mask will be stored.
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_subscribe_interruptions)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from keyboard interrupts.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_unsubscribe_interruptions)();

/**
 * @brief Restores the keyboard controller command byte.
 *
 * This enables keyboard interrupts again if they were disabled.
 * 
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_restore)();

/**
 * @brief Reads from a port and counts accesses to the keyboard controller.
 *
 * Not used in current implementation. Useful for debugging or tracking.
 *
 * @param port I/O port to read from.
 * @param value Pointer to store the read value.
 * @return 0 on success, non-zero on failure.
 */
int util_sys_inb_counter(int port, uint8_t *value);

#endif // _LCOM_KBD_H_
