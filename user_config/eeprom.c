#include "head.h"
#include "eeprom.h"
#include "intrins.h"




/*----------------------------
Disable ISP/IAP/EEPROM function
Make MCU in a safe state
----------------------------*/
void IapIdle()
{
	IAP_CONTR = 0; //Close IAP function
	IAP_CMD = 0; //Clear command to standby
	IAP_TRIG = 0; //Clear trigger register
	IAP_ADDRH = 0x80; //Data ptr point to non-EEPROM area
	IAP_ADDRL = 0; //Clear IAP address to prevent misuse
}
/*----------------------------
Read one byte from ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
Output:Flash data
----------------------------*/
BYTE IapReadByte(WORD addr)
{
	BYTE dat; //Data buffer
	IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
	IAP_CMD = CMD_READ; //Set ISP/IAP/EEPROM READ command
	IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
	IAP_TRIG = 0x5a; //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5; //Send trigger command2 (0xa5)
	_nop_(); //MCU will hold here until ISP/IAP/EEPROM
	//operation complete
	dat = IAP_DATA; //Read ISP/IAP/EEPROM data
	IapIdle(); //Close ISP/IAP/EEPROM function
	return dat; //Return Flash data
}
/*----------------------------
Program one byte to ISP/IAP/EEPROM area
Input: addr (ISP/IAP/EEPROM address)
dat (ISP/IAP/EEPROM data)
Output:-
----------------------------*/


void IapProgramByte(WORD addr, BYTE dat)
{
	IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
	IAP_CMD = CMD_PROGRAM; //Set ISP/IAP/EEPROM PROGRAM command
	IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
	IAP_DATA = dat; //Write ISP/IAP/EEPROM data
	IAP_TRIG = 0x5a; //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5; //Send trigger command2 (0xa5)
	_nop_(); //MCU will hold here until ISP/IAP/EEPROM
	//operation complete
	IapIdle();
}
/*----------------------------
Erase one sector area
Input: addr (ISP/IAP/EEPROM address)
Output:-
----------------------------*/
void IapEraseSector(WORD addr)
{
	IAP_CONTR = ENABLE_IAP; //Open IAP function, and set wait time
	IAP_CMD = CMD_ERASE; //Set ISP/IAP/EEPROM ERASE command
	IAP_ADDRL = addr; //Set ISP/IAP/EEPROM address low
	IAP_ADDRH = addr >> 8; //Set ISP/IAP/EEPROM address high
	IAP_TRIG = 0x5a; //Send trigger command1 (0x5a)
	IAP_TRIG = 0xa5; //Send trigger command2 (0xa5)
	_nop_(); //MCU will hold here until ISP/IAP/EEPROM
	//operation complete
	IapIdle();
}

/*----------------------------
从起始位置开始存储num个数，
数据来源于str
----------------------------*/
void IapWrite(u8 *str , u16 num, WORD addr)
{
	u16 i = 0;
	IapEraseSector(addr); //Erase current sector
	for (i = 0; i< num; i++) //Program 512 bytes data into data flash
	{
		IapProgramByte(addr+i, str[i]);
	}
}

/*----------------------------
从起始位置开始读取num个数，
数据来源于str
----------------------------*/
void IapReadStr(u8 *str , u16 num, WORD addr)
{
	u16 i = 0;
    for(i=0;i<num;i++){
        str[i] = IapReadByte(addr + i);
    }
}




/********************************************************** 
用户自定义
**********************************************************/
void IapInit(void)
{
	u8 num = 0;
// 	IapReadStr(&num, 1, IAP_ADDRESS);
// 	
// 	if (num > 10) num = 0;
	
	IapWrite(&num, 1, IAP_ADDRESS);
}