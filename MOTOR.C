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
	TRISC0 = 0;	//���ģʽ
	TRISC1 = 0;	//���ģʽ
    
	INA = 0;
    INB = 0;
}

void Forward(void)	//ǰ��
{
	INA = 1;
    INB = 0;
}

void Backward(void)	//����
{
	INA = 0;
    INB = 1;
}

void STOP(void)	//ֹͣ
{
	INA = 0;
    INB = 0;
}

void Brake(void)	//ɲ��
{
    INA = 1;
    INB = 1;
}

#endif