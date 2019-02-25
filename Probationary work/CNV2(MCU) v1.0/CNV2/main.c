/*
 * CNV2.c
 *
 * Created: 13.02.2018 13:59:14
 * Author : y_bykovtsev
 */ 
#include "H_MAIN.h"
#include <ioavr.h>
#include <intrinsics.h>

//---------------------------Global variables and structs ---------------
struct ringBuff	uartRingBuff = {{0}, 0, 0}; //ring buffer struct
struct transmission uartTransmission = {TRANSMIT_OFF, WHAT_COMMAND, FALSE, FALSE, 0, 0, 0, {0}, 5}; //UART transmition struct
struct eeprom EEPROM = {0, {0}}; //EEPROM struct

//---------------------------Interrupts--------------------------------
#pragma vector = TIMER1_COMPA_vect
__interrupt void all_registers_transmit_timeout(void)
{
  TIMER1_OFF;
  TIMER1H_CLR;
  TIMER1L_CLR;
  uartRingBuff.pcurrent_read = uartRingBuff.ringBuf;
  uartRingBuff.pcurrent_write = uartRingBuff.ringBuf;
  uartTransmission.status = TRANSMIT_OFF;
  uartTransmission.parse = WHAT_COMMAND;
  uartTransmission.new_data = FALSE;
  EEPROM.address = 0;
  uartTransmission.current_register = 5;
  UART_send_message("TIMEOUT");
}

#pragma vector = TIMER0_COMP_vect
__interrupt void register_transmist_timeout(void)
{  
  TIMER0_OFF;
  TIMER0_CLR;
  uartRingBuff.pcurrent_read = uartRingBuff.ringBuf;
  uartRingBuff.pcurrent_write = uartRingBuff.ringBuf;
  uartTransmission.status = TRANSMIT_OFF;
  uartTransmission.parse = WHAT_COMMAND;
  uartTransmission.new_data = FALSE;
  EEPROM.address = 0;
  uartTransmission.current_register = 5;
  UART_send_message("TIMEOUT");
}

#pragma vector = USART1_RX_vect
__interrupt void receive_func(void)
{
	char byte = UDR1;
	if (uartTransmission.status == TRANSMIT_OFF)
	{
		if (byte == TERMINATOR) 
		{
			uartTransmission.status = TRANSMIT_ON;
                        TIMER0_ON;
		}
		else 
		{
			uartTransmission.status = ERROR;
		}
	}
	else if (uartTransmission.status == TRANSMIT_ON)
	{
		if (byte == TERMINATOR)
		{
			uartTransmission.status = TRANSMIT_OFF;
			uartTransmission.new_data = TRUE;
                        TIMER0_OFF;
                        TIMER0_CLR;
		}
		else 
		{
			put_data_ring(byte);
		}
	}
}

/***********************Main part*********************************/
int main(void)
{
        TIMER0_init();
        TIMER1_init();
	UART_init();
	SPI_master_init();
	LE_init();
	LD_init();

        for (int i = 0; i < 24; i++)
	{
          EEPROM.data[i] = EEPROM_read(EEPROM.address);
          EEPROM.address++;
	}
        EEPROM.address = 0;
	
        unsigned char mainTempCrc8 = Crc8(EEPROM.data, 24);
        unsigned char tempHash = EEPROM_read(HASH_ADDRESS);
        
	if (mainTempCrc8 == tempHash)
	{
		for (int i = 0; i < 4; i++)
		{
			R5[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
		for (int i = 0; i < 4; i++)
		{
			R4[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
		for (int i = 0; i < 4; i++)
		{
			R3[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
		for (int i = 0; i < 4; i++)
		{
			R2[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
		for (int i = 0; i < 4; i++)
		{
			R1[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
		for (int i = 0; i < 4; i++)
		{
			R0[i] = EEPROM.data[EEPROM.address];
			EEPROM.address++;
		}
	}
        EEPROM.address = 0;
	
	SPI_send_register(R5);
	SPI_send_register(R4);
	SPI_send_register(R3);
	SPI_send_register(R2);
	SPI_send_register(R1);
	SPI_send_register(R0);
        
	asm ("sei");
        while (1) 
        {
		__delay_cycles(80000);
		asm ("cli");
		
		if ((uartTransmission.status != ERROR) && (uartTransmission.new_data == TRUE)) //if no errors during transmition and data is new, start parsing
		{
                        TIMER1_ON;
			switch (uartTransmission.parse)
			{
				char temp;
				case WHAT_COMMAND:
					temp = get_data_ring();
					if ((temp == 0) || (temp == 1)) 
					{
						uartTransmission.command_number = temp;
						uartTransmission.parse = WHAT_ADDRESS;
					}
					else
					{
						uartTransmission.status = ERROR;
					}
					break;
					
				case WHAT_ADDRESS:
					temp = get_data_ring();
					if (temp == uartTransmission.current_register)
					{
						uartTransmission.address_number = temp;
						uartTransmission.parse = HOW_BYTES;
                                                uartTransmission.current_register--;
					}
					else
					{
						uartTransmission.status = ERROR;
					}
					break;
					
				case HOW_BYTES:
					temp = get_data_ring();
					if (temp != 4)
					{
						uartTransmission.status = ERROR;
					}
					else
					{
						uartTransmission.bytes_number = 4;
						uartTransmission.parse = WHAT_DATA;
					}
					break;
					
				case WHAT_DATA:
					for (int i = 0; i < uartTransmission.bytes_number; i++)
					{
						uartTransmission.data[i] = get_data_ring();
                                                EEPROM.data[EEPROM.address] = uartTransmission.data[i];
						EEPROM.address++;						
					}
					if (uartRingBuff.pcurrent_read != uartRingBuff.pcurrent_write)
					{
						uartTransmission.status = ERROR;
						break;
					}
					uartTransmission.parse = DONE;
					
					break;
					
				case DONE:
					//if parsing is succeeded (comand, address, number of bytes and data), send data through SPI
					//then: pointers pcurrent_read and pcurrent_write are in the start of buffer
					//point, that the data is old to prevent further processing
					//getting ready for new transmition, setting parsing state to origin
					SPI_send_register(uartTransmission.data);
					
					if (uartTransmission.address_number == 0)
					{
						UART_send_message("ACK");
						
						__delay_cycles(16000);
						if (PINB & (1<<LD))
						{
							UART_send_message("SUCCESS");
                                                        EEPROM.address = 0;
                                                        for (int i = 0; i < 24; i++)
                                                        {
                                                          EEPROM_write(EEPROM.address, EEPROM.data[i]);
                                                          EEPROM.address++;
                                                        }
                                                        EEPROM.address = 0;
                                                        unsigned char tempCrc8 = Crc8(EEPROM.data, 24);
                                                        EEPROM_write(HASH_ADDRESS, tempCrc8);
                                                }
						else
						{
							UART_send_message("FAIL");
						} 
                                                
						uartTransmission.parse = WHAT_COMMAND;
						uartTransmission.new_data = FALSE;
                                                uartTransmission.current_register = 5;
                                                TIMER1_OFF;
                                                TIMER1H_CLR;
                                                TIMER1L_CLR;
						break;
					}					
					
					uartTransmission.answer_inquiry = TRUE;
					uartTransmission.parse = WHAT_COMMAND;
					uartTransmission.new_data = FALSE;
					break;
			}			
		}
		
		if (uartTransmission.status == ERROR) //if an error occuried, then set pointer to the start of buffer, transmition ends, data is old 
		{
                        TIMER1_OFF;
                        TIMER1H_CLR;
                        TIMER1L_CLR;
			uartRingBuff.pcurrent_read = uartRingBuff.ringBuf;
			uartRingBuff.pcurrent_write = uartRingBuff.ringBuf;
			uartTransmission.status = TRANSMIT_OFF;
			uartTransmission.parse = WHAT_COMMAND;
			uartTransmission.new_data = FALSE;
			EEPROM.address = 0;
                        uartTransmission.current_register = 5;
			UART_send_message("NACK");		
		}
		
		if (uartTransmission.answer_inquiry == TRUE)
		{
			UART_send_message("ACK");
			uartTransmission.answer_inquiry = FALSE;
		}
				
		asm ("sei");
    }
    return 0;
}

//----------------------------Function's bodies--------------------
/**************TIMER/COUNTER***************/
void TIMER0_init(void)
{
  TCCR0A = (1 << WGM01);
  OCR0A = 94; //with prescale at 1024
  TIMSK0 = (1 << OCIE0A);
}

void TIMER1_init(void)
{
  TCCR1B = (1 << WGM12);
  OCR1AH = 0x3D;
  OCR1AL = 0x08;
  TIMSK1 = (1 << OCIE1A);
}
//-----------------UART---------------------
void UART_init (void) //enable the transmition and receiving, enable interrupts, 1 stop bit, no parity, 9600 baud
{
	UCSR1B = (1 << RXCIE1) | (1 << TXCIE1) | (1 << RXEN1) | (1 << TXEN1);
	UBRR1L = BAUDRATE_9600;
	uartRingBuff.pcurrent_read = uartRingBuff.ringBuf;
	uartRingBuff.pcurrent_write = uartRingBuff.ringBuf;
}

void put_data_ring (char data)
{
	*uartRingBuff.pcurrent_write = data;
	char * temp = uartRingBuff.ringBuf;
	if ((uartRingBuff.pcurrent_write - temp) == (RING_BUF_SIZE - 1)) //check if reached buffer end
	{
		uartRingBuff.pcurrent_write = uartRingBuff.ringBuf;			
	}
	else
	{
		uartRingBuff.pcurrent_write++;
	}
}

char get_data_ring(void)
{
	char data;
	data = *uartRingBuff.pcurrent_read;
	char * temp = uartRingBuff.ringBuf;
	if ((uartRingBuff.pcurrent_read - temp) == (RING_BUF_SIZE - 1))
	{
		uartRingBuff.pcurrent_read = uartRingBuff.ringBuf;		
	}
	else
	{
		uartRingBuff.pcurrent_read++;
	}
	return data;
}

void UART_send_byte(char byte)
{
	UDR1 = byte;
	while ( ! (UCSR1A & (1<<UDRE1) ) ); //while data register is not empty
}

void UART_send_message (char * p_string)
{
	while (*p_string) UART_send_byte(*p_string++);
}

//-----------------SPI---------------------
void SPI_master_init(void)//prescaler at 16, SCL in LOW during data change, sampling at rising edge, MSB sends first
{
	DDR_SPI = (1<<MOSI) | (1<<SCK) | (1<<SS);
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

void SPI_send_byte(unsigned char byte)
{
	SPDR = byte;
	while (!(SPSR & (1<<SPIF)));
}

void SPI_send_register(unsigned char* abyte)
{
	LE_OFF;
	for (int i = 0; i < 4; i++)
	{
		SPI_send_byte(*(abyte+i));	
	}
	LE_ON;
}

/**********************EEPROM**********************/
void EEPROM_write (unsigned int uiAddress, unsigned char ucData)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address and data registers */
	EEAR = uiAddress;
	EEDR = ucData;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}

unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE));
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}

/***************Proccessing functions******************/
void LE_init (void)
{
	DDRB |= 1 << LE;
	PORTB |= 1 << LE;
}

void LD_init (void)
{
	PORTB |= 1 << LD;
}

unsigned char Crc8(unsigned char *pcBlock, unsigned int len)
{
	unsigned char crc = 0xFF;
	unsigned int i;

	while (len--)
	{
		crc ^= *pcBlock++;

		for (i = 0; i < 8; i++)
		crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
	}

	return crc;
}