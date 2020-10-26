#ifndef _STTS751_H_
#define _STTS751_H_

#include "stm8l15x.h"

#define 	STTS751_ADDRESS 0x76

#define 	STTTEMP_H			0X00
#define 	STTSTATUS			0X01
#define 	STTTEMP_L			0X02
#define 	STTCONFIG			0X03
#define 	STTCONER_RATE		0X04
#define 	STTTEMPLIMIT_HH		0X05
#define 	STTTEMPLIMIT_HL		0X06
#define 	STTTEMPLIMIT_LH		0X07
#define 	STTTEMPLIMIT_LL		0X08
#define 	STTONESHOT			0X0F
#define 	STTTHERMLIMIT		0X20
#define 	STTTHERMHYSTER		0X21
#define 	STTPID				0XFD
#define 	STTMID				0XFE
#define 	STTREVNUM			0XFF



extern  int temp10;


u16 STTSRead(u8 reg);




#endif 