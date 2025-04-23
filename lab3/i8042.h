#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ            1
#define EN_INT                  BIT(0)
#define FULL_OUTPUT_BUF         BIT(0)

#define FULL_INPUT_BUF          BIT(1)
#define OUT_NOT_FOUND           BIT(5)
#define TIMEOUT_ERR             BIT(6)
#define PARITY_ERR              BIT(7)
#define MAKE_CODE               BIT(7)
#define BREAK_CODE_ESCAPE       0x81
#define TWO_BYTES_MASK          0xE0
#define KBC_WRITE_COMMAND       0x60
#define KBC_STATUS_REGISTER     0x64
#define KBC_OUT_COMMAND         0x60
#define KBC_IN_COMMAND          0x64
#define KBC_READ_COMMAND        0x20
#define WAIT_KBCONTROLLER       20000 //nome mais explicativo do que DELAY_US

#endif