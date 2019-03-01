#ifndef MAIN_H_
#define MAIN_H_

#define ADS_address(aaa) (0x48 | (aaa)) //addr is 0b1001aaa 
#define I2C_rw_bit 0
#define ADC_complete_bit 7

typedef struct I2C_slave
{
  unsigned char address;
  unsigned char control_reg;
  unsigned char message_buffer[4];
}I2C_slave;

void I2C_start_convertation(I2C_slave);
unsigned char I2C_check_status(I2C_slave);
signed int convert_I2C_ADC_code_to_mV_value(unsigned char* );
unsigned char* convert_I2C_value_to_string(signed int);

#endif