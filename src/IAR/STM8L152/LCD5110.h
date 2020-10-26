#ifndef _LCD5110_H_
#define _LCD5110_H_

/*--------修改下面 <5110端口和引脚宏定义> 可以改变单片机引脚连线--------*/
/*------------如果要移植到其他单片机上只需再修改LCD5110.c--------------*/
/*------------文件中LcdPortInit函数对单片机引脚的初始化---------------*/
/*--------------5110的VCC为3V时或显示不清时可以修改------------------*/
/*-------------------LCD5110.c中LcdInit函数-------------------------*/

//#include "stm32f0xx.h"
//#include "stm32f0xx_gpio.h"

#include "stm8l15x.h"

//#include "Delay.h"
#include "Font6_8.h"
#include "Icon10_8.h"
#include "Font8_16.h"
#include "Font16_16.h"
#include "Bitmap.h"


/*------保留下面这句表示使用buf，注释掉表示不使用buf-----*/
#define USE_BUF
/*------保留上面这句表示使用buf，注释掉表示不使用buf-----*/

#define u8 		unsigned char
#define u16 	unsigned short

typedef struct 
{
	u8 x;
	u8 y;
}point;

/*------5110屏幕尺寸和功能宏定义------*/
#define DATA	1																//数据
#define CMD 0																	//命令
#define LCD_X	84															//液晶屏横坐标宽度
#define LCD_ROW LCD_X													//液晶屏列宽度
#define LCD_Y	48															//液晶屏纵坐标高度
#define LCD_COLUMN LCD_Y / 8									//液晶屏行高度
#define LCD_AREA LCD_X * LCD_Y								//液晶屏面积
#define LCD_AREA_BYTE LCD_COLUMN * LCD_ROW		//液晶屏字节容量

/*------位宏定义--------*/
#define BIT0 	0x00000001
#define BIT1 	0x00000002
#define BIT2 	0x00000004
#define BIT3 	0x00000008
#define BIT4 	0x00000010
#define BIT5 	0x00000020
#define BIT6 	0x00000040
#define BIT7 	0x00000080
#define BIT8 	0x00000100
#define BIT9 	0x00000200
#define BIT10 0x00000400
#define BIT11 0x00000800
#define BIT12 0x00001000
#define BIT13 0x00002000
#define BIT14 0x00004000
#define BIT15 0x00008000

/*------5110端口和引脚宏定义-------

#define LCD5110_PORT 	GPIOB					//5110所在端口
#define LCD_SCLK_BIT 	BIT3					//SCLK端口位
#define LCD_SDIN_BIT 	BIT4					//SDIN端口位
#define LCD_DC_BIT 	 	BIT5					//DC端口位
#define LCD_SCE_BIT		BIT6					//SCE端口位
#define LCD_REST_BIT 	BIT7					//REST端口位

-*/

  
        
        
#define NLCD_CE_PORT   GPIOE
#define NLCD_CE_PIN    GPIO_Pin_5
#define NLCD_DC_PORT  GPIOD
#define NLCD_DC_PIN    GPIO_Pin_0
#define NLCD_DIN_PORT  GPIOD
#define NLCD_DIN_PIN   GPIO_Pin_1
#define NLCD_SCLK_PORT GPIOD
#define NLCD_SCLK_PIN  GPIO_Pin_2
#define NLCD_RST_PORT GPIOE
#define NLCD_RST_PIN   GPIO_Pin_4



/*------5110引脚动作宏定义-----------*/ 
#define LCD_SCLK_H      GPIO_SetBits(NLCD_SCLK_PORT,   NLCD_SCLK_PIN)
#define LCD_SCLK_L      GPIO_ResetBits (NLCD_SCLK_PORT,   NLCD_SCLK_PIN)//LCD_DC = 0;
#define LCD_SDIN_H	GPIO_SetBits(NLCD_DIN_PORT,   NLCD_DIN_PIN)
#define LCD_SDIN_L      GPIO_ResetBits (NLCD_DIN_PORT,   NLCD_DIN_PIN)//LCD_RST = 0;

#define LCD_DC_H	GPIO_SetBits(NLCD_DC_PORT,   NLCD_DC_PIN)
#define LCD_DC_L  	GPIO_ResetBits (NLCD_DC_PORT,   NLCD_DC_PIN)//LCD_DC = 0;

#define LCD_SCE_H	 GPIO_SetBits(NLCD_CE_PORT,   NLCD_CE_PIN)
#define LCD_SCE_L  	 GPIO_ResetBits (NLCD_CE_PORT,   NLCD_CE_PIN)//LCD_DC = 0;

#define LCD_REST_H	 GPIO_SetBits(NLCD_RST_PORT,   NLCD_RST_PIN)
#define LCD_REST_L      GPIO_ResetBits (NLCD_RST_PORT,   NLCD_RST_PIN)//LCD_RST = 0;

//  #define RCC_LCD5110_PORT 	RCC_AHBENR_GPIOAEN << (((uint32_t)LCD5110_PORT-(uint32_t)GPIOA) >> 10)			//5110 RCC使能

/*--------------------------------------------------------------------------------------------------------------*/
/*-------函数分不使用buf和使用buf，不使用buf最快，但是行不是任意坐标，是在5110本身的行的基础上定义的；----------*/
/*-------画的图案的高度必须是8的倍数，而且图案不能相或来进行叠加。使用buf的话不论是写字还是画图都没有-----------*/
/*-------坐标限制，图案宽度和高度也没有限制，而且可以进行图案和字符的叠加。使用buf的函数分两类，一类------------*/
/*-------是只对buf操作，大部分以tobuf,ofbuf,buf结尾比如PutChartoBuf、PutCircletoBuf、ClearBuf，这一类-----------*/
/*-------函数操作完后需要调用Refresh、RefreshAll这两个函数将buf的变化刷新到5110上，较灵活，可以通过减-----------*/
/*-------少刷新次数来加快画图速度。另一种是对buf操作后自动将结果刷新到5110上，大部分以buf开头，如BufPutChar-----*/
/*-------、BufPutHanzi等，还有画点画线画圆函数PutLine、PutRect等。----------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////*				以下为不使用缓冲区写字画图的相关函数				*//////
//*--------------------------------------------------------*//
/////////////////////////////////////////////////////////////////////////////////////////////////////


//*******				以下是基本操作和初始化函数				*****//

/*----------初始化5110-----------*/
void InitLcd(void);
/*-------清空整个5110屏幕--------*/
void LcdClearAll(void);
/*---设置5110行和列坐标----*/
void SetXY(u8 row,u8 column);

//******				以下是高度为8的各种操作函数				*******//
//**			包括6*8字符和字符串，6*8符号，10*8图标			  **//
//--every row contains 14 characters,there are 6 rows (font = 6 * 8)--//
		//-----------------   	row		  ---------------------//
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 0
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 1
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 2
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 3
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 4
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 5
//-------------------------------------------------------//

/*---直接写一个ASCII字符(6*8)到5110(不需要坐标，紧跟上个字符或图标)---*/
void WriteChar(u8 value);
/*---直接写一个ASCII字符(6*8)到5110---*/
void PutChar(u8 value,u8 column,u8 row);
/*---直接写一个字符串(6*8)到5110(不需要坐标，紧跟上个字符或图标)---*/
void WriteStr(char * str);
/*--直接写一个字符串(6*8)到5110--*/
void PutStr(char * str,u8 column,u8 row);
/*---画sign(6*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteSign(char * sign);
/*----画sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row);
/*---画Icon(10*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteIcon(char * icon);
/*--画Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row);

//********				以下是高度为16的各种操作函数				*******//
//**		    	    包括8*16字符和字符串，16*16汉字		           **//
//--every row contains 10 characters,there are 3 rows (font = 8 * 16)--//
//-----------       row       ------------//
		//0 1 2 3 ... 81 82 83//	column = 0
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 1
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 2
		//0 1 2 3 ... 81 82 83//
//-----------------------------------------//

/*-----直接写一个ASCII字符(8*16)到5110------*/
void PutWideChar(u8 value,u8 column,u8 row);
/*--直接写一个字符串(6*8)到5110，自动换行，row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row);

//--every row contains 5 characters,there are 3 rows (font = 16 * 16)--//
//----------      row       --------------//
		//0 1 2 3 ... 81 82 83//	column = 0
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 1
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 2
		//0 1 2 3 ... 81 82 83//
//----------------------------------------//

/*-----直接写一个汉字(16*16)到5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row);
/*--写一串汉字(16*16),自动换行,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num);

//********				以下是画图的各种操作函数				*********//
//**		    	      包括84*48,高度为8的倍数的图片		       **//
//---------------   	x		  ----------------------//
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 0
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 1
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 2
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 3
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 4
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 5
//-----------------------------------------------//

/*---画picture(84*48)，全屏---*/
void DrawPicture(char bitmap[]);
/*---画picture(高度必须是8的倍数)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height);

#ifdef USE_BUF	/*USE_BUF*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////*				以下为使用缓冲区写字画图的相关函数				*//////
//*-------------以下坐标统一为两种----------------*//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------   	x		  ------------------------//
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 0
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 1
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 2
		//							.
		//							.
		//							.
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 45
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 46
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		y = 47
//--------------------------------------------------------//
//-------------------  	row		  ------------------------//
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 0
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 1
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 2
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 3
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 4
		//0 1 2 3 4 5 ...78 79 80 81 82 83//		column = 5
//------------------------------------------------------//

///////*------------------------------------------*////////
//*********				以下为刷新，清除等基本操作函数			*********//
///////*------------------------------------------*////////

/*-----设置缓冲区指针坐标-------*/
void SetBufPtr(u8 x,u8 y);
/*----将整个缓冲区刷新到5110上-----*/
void RefreshAll(void);
/*-------将缓冲区指定区域刷新到5110对应区域-------*/
void Refresh(u8 x,u8 y,u8 width,u8 height);
/*------清空整个缓冲区-------*/
void ClearAllBuf(void);
/*------清空整个缓冲区并刷新5110------*/
void BufClearAll(void);
/*-------清空缓冲区指定区域-------*/
void ClearBuf(u8 x,u8 y,u8 width,u8 height);
/*-------清空缓冲区指定区域并刷新------*/
void BufClear(u8 x,u8 y,u8 width,u8 height);

///////*------------------------------------------*////////
//*********				以下为写字符，汉字函数				*********//
///////*------------------------------------------*////////


//******        以下字符高度均为8              ******//

/*-------写一个ASCII字符(6*8)到缓冲区(不需要坐标，紧跟上个字符)--------*/
void WriteChartoBuf(u8 value);
/*--------通过缓冲区写一个ASCII字符(6*8)到5110(不需要坐标，紧跟上个字符)--------*/
void BufWriteChar(u8 value);
/*----写一个ASCII字符(6*8)到缓冲区-----*/
void PutChartoBuf(u8 value,u8 x,u8 y);
/*----通过缓冲区写一个ASCII字符(6*8)到5110-----*/
void BufPutChar(u8 value,u8 x,u8 y);
/*------写一个字符串(高度8)到缓冲区(不需要坐标，紧跟上个字符)-----*/
u8 WriteStrtoBuf(char * str);
/*----通过缓冲区写一个字符串(高度8)到5110(不需要坐标，紧跟上个字符)----*/
u8 BufWriteStr(char * str);
/*-----写一个字符串(高度8)到缓冲区------*/
u8 PutStrtoBuf(char * str,u8 x,u8 y);
/*------通过缓冲区写一个字符串(高度8)到5110------*/
u8 BufPutStr(char * str,u8 x,u8 y);

//*******        以下字符汉字高度均为16          ********//

/*------写一个ASCII字符(8*16)到缓冲区------*/
void PutWideChartoBuf(u8 value,u8 x,u8 y);
/*----通过缓冲区写一个ASCII字符(8*16)到5110------*/
void BufPutWideChar(u8 value,u8 x,u8 y);
/*-----写一个字符串(高度16)到缓冲区------*/
u8 PutWideStrtoBuf(char * str,u8 x,u8 y);
/*------通过缓冲区写字符串(高度16)到5110------*/
u8 BufPutWideStr(char *str,u8 x,u8 y);
/*-----写一个汉字(16*16)到缓冲区------*/
void PutHanzitoBuf(char Hanzi[],u8 x,u8 y);
/*-----通过缓冲区写一个汉字(16*16)到5110-------*/
void BufPutHanzi(char Hanzi[],u8 x,u8 y);
/*-------写一串汉字(高度16)到缓冲区(num为汉字个数)-------*/
u8 PutHanziStrtoBuf(char Hanzi[],u8 x,u8 y,u8 num);
/*-----通过缓冲区写一串汉字(高度16)到5110(num为汉字个数)------*/
u8 BufPutHanziStr(char Hanzi[],u8 x,u8 y,u8 num);

//*********    以下为画点，线，折线，矩形，圆，bmp图片函数      *********//

/*------画一个点到缓冲区-------*/
void PutPointtoBuf(u8 x,u8 y);
/*------通过缓冲区画一个点到5110------*/
void PutPoint(u8 x,u8 y);
/*------画一条线到缓冲区--------*/		
void PutLinetoBuf(u8 x1,u8 y1,u8 x2,u8 y2);
/*-----通过缓冲区画一条线到5110-------*/	
void PutLine(u8 x1,u8 y1,u8 x2,u8 y2);
/*--画折线到缓冲区(p为折线各个节点，line_num为折线的线段数目)--*/
void PutPolylinetoBuf(point *p,u8 line_num);
/*--通过缓冲区画折线到5110(p为折线各个节点，line_num为折线的线段数目)--*/
void PutPolyline(point *p,u8 line_num);
/*---------画矩形到缓冲区--------*/
void PutRecttoBuf(u8 x,u8 y,u8 width,u8 height);
/*--------通过缓冲区画矩形到5110--------*/
void PutRect(u8 x,u8 y,u8 width,u8 height);
/*--------画圆到缓冲区--------*/
void PutCircletoBuf(u8 center_x,u8 center_y,u8 r);
/*--------通过缓冲区画圆到5110--------*/
void PutCircle(u8 center_x,u8 center_y,u8 r);
/*----画一个图片到缓冲区-----*/
void DrawBmptoBuf(char bitmap[],u8 x,u8 y,u8 width,u8 height);
/*-----通过缓冲区画一个图片到5110------*/
void BufDrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height);

extern u8 lcd_buf[LCD_COLUMN][LCD_ROW];

#endif /*USE_BUF*/

#endif /*LCD5110_H*/
