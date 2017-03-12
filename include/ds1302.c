#include "head.h"

/********IO���Ŷ���***********************************************************/
sbit RST = P0^6;	//��λ�ţ�Ҳ��Ƭѡ�ţ�
sbit DAT = P0^5;	//��������
sbit CLK = P0^4;   // ʱ������

/********���ݶ���*************************************************************/
code unsigned char write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; 	//дʱ����Ϣ��ַ
code unsigned char read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};	//��ʱ����Ϣ��ַ

unsigned char l_timer[7]={0x00,26,11,6,5,5,16};
											//���ڶ�ʱ����Ϣ���壬��ֵΪ��һ�ε�д��ʱ��
											//�룬�֣�ʱ���գ��£����ڣ���
u8 l_time_buf0[11] = "2015-11-13";
u8 l_time_buf1[11] = "12:30 20";

u8 g_rtc[6];

/************д�ֽڵ�DS1302*******************************************/
void Write_Ds1302_Byte(unsigned  char temp) 
{
 unsigned char i;
 for (i=0;i<8;i++)     		//дһ���ֽ�
  { 
	 CLK=0;
     DAT=temp&0x01;     	//��ȡ��λ���ݣ���λ��ǰ��λ�ں�
     temp>>=1;  			//����һλ
     CLK=1;
   }
}   

/************���ݵ�ַд����*************************************************/
void Write_Ds1302( unsigned char address,unsigned char dat )     
{
 	RST=0;
 	CLK=0;						//׼��
 	RST=1;						//��ʼ

 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	RST=0;  					//�ָ�
}

/***********���ݵ�ַ������*************************************************/
unsigned char Read_Ds1302 ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;
 	CLK=0;
 	RST=1;

 	Write_Ds1302_Byte(address);
 	for (i=0;i<8;i++) 			//��һ�ֽ�
 	{
		temp>>=1;				//����һλ����λ��ǰ��λ�ں�
		CLK=0;	
 		if(DAT)
 			temp|=0x80;			//��λ��ǰ��λ�ں�
		CLK=1;						
	} 
 	RST=0;
	DAT=0;
	return (temp);				//������Ӧֵ
}

/**********���룬�֣�ʱ���գ��£����ڣ���*************************************/
void Read_RTC()		//��ȡ ����
{
 	unsigned char i,*p;
 	p=read_rtc_address; 				//����ַ
 	for(i=0;i<7;i++)					//��7�ζ�ȡ ������ʱ��������
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

/**********���룬�֣�ʱ���գ��£����ڣ���,װ�����ַ�����ʽ*************************************/
void Read_RTC_Char()		//��ȡ ����
{

	l_time_buf0[0]='2';
	l_time_buf0[1]='0';
	l_time_buf0[2]=(l_timer[6]>>4)+'0';  //��    
	l_time_buf0[3]=(l_timer[6]&0x0f)+'0';
	
	
	
	l_time_buf0[5]=(l_timer[4]>>4)+'0'; //��  
	l_time_buf0[6]=(l_timer[4]&0x0f)+'0';
	
	
	
	l_time_buf0[8]=(l_timer[3]>>4)+'0'; //��   
	l_time_buf0[9]=(l_timer[3]&0x0f)+'0';

	
	
	//��2����ʾ  
	l_time_buf1[0]=(l_timer[2]>>4)+'0'; //ʱ   
	l_time_buf1[1]=(l_timer[2]&0x0f)+'0';  
	
	
	
	l_time_buf1[3]=(l_timer[1]>>4)+'0'; //��   
	l_time_buf1[4]=(l_timer[1]&0x0f)+'0';  

	

	l_time_buf1[6]=(l_timer[0]>>4)+'0'; //��   
	l_time_buf1[7]=(l_timer[0]&0x0f)+'0';

	
}


/**********д�룬�֣�ʱ���գ��£����ڣ���*************************************/
void Set_RTC()
{
	unsigned char i,*p,tmp;
	
	l_timer[6] = g_rtc[0];
	l_timer[4] = g_rtc[1];
	l_timer[3] = g_rtc[2];
	l_timer[2] = g_rtc[3];
	l_timer[1] = g_rtc[4];
	l_timer[0] = g_rtc[5];
	for(i=0;i<7;i++){				//������תΪBCD��
		tmp=l_timer[i]/10;
		l_timer[i]=l_timer[i]%10;
		l_timer[i]=l_timer[i]+tmp*16;
	}
 	Write_Ds1302(0x8E,0X00);		//��д����
	
 	p=write_rtc_address;			//д��ַ	
 	for(i=0;i<7;i++)				//д�� ������ʱ��������
 	{
		  Write_Ds1302(*p,l_timer[i]);
 		 p++;  
	 }
	 Write_Ds1302(0x8E,0x80);		//�ر�д����
}

void RTC_Iint(void)
{
	Read_RTC();

}