#include	"SYSCFG.h"
#ifndef __LED_H
#define __LED_H

#define LED_P		RA4
#define LED_M		RC5

void LED_Init(void)
{
	TRISA4 = 0;	//0:输出模式
    TRISC5 = 0;	//0:输出模式
	
    LED_P = 0;
    LED_M = 0;
}

void LED_R(void)
{
	LED_P = 1;
    LED_M = 0;
}

void LED_G(void)
{
	LED_P = 0;
    LED_M = 1;
}

void LED_OFF(void)
{
	LED_P = 0;
    LED_M = 0;
}

#endif

