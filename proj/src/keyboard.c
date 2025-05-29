#include "keyboard.h"
#include "i8042.h"
#include <lcom/lcf.h>

uint8_t scan_code = 0;               ///< Stores the current scan code.
uint32_t counter_kb_controller = 0;  ///< Counter for keyboard controller accesses.

int keyboard_hook_id = 1;            ///< Hook ID used for IRQ subscription.

/**
 * @brief Subscribes keyboard interrupts.
 *
 * Sets up the keyboard IRQ policy and enables exclusive access.
 *
 * @param bit_no Address of variable to store the hook ID bit mask.
 * @return 0 on success, 1 on error.
 */
int (keyboard_subscribe_interruptions)(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;
    *bit_no = BIT(keyboard_hook_id);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

/**
 * @brief Unsubscribes keyboard interrupts.
 *
 * Removes the IRQ policy.
 *
 * @return 0 on success, non-zero on failure.
 */
int (keyboard_unsubscribe_interruptions)() {
    return sys_irqrmpolicy(&keyboard_hook_id);
}

/**
 * @brief Keyboard interrupt handler.
 *
 * Reads a scan code from the output buffer using the controller helper.
 */
void (kbc_ih)() {
    if (readKBControlleroutput(KBC_OUT_COMMAND, &scan_code, 0) != 0) {
        printf("Error reading scan_code.\n");
    }
}

/**
 * @brief Restores the command byte to enable keyboard interrupts.
 *
 * Reads the command byte, enables bit 0 (interrupts), then writes it back.
 *
 * @return 0 on success, non-zero on error.
 */
int (keyboard_restore)() {
    uint8_t cmd_byte;
    if (writeKBControllercommand(KBC_IN_COMMAND, KBC_READ_COMMAND) != 0 ||
        readKBControlleroutput(KBC_OUT_COMMAND, &cmd_byte, 0) != 0) {
        return 1;
    }
    cmd_byte |= EN_INT;
    if (writeKBControllercommand(KBC_IN_COMMAND, KBC_WRITE_COMMAND) != 0 ||
        writeKBControllercommand(KBC_WRITE_COMMAND, cmd_byte) != 0) {
        return 1;
    }
    return 0;
}

// Not used in current implementation
int util_sys_inb_counter(int port, uint8_t *value) {
    counter_kb_controller++;
    return util_sys_inb(port, value);
}
