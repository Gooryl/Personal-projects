/*
 * UART.h
 *
 * Created: 22.02.2019 23:08:45
 *  Author: Юрий
 */ 


#ifndef UART_H_
#define UART_H_

#define MCU_frequency 8000000 
#define UART_baud_rate 9600 //For 8MHz MCU clock all baud rates are supported (from 300 to 115200 bod)
#define TX_pin PB2

void UART_init(void); // Initialize UART hardware
void UART_send_byte(void);


#endif /* UART_H_ */