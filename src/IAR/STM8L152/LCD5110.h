#ifndef _LCD5110_H_
#define _LCD5110_H_

/*--------�޸����� <5110�˿ں����ź궨��> ���Ըı䵥Ƭ����������--------*/
/*------------���Ҫ��ֲ��������Ƭ����ֻ�����޸�LCD5110.c--------------*/
/*------------�ļ���LcdPortInit�����Ե�Ƭ�����ŵĳ�ʼ��---------------*/
/*--------------5110��VCCΪ3Vʱ����ʾ����ʱ�����޸�------------------*/
/*-------------------LCD5110.c��LcdInit����-------------------------*/

//#include "stm32f0xx.h"
//#include "stm32f0xx_gpio.h"

#include "stm8l15x.h"

//#include "Delay.h"
#include "Font6_8.h"
#include "Icon10_8.h"
#include "Font8_16.h"
#include "Font16_16.h"
#include "Bitmap.h"


/*------������������ʾʹ��buf��ע�͵���ʾ��ʹ��buf-----*/
#define USE_BUF
/*------������������ʾʹ��buf��ע�͵���ʾ��ʹ��buf-----*/

#define u8 		unsigned char
#define u16 	unsigned short

typedef struct 
{
	u8 x;
	u8 y;
}point;

/*------5110��Ļ�ߴ�͹��ܺ궨��------*/
#define DATA	1																//����
#define CMD 0																	//����
#define LCD_X	84															//Һ������������
#define LCD_ROW LCD_X													//Һ�����п��
#define LCD_Y	48															//Һ����������߶�
#define LCD_COLUMN LCD_Y / 8									//Һ�����и߶�
#define LCD_AREA LCD_X * LCD_Y								//Һ�������
#define LCD_AREA_BYTE LCD_COLUMN * LCD_ROW		//Һ�����ֽ�����

/*------λ�궨��--------*/
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

/*------5110�˿ں����ź궨��-------

#define LCD5110_PORT 	GPIOB					//5110���ڶ˿�
#define LCD_SCLK_BIT 	BIT3					//SCLK�˿�λ
#define LCD_SDIN_BIT 	BIT4					//SDIN�˿�λ
#define LCD_DC_BIT 	 	BIT5					//DC�˿�λ
#define LCD_SCE_BIT		BIT6					//SCE�˿�λ
#define LCD_REST_BIT 	BIT7					//REST�˿�λ

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



/*------5110���Ŷ����궨��-----------*/ 
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

//  #define RCC_LCD5110_PORT 	RCC_AHBENR_GPIOAEN << (((uint32_t)LCD5110_PORT-(uint32_t)GPIOA) >> 10)			//5110 RCCʹ��

/*--------------------------------------------------------------------------------------------------------------*/
/*-------�����ֲ�ʹ��buf��ʹ��buf����ʹ��buf��죬�����в����������꣬����5110������еĻ����϶���ģ�----------*/
/*-------����ͼ���ĸ߶ȱ�����8�ı���������ͼ��������������е��ӡ�ʹ��buf�Ļ�������д�ֻ��ǻ�ͼ��û��-----------*/
/*-------�������ƣ�ͼ����Ⱥ͸߶�Ҳû�����ƣ����ҿ��Խ���ͼ�����ַ��ĵ��ӡ�ʹ��buf�ĺ��������࣬һ��------------*/
/*-------��ֻ��buf�������󲿷���tobuf,ofbuf,buf��β����PutChartoBuf��PutCircletoBuf��ClearBuf����һ��-----------*/
/*-------�������������Ҫ����Refresh��RefreshAll������������buf�ı仯ˢ�µ�5110�ϣ���������ͨ����-----------*/
/*-------��ˢ�´������ӿ컭ͼ�ٶȡ���һ���Ƕ�buf�������Զ������ˢ�µ�5110�ϣ��󲿷���buf��ͷ����BufPutChar-----*/
/*-------��BufPutHanzi�ȣ����л��㻭�߻�Բ����PutLine��PutRect�ȡ�----------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////*				����Ϊ��ʹ�û�����д�ֻ�ͼ����غ���				*//////
//*--------------------------------------------------------*//
/////////////////////////////////////////////////////////////////////////////////////////////////////


//*******				�����ǻ��������ͳ�ʼ������				*****//

/*----------��ʼ��5110-----------*/
void InitLcd(void);
/*-------�������5110��Ļ--------*/
void LcdClearAll(void);
/*---����5110�к�������----*/
void SetXY(u8 row,u8 column);

//******				�����Ǹ߶�Ϊ8�ĸ��ֲ�������				*******//
//**			����6*8�ַ����ַ�����6*8���ţ�10*8ͼ��			  **//
//--every row contains 14 characters,there are 6 rows (font = 6 * 8)--//
		//-----------------   	row		  ---------------------//
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 0
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 1
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 2
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 3
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 4
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		column = 5
//-------------------------------------------------------//

/*---ֱ��дһ��ASCII�ַ�(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteChar(u8 value);
/*---ֱ��дһ��ASCII�ַ�(6*8)��5110---*/
void PutChar(u8 value,u8 column,u8 row);
/*---ֱ��дһ���ַ���(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteStr(char * str);
/*--ֱ��дһ���ַ���(6*8)��5110--*/
void PutStr(char * str,u8 column,u8 row);
/*---��sign(6*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteSign(char * sign);
/*----��sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row);
/*---��Icon(10*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteIcon(char * icon);
/*--��Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row);

//********				�����Ǹ߶�Ϊ16�ĸ��ֲ�������				*******//
//**		    	    ����8*16�ַ����ַ�����16*16����		           **//
//--every row contains 10 characters,there are 3 rows (font = 8 * 16)--//
//-----------       row       ------------//
		//0 1 2 3 ... 81 82 83//	column = 0
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 1
		//0 1 2 3 ... 81 82 83//
		//0 1 2 3 ... 81 82 83//	column = 2
		//0 1 2 3 ... 81 82 83//
//-----------------------------------------//

/*-----ֱ��дһ��ASCII�ַ�(8*16)��5110------*/
void PutWideChar(u8 value,u8 column,u8 row);
/*--ֱ��дһ���ַ���(6*8)��5110���Զ����У�row(0~76)--*/
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

/*-----ֱ��дһ������(16*16)��5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row);
/*--дһ������(16*16),�Զ�����,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num);

//********				�����ǻ�ͼ�ĸ��ֲ�������				*********//
//**		    	      ����84*48,�߶�Ϊ8�ı�����ͼƬ		       **//
//---------------   	x		  ----------------------//
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 0
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 1
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 2
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 3
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 4
		//0 1 2 3 4 5 6 7 8 9 10 11 12 13//		y = 5
//-----------------------------------------------//

/*---��picture(84*48)��ȫ��---*/
void DrawPicture(char bitmap[]);
/*---��picture(�߶ȱ�����8�ı���)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height);

#ifdef USE_BUF	/*USE_BUF*/

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////*				����Ϊʹ�û�����д�ֻ�ͼ����غ���				*//////
//*-------------��������ͳһΪ����----------------*//
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
//*********				����Ϊˢ�£�����Ȼ�����������			*********//
///////*------------------------------------------*////////

/*-----���û�����ָ������-------*/
void SetBufPtr(u8 x,u8 y);
/*----������������ˢ�µ�5110��-----*/
void RefreshAll(void);
/*-------��������ָ������ˢ�µ�5110��Ӧ����-------*/
void Refresh(u8 x,u8 y,u8 width,u8 height);
/*------�������������-------*/
void ClearAllBuf(void);
/*------���������������ˢ��5110------*/
void BufClearAll(void);
/*-------��ջ�����ָ������-------*/
void ClearBuf(u8 x,u8 y,u8 width,u8 height);
/*-------��ջ�����ָ������ˢ��------*/
void BufClear(u8 x,u8 y,u8 width,u8 height);

///////*------------------------------------------*////////
//*********				����Ϊд�ַ������ֺ���				*********//
///////*------------------------------------------*////////


//******        �����ַ��߶Ⱦ�Ϊ8              ******//

/*-------дһ��ASCII�ַ�(6*8)��������(����Ҫ���꣬�����ϸ��ַ�)--------*/
void WriteChartoBuf(u8 value);
/*--------ͨ��������дһ��ASCII�ַ�(6*8)��5110(����Ҫ���꣬�����ϸ��ַ�)--------*/
void BufWriteChar(u8 value);
/*----дһ��ASCII�ַ�(6*8)��������-----*/
void PutChartoBuf(u8 value,u8 x,u8 y);
/*----ͨ��������дһ��ASCII�ַ�(6*8)��5110-----*/
void BufPutChar(u8 value,u8 x,u8 y);
/*------дһ���ַ���(�߶�8)��������(����Ҫ���꣬�����ϸ��ַ�)-----*/
u8 WriteStrtoBuf(char * str);
/*----ͨ��������дһ���ַ���(�߶�8)��5110(����Ҫ���꣬�����ϸ��ַ�)----*/
u8 BufWriteStr(char * str);
/*-----дһ���ַ���(�߶�8)��������------*/
u8 PutStrtoBuf(char * str,u8 x,u8 y);
/*------ͨ��������дһ���ַ���(�߶�8)��5110------*/
u8 BufPutStr(char * str,u8 x,u8 y);

//*******        �����ַ����ָ߶Ⱦ�Ϊ16          ********//

/*------дһ��ASCII�ַ�(8*16)��������------*/
void PutWideChartoBuf(u8 value,u8 x,u8 y);
/*----ͨ��������дһ��ASCII�ַ�(8*16)��5110------*/
void BufPutWideChar(u8 value,u8 x,u8 y);
/*-----дһ���ַ���(�߶�16)��������------*/
u8 PutWideStrtoBuf(char * str,u8 x,u8 y);
/*------ͨ��������д�ַ���(�߶�16)��5110------*/
u8 BufPutWideStr(char *str,u8 x,u8 y);
/*-----дһ������(16*16)��������------*/
void PutHanzitoBuf(char Hanzi[],u8 x,u8 y);
/*-----ͨ��������дһ������(16*16)��5110-------*/
void BufPutHanzi(char Hanzi[],u8 x,u8 y);
/*-------дһ������(�߶�16)��������(numΪ���ָ���)-------*/
u8 PutHanziStrtoBuf(char Hanzi[],u8 x,u8 y,u8 num);
/*-----ͨ��������дһ������(�߶�16)��5110(numΪ���ָ���)------*/
u8 BufPutHanziStr(char Hanzi[],u8 x,u8 y,u8 num);

//*********    ����Ϊ���㣬�ߣ����ߣ����Σ�Բ��bmpͼƬ����      *********//

/*------��һ���㵽������-------*/
void PutPointtoBuf(u8 x,u8 y);
/*------ͨ����������һ���㵽5110------*/
void PutPoint(u8 x,u8 y);
/*------��һ���ߵ�������--------*/		
void PutLinetoBuf(u8 x1,u8 y1,u8 x2,u8 y2);
/*-----ͨ����������һ���ߵ�5110-------*/	
void PutLine(u8 x1,u8 y1,u8 x2,u8 y2);
/*--�����ߵ�������(pΪ���߸����ڵ㣬line_numΪ���ߵ��߶���Ŀ)--*/
void PutPolylinetoBuf(point *p,u8 line_num);
/*--ͨ�������������ߵ�5110(pΪ���߸����ڵ㣬line_numΪ���ߵ��߶���Ŀ)--*/
void PutPolyline(point *p,u8 line_num);
/*---------�����ε�������--------*/
void PutRecttoBuf(u8 x,u8 y,u8 width,u8 height);
/*--------ͨ�������������ε�5110--------*/
void PutRect(u8 x,u8 y,u8 width,u8 height);
/*--------��Բ��������--------*/
void PutCircletoBuf(u8 center_x,u8 center_y,u8 r);
/*--------ͨ����������Բ��5110--------*/
void PutCircle(u8 center_x,u8 center_y,u8 r);
/*----��һ��ͼƬ��������-----*/
void DrawBmptoBuf(char bitmap[],u8 x,u8 y,u8 width,u8 height);
/*-----ͨ����������һ��ͼƬ��5110------*/
void BufDrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height);

extern u8 lcd_buf[LCD_COLUMN][LCD_ROW];

#endif /*USE_BUF*/

#endif /*LCD5110_H*/
