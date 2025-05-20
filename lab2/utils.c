#include <lcom/lcf.h>

#include <stdint.h>




uint32_t counter_kb_controller;

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
