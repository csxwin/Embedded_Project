#include "PLL.h"
#include "PeriodMeasure.h"
#include "E:\Google Drive\~Embedded Systems\tm4c123gh6pm.h"

uint32_t Period1;              // (1/clock) units
uint32_t First1;               // Timer0A first edge
int32_t Done1;                // set each rising

//debug code
int main(void){           
  PLL_Init();                      // 80 MHz clock
	//Timer0_Capture_Init();
	TIMER0_Input_Capture();
  //PeriodMeasure_Init();            // initialize 24-bit timer0A in capture mode
  while(1){
    WaitForInterrupt();
  }
}




/*
void Timer0A_Handler(void){
	TIMER0_ICR_R |= 0x04;	
	PF2 = PF2^0x04;
	//DR = DE;
}
*/