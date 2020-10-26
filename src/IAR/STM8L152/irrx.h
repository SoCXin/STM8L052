#include "stm8l15x.h"



#ifndef __IR_RX_H
#define __IR_RX_H

#define IRUSERCODE       0x00//0x0B //

#define IRKEYPOWER      0x45
#define IRKEYMEMO       0x09
#define IRKEYCLOCK      0x09
#define IRKEYSLEEP      0x21
#define IRKEYUP         0x15//0x41//  
#define IRKEYDOWN       0x07//0xC3//






extern _Bool	Flag_ir;

extern u8	IR_temp[4];



extern void IR_Init(void);
void IR_rx(void);










#endif

