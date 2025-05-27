#include <lcom/lcf.h>
#include "video_card.h"
#include "timer.c"
#include "keyboard.h"
#include "game.h"
#include "kb_controller.h"


extern uint8_t scan_code;
extern int counter;
extern int h_id;
extern int keyboard_hook_id;

int x_position = 40;
int y_position = 700;

movement_states movement_state = BASE;

int main(int argc, char *argv[]) {

    lcf_set_language("EN-US");
    //lcf_trace_calls("/home/lcom/labs/trace.txt"); // optional
    //lcf_log_output("/home/lcom/labs/output.txt"); // optional

    if (lcf_start(argc, argv) == 1) return 1;

    return 0;

}


int(proj_main_loop)(int argc, char *argv[]) {
    if (set_frame_buffer(0x115) != 0) 
    {
        return 1;
    }
    if (set_graphic_mode(0x115) != 0) return 1;  


    uint8_t irq_mask;

    if (keyboard_subscribe_interruptions(&irq_mask)) return 1;


    uint8_t irq_set;

    if (timer_subscribe_int(&irq_set)) return 1;

    int ipc_status;

    message msg;

    int r, irq_set_timer = BIT(h_id), irq_set_kbd = irq_mask;

    bool running = true;

    while (running) 
    {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) continue;

        if (is_ipc_notify(ipc_status)) 
        {

            switch (_ENDPOINT_P(msg.m_source)) 
            {

                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_timer) 
                    {
                        timer_int_handler();
                        game_update();
                        game_draw();
                    }
                    if (msg.m_notify.interrupts & irq_set_kbd) 
                    {
                        kbc_ih();
                        if (scan_code == BREAK_CODE_ESCAPE) running = false;
                        if (scan_code == 0x39 && y_position == 10) game_jump();
                    }
                    break;
            }
        }
    }

    timer_unsubscribe_int();

    sys_irqrmpolicy(irq_mask); //unsubscribe interrupts. a função keyboard_unsubscribe_interruptions nao estava a fuincionar pq passava uma reference
    //nos labs, essa função trabalha bem, aqui, nem por isso. é um ponto a estudar no futuro


    vg_exit();  

    return 0;

}

