/*
 * I2C.h
 *
 * Created: 16.02.2019 16:28:22
 *  Author: Юрий
 */ 


#ifndef I2C_H_
#define I2C_H_

#define I2C_data_write_register USIDR
#define I2C_data_read_register USIBR

#define I2C_enable_DDR_bits (1 << DDB0) | (1 << DDB2)
#define I2C_enable_PORT_bits (0 << PB0) | (1 << PB2)
#define I2C_configuration_bits (1 << USIWM1) | (0 << USIWM0) | (0 << USICS1) | (0 << USICS1)

// Define functions
void init_I2C_bus(void);
void init_I2C_clock_source(void);



#endif /* I2C_H_ */