#include "UART.h"
#include "ioavr.h"
#include <intrinsics.h>

static UART_transmitter UART = {0, 0, 0};

// In UART protocol there can be problems with skew of level changing due to 
// data capture at half if bit interval
#pragma vector=TIMER1_COMPA_vect
__interrupt void UART_bit_interval(void)
{
  UART.bit_interval_passed = 1;
}

void UART_init(void)
{
  __disable_interrupt();
  //Configure TX pin as output with high logic state
  DDRB |= (1 << TX_pin);
  TX_HIGH;
  //    Calculate prescaler for T/C 0 and compare register value
  unsigned int temp = (MCU_frequency/UART_baud_rate);
  for (unsigned int i = 1;;i++)
  {
    static unsigned int prescaler = 1;
    if (temp/prescaler <= 255)
    {
      //TCCR1 = (1 << CTC1) | i; // set equivalent to prescaler value in T/C1 control reg 
      UART.prescaler_code = i;
      OCR1A = temp/prescaler; // store compare value in compare register
      OCR1C = OCR1A;
      break;
    }
    prescaler *= 2;
  }
}

void UART_send_byte(unsigned char byte)
{
  UART.data = byte;
  
  // Generate START 
  TX_LOW; //set TX pin to 0
  TCNT1 = 0x00; // purge T/C1 counter value
  TIMSK |= (1 << OCIE1A); //enable compare interrupt
  TCCR1 = (1 << CTC1) | (unsigned char)UART.prescaler_code;
  __enable_interrupt();
  while(UART.bit_interval_passed != 1);
  UART.bit_interval_passed = 0;
  
  // Send data byte
  while(UART.bit_to_send_number != 8)
  {
    if (UART.data & (1 <<  UART.bit_to_send_number)) TX_HIGH;
    else TX_LOW;
    UART.bit_to_send_number++;
    while(UART.bit_interval_passed != 1);
    UART.bit_interval_passed = 0;
  }
  UART.bit_interval_passed = 0;
  
  // Generate STOP
  TX_HIGH;
  while(UART.bit_interval_passed != 1);
  
  // Get everything to default
  __disable_interrupt();
  TCCR1 &= (1 << CTC1); // stop T/C1
  TIMSK &= ~(1 << OCIE1A);
  UART.bit_interval_passed = 0;
  UART.bit_to_send_number = 0;
}