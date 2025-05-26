#include <lcom/lcf.h>
#include "video_card.h"
#include "timer.h"
#include "keyboard.h"
#include "game.h"


extern uint8_t scan_code;
extern int counter;
extern int h_id;
extern int keyboard_hook_id;

int x_position = 2;
int y_position = 10;

movement_states movement_state = IDLE;

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    //lcf_trace_calls("/home/lcom/labs/trace.txt"); // optional
    //lcf_log_output("/home/lcom/labs/output.txt"); // optional

    if (lcf_start(argc, argv) == 1) return 1;

    return 0;

}


int(proj_main_loop)(int argc, char *argv[]) {
    if (vg_init(0x115) == NULL) return 1;

    if (keyboard_subscribe_interruptions()) return 1;

    if (timer_subscribe_int()) return 1;

    int ipc_status;

    message msg;

    int r, irq_set_timer = BIT(h_id), irq_set_kbd = BIT(keyboard_hook_id);

    bool running = true;

    while (running) 
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(msg)) {

            switch (_ENDPOINT_P(msg.m_source)) {

                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_timer) {
                        timer_int_handler();
                        game_update();
                        game_draw();
                    }
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbd_int_handler();
                        if (scan_code == BREAK_CODE_ESCAPE) running = false;
                        if (scan_code == 0x39) game_jump();
                    }
                    break;
            }
        }
    }

    timer_unsubscribe_int();~

    kbd_unsubscribe_int();

    vg_exit();

    return 0;
}
