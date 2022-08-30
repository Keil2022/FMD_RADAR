#include	"SYSCFG.h"
#ifndef __LED_H
#define __LED_H



void LED_Init(void)
{
	TRISA4 = 0;	//0:输出模式
    TRISC5 = 0;	//0:输出模式
	
    LED_P = OFF;
    LED = OFF;
}

#endif

