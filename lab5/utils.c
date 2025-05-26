#include <lcom/lcf.h>

#include <stdint.h>






int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  if (lsb != NULL)
  {
    *lsb = 0xFF & val;
  }
  else {
    return 1;
  }

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  if (msb != NULL)
  {
    *msb = val >> 8;
  }
  else 
  {
    return 1;

  }
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL)
  {
    return 1;
  }

  uint32_t v;

  int r = sys_inb(port, &v);
  //escreve o valor que recebe no sys_inb na variável v
  *value = v & 0xFF;
  //pega nos primeiros 2 bytes da variável val
  return r;
}


/*
int util_sys_inb_counter(int port, uint8_t *value) 
{
    counter_kb_controller = counter_kb_controller + 1; 
    
    return util_sys_inb(port, value);
}
*/

int (was_ESC_pressed)() 
{

  int status;

  uint8_t kb_irq_mask;

  message message;


  if (keyboard_subscribe_interruptions(&kb_irq_mask) != 0) 
  {

    return 1;
  }
  while (scan_code != BREAK_CODE_ESCAPE)
  {
    if (driver_receive(ANY, &message, &status) != 0) 
    {

      printf("driver_receive failed\n");
      continue;
    }
    if (is_ipc_notify(status)) 
    {

      if (_ENDPOINT_P(message.m_source) == HARDWARE) 
      {

        if (message.m_notify.interrupts & kb_irq_mask) 
        {
          kbc_ih();
        }
      }
    }
      
  }
  if (keyboard_unsubscribe_interruptions() == 0) 
  {

    return 0;
  }

  return 1;
}