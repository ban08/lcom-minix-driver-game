#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>


typedef enum {
    START,
    END,
    UP,
    DOWN,
    VERTEX
} Possible_States;



void (update_state_machine)(uint8_t tolerance);

#endif /* STATE_MACHINE_H */
