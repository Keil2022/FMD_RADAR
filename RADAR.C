#include	"SYSCFG.h"
#ifndef __RADAR_H
#define __RADAR_H

#define RADAR	RA6
#define POUT	RA5

void Radar_Init(void)
{
	TRISA6 = 0;	//输出模式	//0-输出；1-输入；
	RADAR = 0;
    
	TRISA5 = 1;	//输入模式
    WPUA5 = 1;	//弱上拉
}

#endif
