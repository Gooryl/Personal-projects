/*
 * H_MAIN.h
 *
 * Created: 13.02.2018 14:13:10
 *  Author: y_bykovtsev
 */ 


#ifndef H_MAIN_H_
#define H_MAIN_H_

#define TIMER0_ON TCCR0A |= (1 << CS02) | (1 << CS00)
#define TIMER0_OFF TCCR0A &= ~((1 << CS02) | (1 << CS00))
#define TIMER0_CLR TCNT0 = 0

#define TIMER1_ON TCCR1B |= (1 << CS12) | (1 << CS10)
#define TIMER1_OFF TCCR1B &= ~((1 << CS12) | (1 << CS10))
#define TIMER1H_CLR TCNT1H = 0
#define TIMER1L_CLR TCNT1L = 0

#define TRUE 1
#define FALSE 0

#define RING_BUF_SIZE 7
#define F_CPU 8000000
#define BAUDRATE_9600 51
#define MOSI 2
#define SCK 1
#define SS 0
#define LE 4
#define LD 5
#define DDR_SPI DDRB

#define LE_ON PORTB|=(1 << LE)
#define LE_OFF PORTB&=~(1 << LE)

#define TERMINATOR 0x3A //sign of transmition's start and end (:)
#define HASH_ADDRESS 25 //address of HASH in EEPROM

//-------------------------Data types---------------------------

enum status
{
	TRANSMIT_OFF,
	TRANSMIT_ON,
	ERROR   
};

enum parsing
{
	WHAT_COMMAND,
	WHAT_ADDRESS,
	HOW_BYTES,
	WHAT_DATA,
	DONE	
};

struct ringBuff
{
	char ringBuf[RING_BUF_SIZE];
	char* pcurrent_write;
	char* pcurrent_read;
};

struct transmission
{
	enum status status;
	enum parsing parse;
	char new_data;
	char answer_inquiry;
	char command_number;
	char address_number;
	char bytes_number;
	unsigned char data[4];
        unsigned int current_register;
};

struct eeprom
{
	unsigned int address;
	unsigned char data[24];
};

//2342.5 MHz settings
unsigned char R0 [] = {0x01, 0xD4, 0x80, 0x00};
unsigned char R1 [] = {0x08, 0x00, 0x80, 0x11};
unsigned char R2 [] = {0x19, 0x00, 0xBF, 0xC2};
unsigned char R3 [] = {0x00, 0x60, 0x04, 0xB3};
unsigned char R4 [] = {0x00, 0x81, 0x40, 0x3C};
unsigned char R5 [] = {0x00, 0x58, 0x00, 0x05};

//------------------------Function prototypes----------------------

/*******TIMER/COUNTER********/
void TIMER0_init(void);
void TIMER1_init(void);

//------------UART-------------
void UART_init(void);
void put_data_ring(char);
char get_data_ring(void);
void UART_send_byte(char);
void UART_send_message(char*);

//------------SPI-------------
void SPI_master_init(void);
void SPI_send_byte(unsigned char);
void SPI_send_register(unsigned char*);

/**********EEPROM***********/
void EEPROM_write(unsigned int, unsigned char);
unsigned char EEPROM_read(unsigned int);

/******Proccessing functions****/
void LE_init(void);
void LD_init(void);
unsigned char Crc8(unsigned char *, unsigned int);

#endif /* H_MAIN_H_ */