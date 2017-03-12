#ifndef __EEPROM_H
#define __EEPROM_H

#include "config.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;


/*Declare SFR associated with the IAP */
// sfr IAP_DATA = 0xC2; //Flash data register
// sfr IAP_ADDRH = 0xC3; //Flash address HIGH
// sfr IAP_ADDRL = 0xC4; //Flash address LOW
// sfr IAP_CMD = 0xC5; //Flash command register
// sfr IAP_TRIG = 0xC6; //Flash command trigger
// sfr IAP_CONTR = 0xC7; //Flash control register
/*Define ISP/IAP/EEPROM command*/
#define CMD_IDLE 0 //Stand-By
#define CMD_READ 1 //Byte-Read
#define CMD_PROGRAM 2 //Byte-Program
#define CMD_ERASE 3 //Sector-Erase
/*Define ISP/IAP/EEPROM operation const for IAP_CONTR*/
//#define ENABLE_IAP 0x80 //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81 //if SYSCLK<24MHz
#define ENABLE_IAP 0x82 //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83 //if SYSCLK<12MHz
//#define ENABLE_IAP 0x84 //if SYSCLK<6MHz
//#define ENABLE_IAP 0x85 //if SYSCLK<3MHz
//#define ENABLE_IAP 0x86 //if SYSCLK<2MHz
//#define ENABLE_IAP 0x87 //if SYSCLK<1MHz
//Start address for STC12C5A60S2 series EEPROM
#define IAP_ADDRESS 0x0000



void IapIdle();
BYTE IapReadByte(WORD addr);
void IapProgramByte(WORD addr, BYTE dat);
void IapEraseSector(WORD addr);
void IapWrite(u8 *str , u16 num, WORD addr);
void IapReadStr(u8 *str , u16 num, WORD addr);

/********************************************************** 
用户自定义
**********************************************************/
void IapInit(void);
#endif