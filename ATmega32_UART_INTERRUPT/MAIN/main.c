/*
 * main.c
 *
 * Created: 12-May-25
 *  Author: PCP
 */ 

#include "clock_config.h"
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include<util/delay.h>
#include <string.h>
#include <stdio.h>

// the string we send and receive on UART
char test_string[] = "\r\nHello World!\r\n";
char test_string1[] = "Enter String\r\n";


int main(void)
{
	
	UART_Init(9600);
	sei();
	UART_Send_String(test_string);
	UART_Send_String(test_string1);
    while(1)
    {
        //TODO:: Please write your application code 
    }
}