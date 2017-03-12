#include "head.h"




void main() {
  
	/* 定时器初始化 */
	TimerInit();
	
    /* 串口初始化 */
	Uart1Init();
 
	/* OLED初始化 */
	OLED_Init();
		
	/* EEPROM初始化 */
	IapInit();	
	
	/* 打开总中断 */
    EA = 1;
	
	Read_RTC();
	
	ShowMenu();
    while(1){

         entry();
    }
}

