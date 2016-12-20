// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// December 28, 2014
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
//#include "tm4c123gh6pm.h"

//Constant declarations to access port registers using
//symbolic names instead of addresses
#define GPIO_PORTE_DATA_R              (*((volatile unsigned long*)0x400243FC))
#define SYSCTL_RCGC2_R                 (*((volatile unsigned long *)0x400FE108))
#define GPIO_PORTE_DIR_R               (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R                   (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R                   	 (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R                     (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R              (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R                (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R             (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R              (*((volatile unsigned long *)0x4002452C))



// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void PortE_Init(void); //initial port E.
int Deactivate(void); // Disable interrupts
int activate(void);  // Enable interrupts
int Alarm(void);
int detection(void);
void Delay1ms(unsigned long msec);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  //TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  int step = 0;
	PortE_Init();
	while(1){
	switch(step)
	{
		case 0: 
			step = activate();
			break;
		case 1:
			step = detection();
		  Delay1ms(1);	
		  break;
		case 2:
			step = Alarm();
			
			break;
	}
    
}
}

void PortE_Init(void){volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DIR_R |= 0x18;
	GPIO_PORTE_DIR_R &= ~0x07;
	GPIO_PORTE_AFSEL_R &= 0x00;
	GPIO_PORTE_PUR_R &= 0x00;
	GPIO_PORTE_DEN_R |= 0x1F;
	GPIO_PORTE_AMSEL_R &= 0x00;
	GPIO_PORTE_PCTL_R &= 0x00000000;
}
	
int activate(void)
{
	if(!(GPIO_PORTE_DATA_R & 0x04)) //no push
	{
		Delay1ms(1);
		return 0;
	}
	else{
		GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R | 0x08; //detec press button. 
		Delay1ms(2000);          //give 5 seconds for release
		return 1;
	}
		
}  
	
int Deactivate(void)
{
	if(!(GPIO_PORTE_DATA_R & 0x04))  //no push
	{
		return 0;
	}
	else{
		GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R & ~0x18;
		Delay1ms(2000);
		return 1;  //disable the alarm.
	}
}

int Alarm(void)
{
	while(!Deactivate())
	{
		GPIO_PORTE_DATA_R = GPIO_PORTE_DATA_R ^ 0x10;
		Delay1ms(100);	
	}
	return 0;

}


int detection(void)
{		
	if(!Deactivate())
	{
		if((GPIO_PORTE_DATA_R>>1 & 0x01) || (GPIO_PORTE_DATA_R & 0x01))
		{
			return 2; //go to alarm
		}
		else{
			return 1; //go back to detection stage
		}
	}
	else{ // disable alarm
		return 0;
	}
}

void Delay1ms(unsigned long msec)
{
	unsigned long time;
	time = 4333 * msec;
	while(time)
	{
		time --;
	}
}

