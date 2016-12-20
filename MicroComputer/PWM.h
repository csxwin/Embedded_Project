#include <stdint.h>

#define SYSCTL_RCGCPWM_R        (*((volatile unsigned long *)0x400FE640))
#define SYSCTL_RCC_R            (*((volatile unsigned long *)0x400FE060))

#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
	
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))

#define PWM0_0_CTL_R            (*((volatile unsigned long *)0x40028040))
#define PWM0_0_GENA_R           (*((volatile unsigned long *)0x40028060))
#define PWM0_0_LOAD_R           (*((volatile unsigned long *)0x40028050))	
#define PWM0_0_CMPA_R           (*((volatile unsigned long *)0x40028058))
#define PWM0_ENABLE_R           (*((volatile unsigned long *)0x40028008))

void PWM0_Init(uint16_t period, uint16_t duty);
