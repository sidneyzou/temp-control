#include "head.h"




void main() {
  
	/* ��ʱ����ʼ�� */
	TimerInit();
	
    /* ���ڳ�ʼ�� */
	Uart1Init();
 
	/* OLED��ʼ�� */
	OLED_Init();
		
	/* EEPROM��ʼ�� */
	IapInit();	
	
	/* �����ж� */
    EA = 1;
	
	Read_RTC();
	
	ShowMenu();
    while(1){

         entry();
    }
}

