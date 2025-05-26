#ifndef _LCOM_KB_CONTROLLER_H_
#define _LCOM_KB_CONTROLLER_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"
//#include "keyboard.h"

int (readKBControllerstat)(uint8_t* status);

int (readKBControlleroutput)(uint8_t port, uint8_t *output, uint8_t mouse);

int (writeKBControllercommand)(uint8_t port, uint8_t cmd_byte);

#endif
