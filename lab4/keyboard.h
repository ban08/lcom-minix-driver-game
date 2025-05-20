#ifndef _LCOM_KBD_H_
#define _LCOM_KBD_H_


#include <lcom/lcf.h>
#include <minix/sysutil.h>
#include "i8042.h"
#include "kb_controller.h"

void (kbc_ih)();

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();

int (keyboard_restore)();


int util_sys_inb_counter(int port, uint8_t *value);

#endif
