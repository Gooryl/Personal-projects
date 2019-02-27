#include "I2C.h"
#include "ioavr.h"
#include <intrinsics.h>

static I2C_slave An_to_Dig_Con;

#pragma vector=TIMER0_COMPA_vect
__interrupt void I2C_bit_interval(void)
{
  
}

void I2C_init(void)
{
  __disable_interrupt();
  // Due to using an internal clock - T/C0 - initialize it with desired speed
  
}