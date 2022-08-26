# MCU:FT61F023A  
**应用：垃圾桶盖的感应开关**  
**——采用睡眠低功耗方案**  
**——检测PA口电平变化即唤醒**  

## IO口：  
PA0——烧录口CLK  
PA1——烧录口DAT  

## LED  
**采用两脚双色LED，控制导通方向实现颜色转换**  
PA4——LED+  
PC5——LED-  

## 电机  
**高低电平控制正反转（10、01）、停止（00）、刹车（11）**  
PC1——INA  
PC0——INB  


## 按键  
**采用低功耗待机触摸芯片——BS813A-1（只是手头正好有，还有其他很多选择）**  
**待机输出高电平，检测有触摸按下输出低电平**  
PA2——KEY1  
PA3——KEY2  


## 雷达  
**雷达默认输出低电平，检测到移动输出高电平，期间持续检测，再次检测到人刷新时间**  
PA6——RADAR *//雷达模组开关，通过mos控制*  
PA5——Pout  *//雷达信号接收口*  