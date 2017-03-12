#include "head.h"

/********IO引脚定义***********************************************************/
sbit RST = P0^6;	//复位脚（也叫片选脚）
sbit DAT = P0^5;	//数据引脚
sbit CLK = P0^4;   // 时钟引脚

/********数据定义*************************************************************/
code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; 	//写时间信息地址
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};	//读时间信息地址

unsigned char l_timer[7]={0x00,26,11,6,5,5,16};
											//用于读时间信息缓冲，初值为第一次的写入时间
											//秒，分，时，日，月，星期，年
u8 l_time_buf0[11] = "2015-11-13";
u8 l_time_buf1[11] = "12:30 20";

u8 g_rtc[6];

/************写字节到DS1302*******************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 unsigned char i;
 for (i=0;i<8;i++)     		//写一个字节
  { 
	 CLK=0;
     DAT=temp&0x01;     	//获取低位数据，低位在前高位在后
     temp>>=1;  			//右移一位
     CLK=1;
   }
}   

/************根据地址写数据*************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
 	CLK=0;						//准备
 	RST=1;						//开始

 	Write_Ds1302_Byte(address);	//发送地址
 	Write_Ds1302_Byte(dat);		//发送数据
 	RST=0;  					//恢复
}

/***********根据地址读数据*************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
 	CLK=0;
 	RST=1;

 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 			//读一字节
 	{
		temp>>=1;				//右移一位，低位在前高位在后
		CLK=0;	
 		if(DAT)
 			temp|=0x80;			//低位在前高位在后
		CLK=1;						
	} 
 	RST=0;
	DAT=0;
	return (temp);				//返回相应值
}

/**********读秒，分，时，日，月，星期，年*************************************/
void Read_RTC()		//读取 日历
{
 	unsigned char i,*p;
 	p=read_rtc_address; 				//读地址
 	for(i=0;i<7;i++)					//分7次读取 年月日时分秒星期
 	{
 		 l_timer[i]=Read_Ds1302(*p);
 		 p++;
 	}

	g_rtc[0] = ((l_timer[6] >> 4) * 10) + (l_timer[6]&0x0f);
	g_rtc[1] = ((l_timer[4] >> 4) * 10) + (l_timer[4]&0x0f);
	g_rtc[2] = ((l_timer[3] >> 4) * 10) + (l_timer[3]&0x0f);
	g_rtc[3] = ((l_timer[2] >> 4) * 10) + (l_timer[2]&0x0f);
	g_rtc[4] = ((l_timer[1] >> 4) * 10) + (l_timer[1]&0x0f);
	g_rtc[5] = ((l_timer[0] >> 4) * 10) + (l_timer[0]&0x0f);
	
}

/**********读秒，分，时，日，月，星期，年,装换成字符串形式*************************************/
void Read_RTC_Char()		//读取 日历
{

	l_time_buf0[0]='2';
	l_time_buf0[1]='0';
	l_time_buf0[2]=(l_timer[6]>>4)+'0';  //年    
	l_time_buf0[3]=(l_timer[6]&0x0f)+'0';
	
	
	
	l_time_buf0[5]=(l_timer[4]>>4)+'0'; //月  
	l_time_buf0[6]=(l_timer[4]&0x0f)+'0';
	
	
	
	l_time_buf0[8]=(l_timer[3]>>4)+'0'; //日   
	l_time_buf0[9]=(l_timer[3]&0x0f)+'0';

	
	
	//第2行显示  
	l_time_buf1[0]=(l_timer[2]>>4)+'0'; //时   
	l_time_buf1[1]=(l_timer[2]&0x0f)+'0';  
	
	
	
	l_time_buf1[3]=(l_timer[1]>>4)+'0'; //分   
	l_time_buf1[4]=(l_timer[1]&0x0f)+'0';  

	

	l_time_buf1[6]=(l_timer[0]>>4)+'0'; //秒   
	l_time_buf1[7]=(l_timer[0]&0x0f)+'0';

	
}


/**********写秒，分，时，日，月，星期，年*************************************/
void Set_RTC()
{
	unsigned char i,*p,tmp;
	
	l_timer[6] = g_rtc[0];
	l_timer[4] = g_rtc[1];
	l_timer[3] = g_rtc[2];
	l_timer[2] = g_rtc[3];
	l_timer[1] = g_rtc[4];
	l_timer[0] = g_rtc[5];
	for(i=0;i<7;i++){				//将数据转为BCD码
		tmp=l_timer[i]/10;
		l_timer[i]=l_timer[i]%10;
		l_timer[i]=l_timer[i]+tmp*16;
	}
 	Write_Ds1302(0x8E,0X00);		//打开写允许
	
 	p=write_rtc_address;			//写地址	
 	for(i=0;i<7;i++)				//写入 年月日时分秒星期
 	{
		  Write_Ds1302(*p,l_timer[i]);
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);		//关闭写允许
}

void RTC_Iint(void)
{
	Read_RTC();

}