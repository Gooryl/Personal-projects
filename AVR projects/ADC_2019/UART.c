#include "UART.h"
#include "ioavr.h"
#include <intrinsics.h>

//!!!REWRITE WITH REFERENCE TO TIMER/COUNTER1//

static UART_transmitter UART = {0, 0, 0};

// In UART protocol there can be problems with skew of level changing due to 
// data capture at half if bit interval
#pragma vector=TIMER0_COMPA_vect
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
  unsigned int prescaler = (MCU_frequency / 255) / UART_baud_rate;
  unsigned char compare_reg_value = MCU_frequency / (prescaler * UART_baud_rate);
	
  //	Configure control registers
  //	Mode of timer operation - CTC (Clear Timer on Compare) with calculated prescaler
  TCCR0A = (1 << WGM01);
  TCCR0B = (1 << WGM02);
  if (prescaler == 0) 
  {
    prescaler = 1;
    TCCR0B |= (1 << CS00);		
  }
  if ((prescaler > 1) && (prescaler <= 8)) 
  {
    prescaler = 8;
    TCCR0B |= (1 << CS01);      
  }
  if ((prescaler > 8) && (prescaler <= 64)) 
  {
    prescaler = 64;
    TCCR0B |= (1 << CS01) | (1 << CS00);
  }
  if ((prescaler > 64) && (prescaler <= 256))
  {
    prescaler = 256;
    TCCR0B |= (1 << CS02);
  }
  if (prescaler > 256) 
  {
    prescaler = 1024;
    TCCR0B |= (1 << CS02) | (1 << CS00);
  }
         
  // Store compare match value in register
  OCR0A = compare_reg_value;
}

void UART_send_byte(unsigned char byte)
{
  UART.data = byte;
  
  // Generate START 
  TX_LOW; //set TX pin to 0
  TCNT0 = 0x00; // purge T/C0 counter value
  TIMSK |= (1 << OCIE0A); //enable compare interrupt
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
  TIMSK &= ~(1 << OCIE0A);
  UART.bit_interval_passed = 0;
  UART.bit_to_send_number = 0;
}