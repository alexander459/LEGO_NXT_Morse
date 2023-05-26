#ifndef   _I2C_H
#define   _I2C_H

extern UBYTE volatile invalid_recv_buffer;

enum power_t {
  NORMAL_OP = 0x0000,
  POWERDOWN = 0x5A00,
  REPROGRAM = 0xA55A
};

void I2CInit(void);
void I2CExit(void);
void I2CTransfer(void);
void I2CCtrl(enum power_t);

#endif
