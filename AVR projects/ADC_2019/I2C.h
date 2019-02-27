#ifndef I2C_H_
#define I2C_H_

typedef struct I2C_slave
{
  unsigned char address;
  unsigned char controll_reg;
  unsigned char data_buffer[3];
  unsigned char bit_interval_passed;
}I2C_slave;

#define MCU_frequency 8000000
#define I2C_speed 100000

void I2C_init(void);

#endif