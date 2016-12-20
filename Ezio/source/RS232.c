/*
This file is created to do RS232 communication from TM4C123.
Arthur: Shuxun Cao
*/
#include <stdint.h>
#include "RS232.h"

#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))

#define SYSCTL_RCGCUART_R       (*((volatile unsigned long *)0x400FE618))
#define UART1_DR_R              (*((volatile unsigned long *)0x4000D000))
#define UART1_RSR_R             (*((volatile unsigned long *)0x4000D004))
#define UART1_FR_R              (*((volatile unsigned long *)0x4000D018))
#define UART1_IBRD_R            (*((volatile unsigned long *)0x4000D024))
#define UART1_FBRD_R            (*((volatile unsigned long *)0x4000D028))
#define UART1_LCRH_R            (*((volatile unsigned long *)0x4000D02C))
#define UART1_CTL_R             (*((volatile unsigned long *)0x4000D030))



#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))

#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *)0x40005510))
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))

#define SYSCTL_RCGC1_R          (*((volatile uint32_t *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile uint32_t *)0x400FE108))


#define UART_FR_TXFE            0x80;
#define UART_FR_RXFF            0x40;
#define UART_FR_TXFF            0x20;
#define UART_FR_RXFE            0x10;
#define UART_FR_BUSY            0x08;


static void delay1ms (uint32_t ms)
{
	uint32_t volatile time;
	while(ms){
		time = 72724*2/91*5/8;
		while(time){
			time --;
		}
		ms--;
	}
}

void UART1_Init(void){volatile uint32_t delay;

	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};

	GPIO_PORTB_DIR_R |= 0x04; //Only set bit for PB2, don't set bit for PB0, PB1
	GPIO_PORTB_AMSEL_R &= ~0x07; 
	GPIO_PORTB_PCTL_R |= 0x00000011;
	GPIO_PORTB_PCTL_R &= ~0x00000F00;
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_AFSEL_R &= ~0x04;
	GPIO_PORTB_DEN_R |= 0x07;
  
  GPIO_PORTB_PUR_R |= 0x04; //PB2 will be pull-ed down. RE low and DE low.	
		
	SYSCTL_RCGCUART_R |= 0x02; // unlock UART1
	delay = SYSCTL_RCGCUART_R;
	UART1_CTL_R &= ~0x0001;	
	UART1_RSR_R = 0x00000000;  // clean the error bits
	UART1_IBRD_R = 0x0145;
	UART1_FBRD_R = 0x0021;

	//baud rate is (Bus clk Freq(/(16*divider)
	//PLL 50MHz, divider is 325.521. 9600Hz
	//0101000101.100001
		
	UART1_LCRH_R = 0x70;
	//only after writting LCRH can system updates the baud rate.
	UART1_CTL_R = 0x0301;

}

void UART1_TX(uint8_t data_out){
	//while((UART1_FR_R & 0x80) == 0){};
	
	DR = DE;
	delay1ms(1);
	while((UART1_FR_R & 0x20) != 0){};
	UART1_DR_R = (UART1_DR_R & 0xF0) + data_out;
	while((UART1_FR_R & 0x08) == 0x08){};
}

void UART1_Out_String(char *ptr){
	while(*ptr){
		UART1_TX((int)*ptr);
		ptr++;
	}
}

uint8_t UART1_RX(void){
	uint8_t data_in;
	
	DR = RE;
	
	delay1ms(1);
	
	while((UART1_FR_R & 0x10) == 0x10){};
	data_in = UART1_DR_R & 0x00FF;
	return data_in;
}

void RS485_DR(void)
{
	DR = RE;
}







