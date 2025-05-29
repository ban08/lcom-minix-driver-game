#ifndef _LCOM_KB_CONTROLLER_H_
#define _LCOM_KB_CONTROLLER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"

/**
 * @brief Reads the status register of the keyboard controller.
 * 
 * @param status Pointer to a variable where the status will be stored.
 * @return 0 on success, non-zero on failure.
 */
int (readKBControllerstat)(uint8_t* status);

/**
 * @brief Reads output from the keyboard controller's output buffer.
 * 
 * This function waits until the output buffer is full and then reads the output,
 * checking for parity and timeout errors.
 * 
 * @param port I/O port to read from (e.g., KBC_OUT_BUF).
 * @param output Pointer to store the read byte.
 * @param mouse 1 if expecting mouse data, 0 for keyboard data.
 * @return 0 on success, non-zero on failure.
 */
int (readKBControlleroutput)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes a command to the keyboard controller.
 * 
 * Waits until the input buffer is empty before sending the command.
 * 
 * @param port I/O port to write to (usually KBC_CMD_REG or KBC_IN_BUF).
 * @param cmd_byte Command byte to send.
 * @return 0 on success, non-zero on failure.
 */
int (writeKBControllercommand)(uint8_t port, uint8_t cmd_byte);

#endif // _LCOM_KB_CONTROLLER_H_
