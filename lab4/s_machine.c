#include "s_machine.h"


extern struct packet m_pckt;
Possible_States curr_state = START;
static int16_t x_len_total = 0;
const uint16_t gesture_length = 50;


void (update_state_machine)(uint8_t tolerance) {

    // Atualização do valor percorrido em X
    if (m_pckt.delta_x > 0)
      x_len_total += m_pckt.delta_x;

    switch (curr_state) {

      case START:
          if (m_pckt.lb && !m_pckt.rb && !m_pckt.mb)
            curr_state = UP;
          break;

      case UP:
          if (m_pckt.lb &&
              m_pckt.delta_y < -tolerance &&
              m_pckt.delta_x > tolerance)
            curr_state = UP;

          else if (!m_pckt.lb &&
                   x_len_total >= gesture_length)
          {
            curr_state = VERTEX;
            x_len_total = 0; 
          }

          else if (m_pckt.rb || m_pckt.mb || m_pckt.delta_y > tolerance)
          {
            curr_state = START;
            x_len_total = 0;
          }
          break;

      case VERTEX:
          if (m_pckt.rb && !m_pckt.lb && !m_pckt.mb)
            curr_state = DOWN;

          else if (m_pckt.lb || m_pckt.mb)
          {
            curr_state = START;
            x_len_total = 0;
          }
          break;

      case DOWN:
          if (m_pckt.rb &&
              m_pckt.delta_y > tolerance &&
              m_pckt.delta_x > tolerance)
            curr_state = DOWN;

          else if (!m_pckt.rb &&
                   x_len_total >= gesture_length)
          {
            curr_state = END;
          }

          else if (m_pckt.lb || m_pckt.mb || m_pckt.delta_y < -tolerance)
          {
            curr_state = START;
            x_len_total = 0;
          }
          break;

      case END:
          break;
    }

}
