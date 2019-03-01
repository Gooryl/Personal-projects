#include "main.h"
#include "UART.h"
#include "ioavr.h"
#include "USI_TWI_Master.h"

int main( void )
{
  // create 
  I2C_slave ADS;
  ADS.address = (ADS_address(0x00) << 1);
  ADS.control_reg = 0x90; // start a single conversation with 240sps with 1 gain
  ADS.message_buffer[0] = ADS.address;
  
  // Initialise peripheral
  UART_init();
  USI_TWI_Master_Initialise();
  
  // Start analog to digital convertation
  I2C_start_convertation(ADS);
  
  while(1)
  {
    do 
    {
      unsigned char old_data = I2C_check_status(ADS);
      if (!old_data) break;
    }while (1); // wait until convetation is completed
    
    // convert ADC output code to signed int value
    signed int value = convert_I2C_ADC_code_to_mV_value(ADS.message_buffer+1); // +1 cuz data in second byte
    if (value >= 10) // 10 mV threshold voltage
    {
      //send result via UART
    }
  }
  return 0;
}

void I2C_start_convertation(I2C_slave ADS)
{
  ADS.message_buffer[0] &= ~(1 << I2C_rw_bit);
  ADS.message_buffer[1] = ADS.control_reg;
  USI_TWI_Start_Transceiver_With_Data(ADS.message_buffer, 2);
}

unsigned char I2C_check_status(I2C_slave ADS)
{
  ADS.message_buffer[0] |= (1 << I2C_rw_bit);
  USI_TWI_Start_Transceiver_With_Data(ADS.message_buffer, 4);
  if (!(ADS.message_buffer[3] & (1 << ADC_complete_bit))) return 0; // there is new data
  else return 1; //convertation is not complete yet
}

signed int convert_I2C_ADC_code_to_mV_value(unsigned char* input_data_buffer)
{
  // due to internal GAIN = 1, the output code is equivalent to mV value
  return (*input_data_buffer << 8) | *(input_data_buffer+1);
}

unsigned char* convert_I2C_value_to_string(signed int value)
{
  static unsigned char output_buffer[5] = {0}; // maximum length is -2048
  signed int temp = value;
  if (value < 0)
  {
    output_buffer[0] = '-';
    temp = ~(value - 1);
  }
  else
    output_buffer[0] = ' ';
  for (unsigned char i = 4; i > 0; --i)
  {
    output_buffer[i] = (unsigned char)(temp % 10);
    temp /= 10;
  }
  return output_buffer;
}