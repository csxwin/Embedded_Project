// PLLTestMain.c
// Runs on TM4C1294
// Test the PLL function to verify that the system clock is
// running at the expected rate.  Use the debugger if possible
// or an oscilloscope connected to PN3, PN2, PN1, or PN0.
// The #define statement in the file PLL.h allows PLL_Init() to
// initialize the PLL to the desired frequency.  When using an
// oscilloscope to look at LED1, it should be clear to see that
// the LED flashes about 7.5 (120/16) times faster with a
// 120 MHz clock than with a 16 MHz clock.
// This program implements a failsafe mechanism in case
// PLL_Init() sets the system clock to an invalid state.  If
// you change something in PLL_Init() and the microcontroller
// or debugger stops working, hold SW1 and press the reset
// button on the microcontroller.  This skips the call to
// PLL_Init() and sets the clock to the default 16 MHz
// precision internal oscillator.
// Daniel Valvano
// March 31, 2014

/* This example accompanies the book
   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Program 4.6, Section 4.3
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Program 2.10, Figure 2.37

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "PLL.h"
#include "tm4c1294ncpdt.h"
#include "Timer.h"

#define GPIO_PORTJ0             (*((volatile uint32_t *)0x40060004))
#define GPIO_PORTN3210          (*((volatile uint32_t *)0x4006403C))

#define SYSCTL_RCGCGPIO_R12     0x00001000  // GPIO Port N Run Mode Clock
                                            // Gating Control
#define SYSCTL_RCGCGPIO_R8      0x00000100  // GPIO Port J Run Mode Clock
                                            // Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile uint32_t *)0x400FEA08))
#define SYSCTL_PRGPIO_R12       0x00001000  // GPIO Port N Peripheral Ready
#define SYSCTL_PRGPIO_R8        0x00000100  // GPIO Port J Peripheral Ready

// delay function for testing from sysctl.c
// which delays 3*ulCount cycles
#ifdef __TI_COMPILER_VERSION__
  //Code Composer Studio Code
  void Delay(uint32_t ulCount){
  __asm (  "    subs    r0, #1\n"
      "    bne     Delay\n"
      "    bx      lr\n");
}

#else
  //Keil uVision Code
  __asm void
  Delay(uint32_t ulCount)
  {
    subs    r0, #1
    bne     Delay
    bx      lr
  }

	__asm void
	WaitForInterrupt2(void)
	{
		WFI
		bx lr
	}
	
#endif
	
volatile uint32_t counter = 0;	

void Timer0A_Handler(void){
	
	TIMER0_ICR_R |= 0x00000011;
	GPIO_PORTN3210 ^= 0x0F;        // toggle LED1, LED2, PN2, and PN3
	counter ++;
}	
	
extern void StartCritical(void);

extern void EndCritical(void);
	
	
int main(void){
                                   // activate clock for Port J
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R8;
                                   // allow time for clock to stabilize
  while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R8) == 0){};
  GPIO_PORTJ_DIR_R &= ~0x01;       // make PJ0 in (PJ0 built-in SW1)
  GPIO_PORTJ_AFSEL_R &= ~0x01;     // disable alt funct on PJ0
  GPIO_PORTJ_PUR_R |= 0x01;        // enable pull-up on PJ0
  GPIO_PORTJ_DEN_R |= 0x01;        // enable digital I/O on PJ0
                                   // configure PJ0 as GPIO
  GPIO_PORTJ_PCTL_R = (GPIO_PORTJ_PCTL_R&0xFFFFFFF0)+0x00000000;
  GPIO_PORTJ_AMSEL_R &= ~0x01;     // disable analog functionality on PJ0
  if(GPIO_PORTJ0 == 0x01){         // only call PLL_Init() if switch is initially released
    PLL_Init();                    // set system clock to 120 MHz
  }
                                   // activate clock for Port N
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R12;
                                   // allow time for clock to stabilize
  while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R12) == 0){};
  GPIO_PORTN_DIR_R |= 0x0F;        // make PN3-0 out (PN1-0 built-in LED1-2, PN3-2 easier to reach)
  GPIO_PORTN_AFSEL_R &= ~0x0F;     // disable alt funct on PN3-0
  GPIO_PORTN_DEN_R |= 0x0F;        // enable digital I/O on PN3-0
                                   // configure PN3-0 as GPIO
  GPIO_PORTN_PCTL_R = (GPIO_PORTN_PCTL_R&0xFFFF0000)+0x00000000;
  GPIO_PORTN_AMSEL_R &= ~0x0F;     // disable analog functionality on PN3-0
  GPIO_PORTN3210 = 0x05;           // turn on LED2 and PN2 and turn off LED1 and PN3
  
	StartCritical();	
	Timer0_Init();	
	EndCritical();
		
/*		
	while(1){
    GPIO_PORTN3210 ^= 0x0F;        // toggle LED1, LED2, PN2, and PN3
    Delay(2000000);                // delay ~0.05 sec at 120 MHz (0.375 sec at 16 MHz; 0.078125 sec at 76.8 MHz)
  }
*/
	while(1){
	
		WaitForInterrupt2();
		
	};
		
}
