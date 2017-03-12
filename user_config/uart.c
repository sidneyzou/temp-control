#include "uart.h"


//注意：串口1，2都用独立波特率发生器
void Uart1Init(void)		//115200bps@11.0592MHz
{
	PCON &= 0x7F;		//??????
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1???
	TMOD |= 0x20;		//?????1?8???????
	TL1 = 0xFD;		//??????
	TH1 = 0xFD;		//????????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
}

void Uart2Init(void)//115200bps@11.0592MHz
{
	AUXR &=0XF7;		//波特率不倍速
	S2CON = 0x50;		//8位数据，可变波特率
	AUXR |= 0x04;		//1T模式
	BRT   =  0xDC;		//设置独立波特率发生器重装值
// 	AUXR |=0X01;		//串口2选择独立发生器为波特率发生器
	AUXR |=0X10;		//启动独立波特率发生器
// 	IE2  |=0X01; 		//使能串口2中断
}

/*----------------------------
UART1 发送串口数据
-----------------------------*/
void UART1_SendData(u8 dat)
{
	ES=0;					//关串口中断
	SBUF=dat;			
	while(TI!=1);	//等待发送成功
	TI=0;					//清除发送中断标志
	ES=1;					//开串口中断
}
/*----------------------------
UART1 发送字符串
-----------------------------*/
void UART1_SendString(char *s)
{
	while(*s)//检测字符串结束符
	{
		UART1_SendData(*s++);//发送当前字符
	}
}

/*----------------------------
UART2 发送串口数据
-----------------------------*/
void UART2_SendData(u8 dat)
{
	IE2 &= ~0x01;					//关串口中断
	S2BUF=dat;			
	while((S2CON&S2TI)!=S2TI);	//等待发送成功
	S2CON &= ~S2TI;					//清除发送中断标志
	IE2 = 0x01;					//开串口中断
}
/*----------------------------
UART2 发送字符串
-----------------------------*/
void UART2_SendString(u8 *s)
{
	while(*s)//检测字符串结束符
	{
		UART2_SendData(*s++);//发送当前字符
	}
}

 void VDEBUG(char *s)
{
#if DEBUG
	UART1_SendString(s);
#endif
}
