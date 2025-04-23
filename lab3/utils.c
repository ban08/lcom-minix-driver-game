#include <lcom/lcf.h>

#include <stdint.h>


uint32_t counter_kb_controller; //adaptação para o lab3

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL){
    return 1;
  }
  else{
    *lsb = 0xFF & val;
  }
  
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL){
    return 1;
  }
  else {
    *msb = val >> 8;
  }
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if (value == NULL){
    return 1;
  }

  uint32_t val;

  int ret = sys_inb(port, &val);
  //escreve o valor que recebe no sys_inb na variável val
  *value = val && 0xFF;
  //pega nos primeiros 2 bytes da variável val
  return ret;
}
