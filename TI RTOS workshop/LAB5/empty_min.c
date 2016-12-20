/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/cfg/global.h>
#include <xdc/std.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include "Board.h"

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"




#define TASKSTACKSIZE   512

#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))

#define BLUE_LED 				(*((volatile unsigned long *)0x40025010))
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.

Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((UInt)arg0);
        GPIO_toggle(Board_LED0);
    }
}
 */
void BLUE_LED_Init(){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_PRGPIO_R & 0x00000020) == 0){};

	GPIO_PORTF_AFSEL_R &= ~0x04;
	GPIO_PORTF_DIR_R |= 0x04;
	GPIO_PORTF_DEN_R |= 0x04;
	GPIO_PORTF_AMSEL_R &= ~0x04;
	GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & 0xFFFFF0FF) + 0x00000000;
}
/*
void
Timer0IntHandler(void)
{
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    BLUE_LED ^= 0x04;
    ROM_IntMasterDisable();
    ROM_IntMasterEnable();
}

void
Timer1IntHandler(void)
{
    ROM_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    BLUE_LED ^= 0x04;
    ROM_IntMasterDisable();
    ROM_IntMasterEnable();
}
*/
void LEDToggle()
{
	BLUE_LED ^= 0x04;
}

void Timer_ISR()
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	//Swi_post(LEDSwi);
	LEDToggle();
}



int main(void)
{
    //Task_Params taskParams;
	//ROM_FPULazyStackingEnable();

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	BLUE_LED_Init();
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    //ROM_IntMasterEnable();
    ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER2_BASE, TIMER_A, ROM_SysCtlClockGet()/2);
    //ROM_IntEnable(INT_TIMER2A);
    ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER2_BASE, TIMER_A);
    /* Call board init functions */
    //Board_initGeneral();
    //Board_initGPIO();
    // Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initWatchdog();
    // Board_initWiFi();
/*
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);
*/

    /* Turn on user LED  */
    //GPIO_write(Board_LED0, Board_LED_ON);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
