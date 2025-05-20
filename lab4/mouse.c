#include "mouse.h"

int m_h_id = 5; // kdb -> 1 ; timer -> 0

struct packet m_pckt;

uint8_t mouse_ctrler_bytes[3];

uint8_t curr_byte;

uint8_t byte_idx = 0;

int (mouse_subscribe_int)(uint8_t *bit_no)
{
  if (bit_no != NULL)
  {

    *bit_no = BIT(m_h_id);

    return sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &m_h_id);

  }

  return 1;

}

int (mouse_unsubscribe_int)()
{

  return sys_irqrmpolicy(&m_h_id);
}

void (mouse_ih)(){
  if(readKBControlleroutput(KBC_WRITE_COMMAND, &curr_byte, 1) == 1)
  {

  printf("Error: Could not read keyboard controller output! (readKBControlleroutput returned 1)\n");


  }
}




void (mouse_ctrler_bytes_to_packet)() 
{
    for (int i = 0; i < 3; i++) 
    {
      m_pckt.bytes[i] = mouse_ctrler_bytes[i];
    }
  
    uint8_t byte0 = mouse_ctrler_bytes[0];

    uint8_t byte1 = mouse_ctrler_bytes[1];

    uint8_t byte2 = mouse_ctrler_bytes[2];


    //
  
    m_pckt.lb = byte0 & LB;

    m_pckt.mb = byte0 & MB;

    m_pckt.rb = byte0 & RB;

    m_pckt.x_ov = byte0 & M_X_OVERFLOW;

    m_pckt.y_ov = byte0 & M_Y_OVERFLOW;
  
    if (byte0 & M_X_SIG)
    {
        m_pckt.delta_x = (int16_t)(0xFF00 | byte1);
    }
    else
    {
    m_pckt.delta_x = (int16_t)byte1;
    }
  
    if (byte0 & M_Y_SIG)
    {
      m_pckt.delta_y = (int16_t)(0xFF00 | byte2);
    }
    else 
    {
      m_pckt.delta_y = (int16_t)byte2;
    }

}
  


int (mouse_write)(uint8_t command) {

  uint8_t attempts = 10;
  uint8_t reply;

  do {
    attempts = attempts - 1;

    if ((writeKBControllercommand(KBC_IN_COMMAND, WR_BYTE_M)) || (writeKBControllercommand(KBC_OUT_COMMAND, command))) 
    {

        return 1;
    }

    tickdelay(micros_to_ticks(WAIT_KBCONTROLLER));


    if (util_sys_inb(KBC_OUT_COMMAND, &reply)) 
    {
        return 1;
    }
    if (reply == ACK) 
    {
        return 0;
    }
  } while (reply != ACK && attempts);

  return 1;
}



void m_sync_pckt() 
{
    //
    if (((byte_idx == 0) && (curr_byte & FST_BYTE)) || (byte_idx > 0)) 
    {

        mouse_ctrler_bytes[byte_idx] = curr_byte;


        byte_idx = byte_idx + 1;

    }
      
}


