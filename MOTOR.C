#include "SYSCFG.h"
#ifndef __MOTOR_H
#define __MOTOR_H

#define REVERSE 0 

#if REVERSE
#define INA    RC0
#define INB    RC1
#else
#define INA    RC1
#define INB    RC0
#endif

void MOTOR_Init(void)
{
	TRISC0 = 0;	//输出模式
	TRISC1 = 0;	//输出模式
    
	INA = 0;
    INB = 0;
}

void Forward(void)	//前进
{
	INA = 1;
    INB = 0;
}

void Backward(void)	//后退
{
	INA = 0;
    INB = 1;
}

void STOP(void)	//停止
{
	INA = 0;
    INB = 0;
}

void Brake(void)	//刹车
{
    INA = 1;
    INB = 1;
}

#endif