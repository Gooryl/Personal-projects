/*
 * I2C.c
 *
 * Created: 17.02.2019 0:45:29
 *  Author: Юрий
 */ 
#include "../Header/I2C.h"

void init_I2C_bus(void)
{
	DDRB = I2C_enable_DDR_bits;
	PORTB = I2C_enable_PORT_bits;
	
}

void init_I2C_clock_source(void)
{
	
}