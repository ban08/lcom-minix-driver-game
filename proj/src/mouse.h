#ifndef _PS2_MOUSE_H_
#define _PS2_MOUSE_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "kb_controller.h"

/**
 * @brief Subscribes to mouse interrupts.
 * 
 * @param bit_no Pointer to store the bit number of the IRQ.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts.
 * 
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler.
 */
void (mouse_ih)();

/**
 * @brief Converts the 3 bytes received from the mouse into a packet struct.
 */
void (mouse_ctrler_bytes_to_packet)();

/**
 * @brief Sends a command byte to the mouse.
 * 
 * @param command The command byte to send.
 * @return 0 on success, non-zero otherwise.
 */
int (mouse_write)(uint8_t command);

/**
 * @brief Synchronizes mouse packet collection based on the first byte.
 */
void (m_sync_pckt)();

#endif
