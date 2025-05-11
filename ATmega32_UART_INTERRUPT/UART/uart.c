/*
 * uart.c
 *
 * Created: 12-May-25
 *  Author: P C Prusti
 */ 

#include "clock_config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

//#include "defs.h"
/**
 * \def BUFFER_SIZE
 * \brief The size of the UART buffer
 */
#define BUFFER_SIZE 50

#include "circularbuffer.h"
// buffers for use with the ring buffer (belong to the UART)
uint8_t out_buffer[BUFFER_SIZE];
uint8_t in_buffer[BUFFER_SIZE];

//! ring buffer to use for the UART transmission
circular_buffer_t circular_buffer_out;
//! ring buffer to use for the UART reception
circular_buffer_t circular_buffer_in;

#define UBRR(USART_BAUDRATE) (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void UART_Init(uint32_t baud_rate)
{
	UBRRL = UBRR(baud_rate);			/* Load lower 8-bits of the baud rate value */
	UBRRH = (UBRR(baud_rate) >> 8);	/* Load upper 8-bits*/
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1<<RXCIE) | (1 << UDRIE);			/* Turn on transmission and reception */
	UCSRC |= (1<<URSEL) | (1 << UCSZ0) | (1 << UCSZ1);		/* Use 8-bit character sizes */
	
	// initialize the in and out buffer for the UART
	circular_buffer_init(&circular_buffer_out, out_buffer, BUFFER_SIZE);
	circular_buffer_init(&circular_buffer_in, in_buffer, BUFFER_SIZE);
}

void UART_Send_Char(uint8_t data)
{
		// Put data in buffer
		circular_buffer_put(&circular_buffer_out, data);
}


uint8_t UART_Receive_Char()
{
	uint8_t data = 0;
	uint8_t ret = 0;
	
	ret = circular_buffer_get(&circular_buffer_in, &data);
	if(1 == ret)
	{
		return data;
	}
	else
	{
		return data;
	}
}

char* UART_Receive_String()
{
	return ((char*)circular_buffer_in.buffer);
}

void UART_Send_String(char *str)
{
	while ('\0' != (*str))						/* Send string till null */
	{
		UART_Send_Char(*(str++));
	}
}


ISR(USART_UDRE_vect)
{
	uint8_t data = 0;
	uint8_t ret = 0;
	
	ret = circular_buffer_get(&circular_buffer_out, &data);
	if(1 == ret)
	{
		UDR = data;
	}
	
}


ISR(USART_RXC_vect)
{
	char data = UDR;
	circular_buffer_put(&circular_buffer_in, data);
	circular_buffer_put(&circular_buffer_out, data);
	
}

void UART_Flush( void )
{
	uint8_t dummy;
	while ( UCSRA & (1<<RXC) )
	{
		dummy = UDR;
	}
	(void)dummy;
}
