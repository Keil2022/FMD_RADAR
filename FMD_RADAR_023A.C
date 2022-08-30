//Project: FMD_022A.prj
// Device: FT61F02X
// Memory: Flash 2KX14b, EEPROM 256X8b, SRAM 128X8b
// Author: 
//Company: 
//Version:
//   Date: 2022年8月17日11:42:04
//===========================================================
//===========================================================
#include "SYSCFG.h"
#include "Hardward.c"
#include "motor.c"

#define ON_Time		2		//s
#define	OFF_Time		2

#define KEY1    RA2
#define KEY2    RA3

#define LED_P		RC5
#define LED			RA4

#define RADAR	RA6
#define POUT	RA5

#define ON 	1
#define OFF	0

#define True 	1
#define Fault		0

BITS_T falg;
#define STATE		falg.bit0	//0-关；1-开
#define KEY1_OK	falg.bit1
#define KEY2_OK	falg.bit2

#define eSTATE 	0
#define	eSetAddr	1

u8 ReadAPin;
u8 FCount;

void Test(void);

//中断入口
void interrupt ISR(void)
{
	//PA电平变化中断**********************
	 if(PAIE && PAIF)
    {
		ReadAPin = PORTA; 	//读取PORTA数据清PAIF标志
		PAIF = 0;  				//清PAIF标志位
		PAIE = 0;  				//暂先禁止PA2中断
		IOCA2 = 0;  				//禁止PA2电平变化中断
        IOCA3 = 0;  				//禁止PA3电平变化中断
        IOCA5 = 0;  				//禁止PA2电平变化中断
    }
}

void Sys_Init(void) 
{ 
	OSCCON = 0B01110001; 
    //WDT:32KHZ;	
    //IRCF=111=16MHZ;	2T模式
    //Bit0	=	1——系统时钟为内部振荡器；0——时钟源由FOSC<2：0>决定即编译选项时选择

    INTCON= 0;      //暂禁止所有中断
	
	//端口配置
	//A
    TRISA = 0B00000000;    	//0-输出；1-输入；
    PORTA = 0B00000000;	//数据寄存器
    WPUA = 0B00000000;    //0-关闭弱上拉；1-打开弱上拉

	//C
    TRISC     = 0B00000000;    //0-输出；1-输入；
    PORTC     = 0B00000000;	
    WPUC      = 0B00000000;    //0-关闭弱上拉；1-打开弱上拉
    
    MSCKCON =	0B00000000;
	//MSCKCON.7——保留位:写0
    //MSCKCON.6—REG_OE:1 = 打开片内稳压器，PA4 和 PC5 输出稳压器电压；\
											  0 = PA4 和 PC5 为普通 IO
    //MSCKCON.5——T2CKSRC:0 = TIMER2 时钟源为系统时钟\
												1 = TIMER2 时钟源为内部 32MHz
    //MSCKCON.4——SLVREN:1 = 打开 LVR\
											0 = 禁止 LVR\
											当 UCFG1<1:0>不为 01 时，此位无实际意义
    
    //MSCKCON.3——保留位：写0
    //MSCKCON.2——CKMAVG:1 = 打开平均模式（自动测量并累加 4 次）\
												0 = 关闭平均模式
    //MSCKCON.1——CKCNTI:1 = 使能快时钟测量慢时钟周期\
											0 = 关闭快时钟测量慢时钟周期
    //MSCKCON.0——保留位：写0

    CMCON0 = 0B00000111;     //关闭比较器，CxIN为数字IO口
    
//    ADCON1 = 0B01100000;     //DIVS=0,时钟选FOSC    //ADCS[2:0]=110,分频FOSC/64
//    ADCON0 = 0B10011101;     //B7,ADFM=1,结果右对齐                         
   // ANSEL = 0X00;	//关闭所有模拟输入

    OPTION = 0B00001000;    //Bit3 = 1 预分频器分配给 WDT；PS=000=1:1 WDT RATE	
											//Bit7(RAPU)=0 ENABLED PULL UP PA
}

void PA_Level_Change_INITIAL(void)
{
	ANSEL = 0B11010011;		//关闭中断引脚模拟输入功能
    
    TRISA2 = 1;						//SET PA2 INPUT
    TRISA3 = 1;
	TRISA5 =1; 						
    
	ReadAPin = PORTA;			//清PA电平变化中断
	PAIF =0;   						//清PA INT中断标志位
    
	IOCA2 = 1;  				//使能PA2电平变化中断
	IOCA3 = 1;  				//使能PA3电平变化中断
	IOCA5 = 1;  				//使能PA5电平变化中断
    
	PAIE =1;   						//使能PA INT中断
	GIE =1;    						//使能全局中断
}

void Key_Init(void)
{
	TRISA2 = 1;	//输入模式	//0-输出；1-输入；
	//WPUA2 = 1;	//弱上拉
   
	TRISA3 = 1;	//输入模式
	//WPUA3 = 1;	//弱上拉 
}

void LED_Init(void)
{
	TRISA4 = 0;	//0:输出模式
    TRISC5 = 0;	//0:输出模式
	
    LED_P = OFF;
    LED = OFF;
}

void Radar_Init(void)
{
	TRISA6 = 0;	//输出模式	//0-输出；1-输入；
	RADAR = 0;
    
	TRISA5 = 1;	//输入模式
    //WPUA5 = 1;	//弱上拉
}

//数据初始化
void InitRam(void)
{
    u8 set;
    
	DelayMs(20);
    set = EEPROMread(eSetAddr);
    if(set!=0x5A)
    {
		set=0x5A;
        STATE = 1;
        
		EEPROMwrite(eSTATE,STATE);
		EEPROMwrite(eSetAddr, set);  
    }
    STATE = EEPROMread(eSTATE);
    
	KEY1_OK = Fault;
    KEY2_OK = Fault;
}

//主函数
int main(void)
{
	DelayS(2);	//热机，让外设就位
    
	Sys_Init();
    Key_Init();
	LED_Init();
	Radar_Init();
    MOTOR_Init();
	
	InitRam();

	while(1)
    {
		DelayMs(10);	
        STATE = EEPROMread(eSTATE);	//读取

        if(POUT == 1)	//雷达检测高电平——开
        {
			if(STATE == 0)
            {
				Forward();
				DelayS(ON_Time);
				Brake();
				DelayMs(100);
				STOP();
				
				STATE = 1;
				EEPROMwrite(eSTATE,STATE);
            }
        }
        else		//雷达检测低电平——关
        {
			if(STATE)
            {
				Backward();
				DelayS(OFF_Time);
				Brake();
				DelayMs(100);
				STOP();
				
				STATE = 0;
				EEPROMwrite(eSTATE,STATE);
            }
		}
        
		if(KEY2 == 0)
        {
            KEY2_OK = True;
		}
        else	//检测松手有效
        {
            if(KEY2_OK == True)
            {
                KEY2_OK = Fault;
                
				RADAR ^= 1;
				
				/*ON -- 常亮；OFF -- 闪烁*/
				for(FCount=0;FCount<3;FCount++)
				{
					LED = ON;
					DelayMs(350);
					if(RADAR == OFF)	LED = OFF;
					DelayMs(350);
				}
				LED = OFF;
            }
		}
        
		if(KEY1 == 0)
        {
            KEY1_OK = True;
		}
		else	//检测松手有效
        {
			if(KEY1_OK == True)
            {
                KEY1_OK = Fault;
                
				if(STATE)
				{
					STATE = 0;
					EEPROMwrite(eSTATE,STATE);
					
					Backward();
					DelayS(OFF_Time);
					Brake();
					DelayMs(100);
					STOP();
				}
				else
				{
					STATE = 1;
					EEPROMwrite(eSTATE,STATE);
					
					Forward();
					DelayS(ON_Time);
					Brake();
					DelayMs(100);
					STOP();
				}
            }
        }
        
		//Test();
        
		PA_Level_Change_INITIAL();
		SLEEP(); 
	}
	return 0;
}

void Test(void)
{
	if(KEY1 == 0)
	{
		for(FCount=0;FCount<100;FCount++)	//输出100次波形	
		{
			LED = 1;				
			DelayMs(5);  	//10ms 
			LED = 0;
			DelayMs(15); 
		}
	}
	else
	{
		for(FCount=0;FCount<100;FCount++)	//输出100次波形	
		{
			LED = 1;
			DelayMs(10);  					//10ms 
			LED = 0;
			DelayMs(10); 
		}
	}
}


//===========================================================
