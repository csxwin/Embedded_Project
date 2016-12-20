//Created by Shuxun Cao to implement an input capture on PB6.
//when there is an rising edge on PB6, an interrupt will happen.
#include <stdint.h>
#include "TIMER0.h"
#include "Nokia5110.h"
#include "PLL.h"
#include "RS232.h"
#include "ST7735.h"

#define PF2                     (*((volatile uint32_t *)0x40025010))

uint32_t Period;              // (1/clock) units
uint32_t First;               // Timer0A first edge
int32_t Done;                // set each rising
uint32_t High;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void TIMER0_Input_Capture(void)
{
	SYSCTL_RCGCTIMER_R |= 0x01; // timer 0 activate
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
	GPIO_PORTB_DIR_R &= ~0x40;
	GPIO_PORTB_DEN_R |= 0x40; //enable PB6
	GPIO_PORTB_AFSEL_R |= 0x40;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xF0FFFFFF) + 0x07000000;
	GPIO_PORTB_AMSEL_R &= ~0x40;	
		
	TIMER0_CTL_R &= ~0x00000001;
	TIMER0_CTL_R = TIMER0_CTL_R	& ~TAEVENT ;
	TIMER0_CFG_R = 0x00000004;   //Input capture mode		
	TIMER0_TAMR_R = 0x00000007;  //Edge Time mode. Input Capture mode

	TIMER0_TAILR_R |= 0x000FFFF;	//????
		
	TIMER0_IMR_R |= 0x00000004;	
	TIMER0_ICR_R |= 0x00000004;	
	TIMER0_CTL_R |= 0x00000001;
		
	NVIC_PRI4_R = NVIC_PRI4_R & 0x00FFFFFF + 0x20000000;	
	NVIC_EN0_R |= 0x01 << (19 - 0);		
}

void TIMER1_Input_Capture(void)
{
	SYSCTL_RCGCTIMER_R |= 0x02; // timer 0 and 1 activate
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
	GPIO_PORTB_DIR_R &= ~0x10;
	GPIO_PORTB_DEN_R |= 0x10; //enable PB6
	GPIO_PORTB_AFSEL_R |= 0x10;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xFFF0FFFF) + 0x00070000;
	GPIO_PORTB_AMSEL_R &= ~0x10;	
		
	TIMER1_CTL_R &= ~0x00000001;
	TIMER1_CTL_R = TIMER0_CTL_R	& ~TAEVENT ;
	TIMER1_CFG_R = 0x00000004;   //Input capture mode		
	TIMER1_TAMR_R = 0x00000007;  //Edge Time mode. Input Capture mode

	TIMER1_TAILR_R |= 0x000FFFF;	//????
		
	TIMER1_IMR_R |= 0x00000004;	
	TIMER1_ICR_R |= 0x00000004;	
	TIMER1_CTL_R |= 0x00000001;
		
	NVIC_PRI4_R = NVIC_PRI4_R & 0xFFFF00FF + 0x00008000;	
	NVIC_EN0_R |= 0x01 << (21 - 0);		
}

void TIMER0_PWM_Init(void)
{
	SYSCTL_RCGCTIMER_R |= 0x01; // timer 0 and 1 activate
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
	GPIO_PORTB_DEN_R |= 0x40; //enable PB6
	GPIO_PORTB_AFSEL_R |= 0x40;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xF0FFFFFF) + 0x07000000;
	GPIO_PORTB_AMSEL_R &= ~0x40;	
		
	TIMER0_CTL_R &= ~0x00000001;
	TIMER0_CTL_R |= 0x00000040;	
	TIMER0_CFG_R = 0x00000004;   //16 bit		
	TIMER0_TAMR_R = 0x0000000A;  //Edge Time mode. Input Capture mode

	TIMER0_TAILR_R = 0x00009FFF;	//????
  TIMER0_TAMATCHR_R = 0x00000F00;
		
	TIMER0_CTL_R |= 0x00000001;	
}

void TIMER0_PWM_DUTY(void)
{
	TIMER0_TAMATCHR_R = TIMER0_TAILR_R - 0x0007FFF;
}

void FreqMeasure_Timer0_Init(void)
{
	SYSCTL_RCGCTIMER_R |= 0x01; // timer 0 activate
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
	GPIO_PORTB_DIR_R &= ~0x80;
	GPIO_PORTB_DEN_R |= 0x80; //enable PB6
	GPIO_PORTB_AFSEL_R |= 0x80;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0x0FFFFFFF) + 0x70000000;
	GPIO_PORTB_AMSEL_R &= ~0x80;	
		
	TIMER0_CTL_R &= ~0x00000001;

	TIMER0_CFG_R = 0x00000004;   //16 bit		
	TIMER0_TAMR_R = 0x00000002;  //Periodic Mode
  
	TIMER0_TBMR_R = 0x00000003;
	TIMER0_TBILR_R = 0xFFFF;	
	TIMER0_TBPR_R = 0xFF;
	TIMER0_IMR_R &= ~0x0700;
		
	TIMER0_TAILR_R = 0x0000C34F;	//
  TIMER0_TAPR_R = 0x63;	// During Timer Counter, TAPR act as Prescaler
	// (prescaler + 1) * (period + 1)
		
	TIMER0_IMR_R |= 0x00000001;	
	TIMER0_ICR_R |= 0x00000001;	
	TIMER0_CTL_R |= 0x00000101;
		
	NVIC_PRI4_R = NVIC_PRI4_R & 0x00FFFFFF + 0x40000000;	
	NVIC_EN0_R |= 0x01 << (19 - 0);	
}





