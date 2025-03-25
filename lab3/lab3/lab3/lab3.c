#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t irq_set = BIT(1); // Assuming IRQ1 for keyboard

  if (kbd_subscribe_int(&irq_set) != 0) {
    return 1;
  }

  while (scancode != ESC_BREAK_CODE) { // Assuming ESC_BREAK_CODE is defined
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih(); // Assuming kbc_ih() is implemented
            kbd_print_scancode(); // Assuming kbd_print_scancode() is implemented
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

int(kbd_test_poll)() {
  uint8_t scancode;

  while (scancode != ESC_BREAK_CODE) { // Assuming ESC_BREAK_CODE is defined
    kbc_poll(); // Assuming kbc_poll() is implemented
    kbd_print_scancode(); // Assuming kbd_print_scancode() is implemented
  }

  kbd_reenable_interrupts(); // Assuming kbd_reenable_interrupts() is implemented

  return 0;
}

int(kbd_test_timed_scan)(uint8_t idle) {
  int ipc_status;
  message msg;
  uint8_t r;
  uint8_t irq_set_kbd = BIT(1); // Assuming IRQ1 for keyboard
  uint8_t irq_set_timer = BIT(0); // Assuming IRQ0 for timer
  uint32_t timer_counter = 0;

  if (kbd_subscribe_int(&irq_set_kbd) != 0) {
    return 1;
  }

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    return 1;
  }

  while (scancode != ESC_BREAK_CODE && timer_counter < idle * 60) { // Assuming ESC_BREAK_CODE is defined
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih(); // Assuming kbc_ih() is implemented
            kbd_print_scancode(); // Assuming kbd_print_scancode() is implemented
            timer_counter = 0; // Reset timer counter on keyboard interrupt
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler(); // Assuming timer_int_handler() is implemented
            timer_counter++;
          }
          break;
        default:
          break;
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) {
    return 1;
  }

  if (timer_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}