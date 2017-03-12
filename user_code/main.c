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
	
	/* PID��ʼ�� */
	Init_pid();
	
	RTC_Iint();
	
	ShowMenu();
	
	
    while(1){

         entry();
    }
}

