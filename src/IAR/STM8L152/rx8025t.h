#ifndef __RX8025T_H__
#define __RX8025T_H__


#include "stm8l15x.h"
#include "stm8l15x_i2c.h"


#define   RX8025_SECO_ADDR         0x00
#define   RX8025_MINU_ADDR         0x01
#define   RX8025_HOUR_ADDR         0x02
///#define   WEEK_ADDR         0x03  周 实际没有用到
#define   RX8025_DAYS_ADDR         0x04
#define   RX8025_MONT_ADDR         0x05
#define   RX8025_YEAR_ADDR         0x06

#define   RX8025_MODE_ADDR         0x0e   ///12/24小时制设置地址


typedef enum
{
    HOURS_12 = 0x00,///默认12小时制
    HOURS_24 = 0x20,///24小时制
} HOUR_MODE;


extern u8  R8025_DataBuf[];


 void Rtc8025_ReadData(unsigned char *buffer,unsigned char addr,unsigned char num);
  void Rtc8025_WriteData(unsigned char *buffer,unsigned char addr,unsigned char num);
 
void R8025AC_Init(HOUR_MODE the_mode);
void R8025AC_Read(u8 sadd,  u8 *buf, u8 len); 
void R8025AC_Write(u8 sadd, u8 *buf, u8 len);
 void Rtc8025_Init(void);



#endif 