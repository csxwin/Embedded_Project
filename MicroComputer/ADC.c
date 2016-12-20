#include <stdint.h>
#include "TIMER0.h"
#include "ADC.h"
#include "Nokia5110.h"

uint16_t ADC_Read = 0xFF;

void ADC_TIMER1B_Init(void)
{
	SYSCTL_RCGCADC_R |= 0x01;
	SYSCTL_RCGCTIMER_R |= 0x02; // timer 1 activate
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R & 0x00000010) == 0){};
	GPIO_PORTE_DIR_R &= ~0x04;
	GPIO_PORTE_DEN_R &= ~0x04; //enable PE3
	GPIO_PORTE_AFSEL_R |= 0x04;
	//GPIO_PORTE_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFFF0FFF) + 0x00000000;
	GPIO_PORTB_AMSEL_R |= 0x04;	
	
	TIMER1_CTL_R &= ~0x00000100;

	TIMER1_CFG_R = 0x00000004;   //16 bit		
	TIMER1_TBMR_R = 0x00000002;  //Periodic Mode
		
	TIMER1_TBILR_R = 0x0000C34F;	// with 0x63 and 0xC34F. the period is 0.1s
  TIMER1_TBPR_R = 0x63;	// During Timer Counter, TAPR act as Prescaler
	// (prescaler + 1) * (period + 1)
	//TIMER1_ICR_R |= 0x00000100;	
	//TIMER1_IMR_R |= 0x00000100;
	TIMER1_IMR_R &= ~0x00000F00;	 //do not arm any interrupt on TIMER1
	TIMER1_CTL_R |= 0x00002100;
	TIMER1_CTL_R |= 0x00000100;
		
	ADC0_ACTSS_R &= ~0x8;
	
  ADC0_SSPRI_R = 0x3210;		
	ADC0_PC_R = 0x1;
	ADC0_EMUX_R = (ADC0_EMUX_R&0x0FFF)+0x5000;
	ADC0_SSMUX3_R = 1;
	ADC0_SSCTL3_R = 0x6;
	ADC0_IM_R |= 0x8;
	ADC0_ISC_R |= 0x8;
	ADC0_ACTSS_R |= 0x8;
	
	NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFF00FF) + 0x00002000;	
	NVIC_EN0_R |= 0x01 << (17 - 0);	
	
	//NVIC_PRI5_R = (NVIC_PRI5_R & 0xFF00FFFF) + 0x00400000;	
	//NVIC_EN0_R |= 0x01 << (22 - 0);	
	
}

void ADC0Seq3_Handler(void)
{
	ADC0_ISC_R |= 0x8;
	ADC_Read = (ADC0_SSFIFO3_R*3300) / 0xFFF;
	//ADC_Read = ADC0_SSFIFO3_R;
	Nokia5110_SetCursor(0,4);
	Nokia5110_OutString("ADC0");
	Nokia5110_OutUDec(ADC_Read);
	Nokia5110_SetCursor(0,5);
	Nokia5110_OutUDec(ADC0_SSFIFO3_R);
}

void Timer1B_Handler(void)
{
	TIMER1_ICR_R |= 0x00000100;	
	Nokia5110_SetCursor(0,4);
	Nokia5110_OutString("TIMER1B");
}



