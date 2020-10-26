#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "stm8l15x.h"

#include "LCD5110.h"

void Display_coordinate (u8 longy,u8 offset);

void Display_tempdot(u16 temp,u8 zerolevel);

#endif

