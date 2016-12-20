#include "PWM.h"

void PWM0_Init(uint16_t period, uint16_t duty)
{
	SYSCTL_RCGCPWM_R |= 0x01; // activate PWM0 ???Only have two module?
	SYSCTL_RCGCGPIO_R |= 0x02;
	while((SYSCTL_PRGPIO_R & 0x00000002) == 0){};
		
	GPIO_PORTB_DEN_R |= 0x40; //enable PB6
	GPIO_PORTB_AFSEL_R |= 0x40;
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R & 0xF0FFFFFF) + 0x04000000;
	GPIO_PORTB_AMSEL_R &= ~0x40;	
		
	SYSCTL_RCC_R = 0x00100000 | (SYSCTL_RCC_R & ~0x000E0000);	
	
	PWM0_0_CTL_R = 0;	
	PWM0_0_GENA_R = 0xC8;
  PWM0_0_LOAD_R = period;
  PWM0_0_CMPA_R = duty;
 	PWM0_ENABLE_R	= 0x01;
  PWM0_0_CTL_R = 1;
}

