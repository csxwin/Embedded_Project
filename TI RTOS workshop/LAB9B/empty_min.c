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
#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Log.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Queue.h>

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

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
Semaphore_Handle hMySem;


typedef struct MsgObj{
	Queue_Elem elem;
	int Val;
}MsgObj, *Msg;


Void mailbox_queue()
{
	MsgObj msg;
	Msg msgp;
	msgp = &msg;

	msg.Val = 0;
	while(1)
	{
		Semaphore_pend(hMySem, -1);
		msg.Val ^= 1;
		Queue_put(LED_Queue, (Queue_Elem *)msgp);
		Semaphore_post(QueSem);
	}
}

Void BLUE_Toggle()
{
	MsgObj msg;
	Msg msgp;


    while (1) {
    	//Log_info1("start task [%u]", 0);
    	Semaphore_pend(QueSem, -1);
    	msgp = Queue_get(LED_Queue);
    	msg = * msgp;
    	if(msg.Val)
    	{
    		GPIO_write(Board_LED0, Board_LED_ON);
    	}
    	else{
    		GPIO_write(Board_LED0, Board_LED_OFF);
    	}
        //Log_info1("finish task [%u]", 1);
    }
}


void Timer_ISR()
{
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	Semaphore_post(hMySem); //hMySem is used for unblock mailbox_queue
}


/*
 *  ======== main ========
 */
int main(void)
{
    //Task_Params taskParams;

    hMySem = Semaphore_create(0, NULL, NULL);

    Board_initGeneral();
    Board_initGPIO();

	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    ROM_TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER2_BASE, TIMER_A, ROM_SysCtlClockGet()/2);
    ROM_TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER2_BASE, TIMER_A);

    /* Call board init functions */

    // Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initWatchdog();
    // Board_initWiFi();

    /*Below code is dynamically construct task
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)BLUE_Toggle, &taskParams, NULL);
	*/

    /* Turn on user LED  */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
