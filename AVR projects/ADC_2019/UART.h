#ifndef UART_H_
#define UART_H_

#define MCU_frequency 8000000 
#define UART_baud_rate 9600 //9600 work fine? else doesn't tested
#define TX_pin PB3
#define TX_HIGH (PORTB |= (1 << TX_pin)) 
#define TX_LOW (PORTB &= ~(1 << TX_pin))

typedef struct UART_transmitter
{
  unsigned char prescaler_code;
  unsigned char bit_to_send_number;
  unsigned char data;
  volatile unsigned char bit_interval_passed;
} UART_transmitter;

void UART_init(void); // Initialize UART hardware
void UART_send_byte(unsigned char);


#endif /* UART_H_ */