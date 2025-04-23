#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_


#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "KBC.h"

void (kbc_ih)();

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();

int (keyboard_restore)();

#endif