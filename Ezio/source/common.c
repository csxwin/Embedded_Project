/*
 * common.c
 *
 *  Created on: Apr 25, 2016
 *      Author: csxwin
 */
#include <stdint.h>
#include "common.h"

void delay1ms (uint32_t ms)
{
	uint32_t volatile time;
	while(ms){
		time = 72724*2/91*5/8;
		while(time){
			time --;
		}
		ms--;
	}
}
