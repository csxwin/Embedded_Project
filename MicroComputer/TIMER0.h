#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))

#define SYSCTL_RCGCTIMER_R      (*((volatile unsigned long *)0x400FE604))

#define PB6                     (*((volatile unsigned long *)0x40005100))
#define PB4                     (*((volatile unsigned long *)0x40005040))

#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
	
#define GPIO_PORTB_PDR_R        (*((volatile unsigned long *)0x40005514))

#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_IMR_R            (*((volatile unsigned long *)0x40030018))
#define TIMER0_RIS_R            (*((volatile unsigned long *)0x4003001C))
#define TIMER0_MIS_R            (*((volatile unsigned long *)0x40030020))
#define TIMER0_ICR_R            (*((volatile unsigned long *)0x40030024))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TAPR_R           (*((volatile unsigned long *)0x40030038))	
#define TIMER0_TAMATCHR_R       (*((volatile unsigned long *)0x40030030))

#define TIMER0_TBMR_R           (*((volatile unsigned long *)0x40030008))
#define TIMER0_TBILR_R          (*((volatile unsigned long *)0x4003002C))
#define TIMER0_TBPR_R           (*((volatile unsigned long *)0x4003003C))
#define TIMER0_TBR_R            (*((volatile unsigned long *)0x4003004C))

#define TIMER1_CFG_R            (*((volatile unsigned long *)0x40031000))
#define TIMER1_TAMR_R           (*((volatile unsigned long *)0x40031004))
#define TIMER1_CTL_R            (*((volatile unsigned long *)0x4003100C))
#define TIMER1_IMR_R            (*((volatile unsigned long *)0x40031018))
#define TIMER1_RIS_R            (*((volatile unsigned long *)0x4003101C))	
#define TIMER1_ICR_R            (*((volatile unsigned long *)0x40031024))
#define TIMER1_TAILR_R          (*((volatile unsigned long *)0x40031028))

#define TIMER1_TBMR_R           (*((volatile unsigned long *)0x40031008))
#define TIMER1_TBILR_R          (*((volatile unsigned long *)0x4003102C))
#define TIMER1_TBPR_R           (*((volatile unsigned long *)0x4003103C))


#define NVIC_PRI4_R             (*((volatile unsigned long *)0xE000E410))
#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))
#define TAEVENT 0x0C;

extern uint16_t freq;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void TIMER0_Input_Capture(void);

void TIMER1_Input_Capture(void);

void PeriodMeasure_Init(void);

void TIMER0_PWM_Init(void);

void TIMER0_PWM_DUTY(void);

void FreqMeasure_Timer0_Init(void);
