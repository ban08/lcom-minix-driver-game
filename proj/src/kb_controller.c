#include "kb_controller.h"


/**
 * @brief Reads the keyboard controller status register.
 *
 * @param status Pointer to variable to store the result.
 * @return 0 on success, non-zero on failure.
 */
int (readKBControllerstat)(uint8_t* status) {
    return util_sys_inb(KBC_STATUS_REGISTER, status);
}

/**
 * @brief Reads a byte from the keyboard controller's output buffer.
 *
 * Checks for buffer readiness and verifies against parity and timeout errors.
 * Also verifies whether the byte is from the mouse or the keyboard, based on `mouse` flag.
 *
 * @param port The port to read from (typically KBC_OUT_BUF).
 * @param output Pointer where the output byte will be stored.
 * @param mouse Boolean flag (1 for mouse, 0 for keyboard).
 * @return 0 on success, non-zero on failure.
 */
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

/**
 * @brief Writes a command byte to the keyboard controller.
 *
 * Waits for the input buffer to be empty before sending.
 *
 * @param port The port to write to (usually command or input buffer).
 * @param cmd_byte The command byte to send.
 * @return 0 on success, non-zero on failure.
 */
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
