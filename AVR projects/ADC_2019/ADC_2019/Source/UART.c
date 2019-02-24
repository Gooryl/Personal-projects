/*
 * UART.c
 *
 * Created: 22.02.2019 23:08:59
 *  Author: Юрий
 */ 

#include "../Header/UART.h"

void UART_init(void)
{
	//	Calculate prescaler for T/C 0 and compare register value
	unsigned int prescaler = MCU_frequency / (255 * UART_baud_rate);
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

void UART_send_byte(void)
{
	
}