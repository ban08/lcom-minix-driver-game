#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /*0000 1111
  AND
  1010 1010
  =
  0000 1010
  */
  printf("Inside %s!\n", __func__);
  *lsb = val & 0x00FF;
  printf("LSB of %d is %u\n", val, *lsb);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /*
  1111 0000
  AND
  1010 1010
  =
  1010 0000
  >> quatro para o lado
  */
  printf("Inside %s!\n", __func__);
  *msb = (val & 0xFF00) >> 8;
  printf("MSB of %d is %u\n", val, *msb);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  printf("Inside %s!\n", __func__);
  uint8_t lsb, msb = 0x00;
  util_get_LSB(0xABC2, &lsb);
  util_get_MSB(0xABC2, &msb);

  //sys_inb(port, value);
  return 0;
}
