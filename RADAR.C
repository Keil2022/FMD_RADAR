#include	"SYSCFG.h"
#ifndef __RADAR_H
#define __RADAR_H

#define RADAR	RA6

void Radar_Init(void)
{
	TRISA6 = 0;	//输入模式	//0-输出；1-输入；
    
	RADAR = 0;
}

#endif
