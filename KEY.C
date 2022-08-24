#include	"SYSCFG.h"
#ifndef __KEY_H
#define __KEY_H

#include "Hardward.c"
#include "led.c"
#include "RADAR.c"

#define KEY1    RA2
#define KEY2    RA3

#define KEY_Press 	1
#define KEY_Lift 		0
#define	True			1
#define	Fault			0
#define	Lid_ON				1
#define	Lid_OFF			0

#define KEY_ShortTime		2		//50ms
#define KEY_LongTime		100	//1s
#define KEY_EndTime		300	//3s

bit KEY1_ShortOK;	//�̰���Ч
bit KEY1_LongOK;		//������Ч
bit KEY1_EndOK;		//��������
u8 KEY1_Phase;		//�����׶�
u16 KEY1_TimeCount;	//����ʱ��

bit KEY2_ShortOK;
bit KEY2_LongOK;	
bit KEY2_EndOK;
u8 KEY2_Phase;
u16 KEY2_TimeCount;

bit State;

void Key_Init(void)
{
	TRISA2 = 1;	//����ģʽ	//0-�����1-���룻
	WPUA2 = 1;	//������
   
	TRISA3 = 1;	//����ģʽ
	WPUA3 = 1;	//������ 
    
	KEY1_Phase = 0;
    KEY1_TimeCount = 0;
    
	KEY2_Phase = 0;
    KEY2_TimeCount = 0;
}

void Key1_Scanf(void)
{
	switch(KEY1_Phase)
    {
		case 0:
					//����1
					if(KEY1 == KEY_Press)
					{
						KEY1_TimeCount++;
						if(KEY1_TimeCount >=  KEY_LongTime)
						{
							//KEY1_Phase = 1;	//ע�͵�Ϊ ����ת��������
						}
					}
					else
					{
						if(KEY1_TimeCount >= KEY_ShortTime)		KEY1_ShortOK = True;
						KEY1_TimeCount = 0;
					}
		break;
        
        
        case 1:
					if(KEY1 == KEY_Press)
					{
						KEY1_TimeCount++;
						if(KEY1_TimeCount >= KEY_EndTime)
						{
							KEY1_Phase = 2;
                            //KEY1_LongOK = True;
						}
					}
					else
					{
						KEY1_Phase = 0;
						KEY1_TimeCount = 0;
                        KEY1_LongOK = True;
					}
		break;
        
        
        case 2:
					if(KEY1 == KEY_Lift)	//�ȴ�����
					{
						KEY1_Phase = 0;
						KEY1_TimeCount = 0;
                        KEY1_LongOK = True;
					}
		break;
        

        default:
					KEY1_Phase = 0;
					KEY1_TimeCount = 0;
		break;
	}
}

void Key_Scanf(void)
{
	//����1
	if(KEY1 == KEY_Press)
	{
		KEY1_TimeCount++;
		if(KEY1_TimeCount >=  KEY_LongTime)
		{
			//KEY1_Phase = 1;	//ע�͵�Ϊ ����ת��������
		}
        else {/*do nothing*/}
	}
	else
	{
		if(KEY1_TimeCount >= KEY_ShortTime)	KEY1_ShortOK = True;
        else {/*do nothing*/}

		KEY1_TimeCount = 0;
	}
	
	//����2
	if(KEY2 == KEY_Press)
	{
		KEY2_TimeCount++;
		if(KEY2_TimeCount >=  KEY_LongTime)
		{
			//KEY2_Phase = 1;	//ע�͵�Ϊ ����ת��������
		}
        else {/*do nothing*/}
	}
	else
	{
		if(KEY2_TimeCount >= KEY_ShortTime)	KEY2_ShortOK = True;
        else {/*do nothing*/}
		
		KEY2_TimeCount = 0;
	}
}

void Key_Handle(void)
{
	if(KEY1_ShortOK == True)
    {
		KEY1_ShortOK = Fault;
		
		RADAR ^= 1;
		if(RADAR == 1)	LED_G();
        else LED_R();
        
		delay_ms(500);
		LED_OFF();
	}
    else {/*do nothing*/}
    
	if(KEY2_ShortOK == True)
    {
		KEY2_ShortOK = Fault;
		
		State ^= 1;
	}
    else {/*do nothing*/}
}

#endif
