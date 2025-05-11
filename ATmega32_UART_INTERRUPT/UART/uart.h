/*
 *uart.h
 *
 * Created: 12-May-25
 *  Author: P C Prusti
 */ 


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

extern void UART_Init(uint32_t baud_rate);

extern uint8_t UART_Receive_Char(void);
extern char* UART_Receive_String(void);

extern void UART_Send_Char(uint8_t ch);
extern void UART_Send_String(char *str);

extern void UART_Flush( void );

#endif /* UART_H_ */