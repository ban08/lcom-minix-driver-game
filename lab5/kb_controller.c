#include "kb_controller.h"



int (readKBControllerstat)(uint8_t* status) {

    return util_sys_inb(KBC_STATUS_REGISTER, status);

}

int readKBControlleroutput(uint8_t port, uint8_t *output, uint8_t mouse) {

    uint8_t status;

    
    for (int i = 0; i < 10; i++){

        if (readKBControllerstat(&status) != 0) {
            printf("Error: Status!\n");
            return 1;
        }

        if ((status & FULL_OUTPUT_BUF) != 0) {
            if(util_sys_inb(port, output) != 0){
                printf("Error: FULL_OUTPUT_BUF!\n");
                return 1;
            }
            if((status & PARITY_ERR) != 0){
                printf("Error: PARITY_ERR!\n");
                return 1;
            }
            if((status & TIMEOUT_ERR) != 0){
                printf("Error: TIMEOUT_ERR!\n");
                return 1;
            }
            if (mouse && !(status & OUT_NOT_FOUND)) {
                printf("Error: OUT_NOT_FOUND_MOUSE\n");
                return 1;
            } 
            if (!mouse && (status & OUT_NOT_FOUND)) {
                printf("Error: OUT_NOT_FOUND\n");
                return 1;
            } 
            return 0; //se não existirem erros, return 0, success
        }


        tickdelay(micros_to_ticks(WAIT_KBCONTROLLER)); //20000 microsegundos, ou seja, 20ms, tal como especificado em "IBM's specification of the i8042 requires the keyboard to respond to a command in 20 ms"

    }
    return 1;
}

int (writeKBControllercommand)(uint8_t port, uint8_t cmd_byte) {

    uint8_t status;

    uint8_t attempts = 10;

    for (int i = 0; i < attempts; i++){

        if (readKBControllerstat(&status) != 0){
            printf("Error: Status!\n");
            return 1;
        }

        if ((status & FULL_INPUT_BUF) == 0){

            if(sys_outb(port, cmd_byte) != 0){

                printf("Error: cmd_byte!\n");

                return 1;

            }

            return 0;
        }
        tickdelay(micros_to_ticks(WAIT_KBCONTROLLER));
        
    }
    
    return 1;

}
