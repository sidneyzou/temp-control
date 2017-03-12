/**********************DS18B20�����ļ�*********************************

******************************************************************/

/**********************�����ļ�*******************/
#include "head.h"
#include <INTRINS.H>
/********************************ȫ�ֱ�����ʼ��********************************/ 


/*********************************************************************************************************
** �������� ����ʱ����
** ����˵�� ������������ʱ��ռ��CPU����������С��λ��ԼΪ1us
** ��ڲ��� ��time:��Ҫ��ʱ��ʱ�䣬��λus
** ���ڲ��� ����
*********************************************************************************************************/
void DelayXus(u8 n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}
/*********************************************************************************************************
** �������� ��DS18B20��λ����
** ����˵�� ����λDS18B20,������豸�Ƿ����
** ��ڲ��� ����
** ���ڲ��� ����
*********************************************************************************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //�ͳ��͵�ƽ��λ�ź�
        DelayXus(240);              //��ʱ����480us
        DelayXus(240);
        DQ = 1;                     //�ͷ�������
        DelayXus(60);               //�ȴ�60us
        CY = DQ;                    //����������
        DelayXus(240);              //�ȴ��豸�ͷ�������
        DelayXus(180);
    }
}

/*********************************************************************************************************
** �������� ��DS18B20���ֽں���
** ����˵�� ����DS18B20��1�ֽ�����
** ��ڲ��� ����
** ���ڲ��� ����DS18B20���ص�1�ֽ�����
*********************************************************************************************************/
u8 DS18B20_ReadByte()
{
    u8 i;
    u8 dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        DQ = 1;                     //׼������
        DelayXus(1);                //������ʱ
        if (DQ) dat |= 0x80;        //��ȡ����
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/*********************************************************************************************************
** �������� ��DS18B20д�ֽں���
** ����˵�� ����DS18B20д1�ֽ�����
** ��ڲ��� ��Ҫд��DS18B20��1�ֽ�����
** ���ڲ��� ����
*********************************************************************************************************/
void DS18B20_WriteByte(u8 dat)
{
    char i;

    for (i=0; i<8; i++)             //8λ������
    {
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        dat >>= 1;                  //�ͳ�����
        DQ = CY;
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
        DQ = 1;                     //�ָ�������
        DelayXus(1);                //�ָ���ʱ
    }
}


/******************************************************************************/
/* ��������  : TMP                                                            */
/* ��������  : DS18B20�¶Ȳɼ�����                                            */
/* �������  : void                                                           */
/* ��������  : ��                                                             */
/* ����ֵ    : unsigned int  �¶�ֵ                                           */
/******************************************************************************/
u16 TMP(void)               //���Ĵ������ݺ���ֵת��
{
    static u8 num = 0;
    static u16 TP;
    u8 TPL,TPH;
    
    num++;
    num = num % 2;
    
    if(num)
    {
	    DS18B20_Reset();                //�豸��λ	
	    DS18B20_WriteByte(0xCC);        //����ROM����  	
	    DS18B20_WriteByte(0xBE);        //���ݴ�洢������
	    TPL = DS18B20_ReadByte();       //���¶ȵ��ֽ�
	    TPH = DS18B20_ReadByte();       //���¶ȸ��ֽ�
        
        TP=((TPH<<8)|TPL) * 0.625;			//����ȡ������ת����ʮ������
    } else {
	    DS18B20_Reset();                //�豸��λ
	    DS18B20_WriteByte(0xCC);        //����ROM����
	    DS18B20_WriteByte(0x44);     	//��ʼת������            
    }
	
	return TP;
}



