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
      unsigned char old_data = I2C_check_status(ADS); //send data if data new
      if (!old_data) break;
    }while (1); // wait until convetation is completed
     
    I2C_start_convertation(ADS);
  }
  return 0;
}

void I2C_start_convertation(I2C_slave ADS)
{
  ADS.message_buffer[address_byte] &= ~(1 << I2C_rw_bit); // write operation
  ADS.message_buffer[fdata_byte] = ADS.control_reg;
  USI_TWI_Start_Transceiver_With_Data(ADS.message_buffer, 2);
}

unsigned char I2C_check_status(I2C_slave ADS)
{
  ADS.message_buffer[address_byte] |= (1 << I2C_rw_bit); // read operation
  USI_TWI_Start_Transceiver_With_Data(ADS.message_buffer, 4);
  if (!(ADS.message_buffer[control_reg_byte] & (1 << ADC_complete_bit))) 
  {
    UART_send_byte('a');
    UART_send_byte('d');
    UART_send_byte('c');
    UART_send_byte('=');
    UART_send_byte(ADS.message_buffer[fdata_byte]);
    UART_send_byte(ADS.message_buffer[sdata_byte]);
    UART_send_byte(0x0D); // CR
    UART_send_byte(0x0A); // LF 
    return 0; // there is new data
  }
  else return 1; //convertation is not complete yet
}