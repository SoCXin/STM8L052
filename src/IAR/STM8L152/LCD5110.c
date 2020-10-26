#include "LCD5110.h"

#ifdef USE_BUF

u8 lcd_buf[LCD_COLUMN][LCD_ROW];

point buf_ptr;
point start_ptr;

#endif /*USE_BUF*/



/*------初始化5110端口和引脚-------
void LcdPortInit(void)
{
	
  
  
  
  _InitTypeDef        GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_LCD5110_PORT, ENABLE);	//使能端口时钟
  GPIO_InitStructure.GPIO_Pin = LCD_SCLK_BIT | LCD_SDIN_BIT | LCD_DC_BIT | LCD_SCE_BIT | LCD_REST_BIT;//选中要配置的引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//引脚配置为输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//引脚配置为推拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	//配置引脚输出速度
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//引脚输出无上拉下拉
  GPIO_Init(LCD5110_PORT, &GPIO_InitStructure);
  
  
}
-*/



/*-----------------------------------------------------------------------
LCD_init          : 3310LCD初始化

编写日期          ：2004-8-10 
最后修改日期      ：2004-8-10 
-----------------------------------------------------------------------*/
void LcdPortInit(void)
{
  
    GPIO_Init(  NLCD_CE_PORT,   NLCD_CE_PIN,     GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(  NLCD_DC_PORT,   NLCD_DC_PIN,     GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(  NLCD_DIN_PORT,  NLCD_DIN_PIN,    GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(  NLCD_SCLK_PORT, NLCD_SCLK_PIN,   GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(  NLCD_RST_PORT,  NLCD_RST_PIN,    GPIO_Mode_Out_PP_High_Fast);    
    
}   
    
    
    
    
  

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////*				以下为不使用缓冲区写字画图的相关函数				*//////
//*--------------------------------------------------------*//
/////////////////////////////////////////////////////////////////////////////////////////////////////


//*******				以下是基本操作和初始化函数				*****//

/*-------写一个字节到5110(d_c决定是数据还是命令)----------*/
void LcdWrite(u8 byte,u8 d_c)
{
	u8 i;
	
	LCD_SCE_L;									//使能片选
	//Delayus(1);
	if(d_c == DATA)							//选择发送的是数据还是命令
		LCD_DC_H;
	else 
		LCD_DC_L;
	//Delayus(1);
	for(i = 0;i < 8;i ++)				//发送一个字节
	{
		LCD_SCLK_L;
		if(byte & (0x80 >> i)) 
			LCD_SDIN_H;
		else
			LCD_SDIN_L;
		//Delayus(1);
		LCD_SCLK_H;
	//	Delayus(1);
	}
	LCD_SCLK_L;
	//Delayus(1);	
	LCD_SCE_H;										//片选失效
	//Delayus(1);
}
/*----------初始化5110-----------*/
void InitLcd(void)
{
	 Delay(0x0F);			//延时1ms
	LcdPortInit();			//初始化5110端口和引脚
	LCD_REST_L;					//发送复位信号初始化5110内部寄存器
	Delay(1);
	LCD_REST_H;
	 Delay(0x0F);
	//LCD_SCE_L;				//使能片选
	//Delayus(1);
	//LCD_SCE_H;				//片选失效
	//Delayus(1);
	//拓展指令(function set PD = 0 and V = 0, select extended instruction set(H = 1 mode))
	LcdWrite(0x21,CMD);
	//设置液晶电压(set VOP: VOP is set to a + 65 × b [V])
	LcdWrite(0xc8,CMD);				//VCC接5V
	//LcdWrite(0xbf,CMD);			  //VCC接3V
	//温度校正
	LcdWrite(0x06,CMD);
	//设置混合率(RECOMMENDED MUX RATE(1:10),you can chose other MUX RATE)
	LcdWrite(0x13,CMD);				//VCC接5V
	//LcdWrite(0x17,CMD);			  //VCC接3V
	//标准指令(function set PD = 0 and V = 0, select normal instruction set(H = 0 mode))
	LcdWrite(0x20,CMD);
	//标准显示模式(display control set normal mode (D=1andE=0))
	LcdWrite(0x0c,CMD);
	//清屏
	LcdClearAll();				
}
/*---设置5110行和列坐标----*/
void SetXY(u8 row,u8 column)
{
	if(row >= LCD_ROW) row = 0;						//纠正错误列输入
	if(column >= LCD_COLUMN) column = 0;	//纠正错误行输入
	
	LcdWrite(0x40 | column,CMD);					//设置行坐标
	LcdWrite(0x80 | row,CMD);							//设置列坐标
}

/*-------清空整个5110屏幕--------*/
void LcdClearAll(void)
{
	u16 i;
	
	SetXY(0,0);									//坐标回到原点
	for(i = 0;i < LCD_AREA_BYTE;i ++)	
	{
		LcdWrite(0,DATA);					//写0到5110以清除该字节数据
	}
}

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
void WriteChar(u8 value)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(ASCII_6_8[value - 0x20][i],DATA);
}
/*---直接写一个ASCII字符(6*8)到5110---*/
void PutChar(u8 value,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteChar(value);
}
/*---直接写一个字符串(6*8)到5110(不需要坐标，紧跟上个字符或图标)---*/
void WriteStr(char * str)
{
	while(*str)
	{
		WriteChar(*str ++);
	}
}
/*--直接写一个字符串(6*8)到5110--*/
void PutStr(char * str,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteStr(str);
}
/*---画sign(6*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteSign(char * sign)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(sign[i],DATA);
}
/*----画sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row)
{
	SetXY(row,column);
	WriteSign(sign);
}
/*---画Icon(10*8)(不需要坐标，紧跟上个字符或图标)---*/
void WriteIcon(char * icon)
{
	u8 i;
	for(i = 0;i < 10;i ++)
	LcdWrite(icon[i],DATA);
}
/*--画Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row)
{
	if(row > (LCD_ROW - 10))//如果需要换行
	{
		row = 0;
		column += 1;
	}
	SetXY(row,column);
	WriteIcon(icon);
}

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
void PutWideChar(u8 value,u8 column,u8 row)
{
	u8 i;
	//写上半行
	SetXY(row,column << 1);
	for(i = 0;i < 8;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
	//写下半行
	SetXY(row,(column << 1) + 1);
	for(i = 8;i < 16;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
}
/*--直接写一个字符串(6*8)到5110，自动换行，row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row)
{
	u8 i = 0;
	
	while(*str)
	{	
		if(row + (i << 3) > (LCD_ROW - 8))//如果需要换行
		{
			row = 0;
			i = 0;
			column += 1;
		}
		PutWideChar(*str,column,row + (i << 3));
		str++;
		i++;
	}
}

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
void PutHanzi(char Hanzi[],u8 column,u8 row)
{
	u8 i;
	//写上半行
	SetXY(row,column << 1);
	for(i = 0;i < 16;i ++)
	LcdWrite(Hanzi[i],DATA);
	//写下半行
	SetXY(row,(column << 1) + 1);
	for(i = 16;i < 32;i ++)
		LcdWrite(Hanzi[i],DATA);
}
/*--写一串汉字(16*16),自动换行,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num)
{
	u8 num_i,i = 0;
	for(num_i = 0;num_i < num;num_i ++)
	{
		if(row + (num_i << 4) > (LCD_ROW - 16))	//如果需要换行
		{
			row = 0;
			i = 0;
			column += 1;
		}
		PutHanzi(Hanzi + (num_i << 5),column,row + (i << 4));
		i ++;
	}
}

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
void DrawPicture(char bitmap[])
{
	u16 i;
	for(i = 0;i < LCD_AREA_BYTE;i ++)
		LcdWrite(bitmap[i],DATA);
}

/*---画picture(高度必须是8的倍数)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	height = (height >> 3);
	//画第height_i行
	for(height_i = 0;height_i < height;height_i ++)
	{
		SetXY(x,y + height_i);
		//画该行的width列
		for(width_i = 0;width_i < width;width_i ++)
		{
			LcdWrite(bitmap[width_i + height_i * width],DATA);
		}
	}
}
#ifdef USE_BUF
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

/*----获得一个8位整型数的绝对值----*/
u8 Abs(char a)
{
	if((u8)a >= 128)
		return 256 - a;
	else 
		return a;
}
/*----交换两个8位整型数----*/
void Swap(u8 *a,u8 *b)
{
	u8 temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
}
/*-----设置缓冲区指针坐标-------*/
void SetBufPtr(u8 x,u8 y)
{
	if(y >= LCD_Y) y= 0;				//纠正错误纵坐标输入
	if(x >= LCD_X) x = 0;				//纠正错误横坐标输入
	
	buf_ptr.y = y;							//设置纵坐标
	buf_ptr.x = x;							//设置横坐标
}
/*----将整个缓冲区刷新到5110上-----*/
void RefreshAll(void)
{
	u16 i,j;
	SetXY(0,0);
	for(i = 0;i < LCD_COLUMN;i ++)
		for(j = 0;j < LCD_X;j++)
		LcdWrite(lcd_buf[i][j],DATA);
}
/*-------将缓冲区指定区域刷新到5110对应区域-------*/
void Refresh(u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	u8 start_row,start_column,end_row,end_column;
	start_row = y >> 3;					//刷新起始行
	start_column = x;						//刷新起始列
	end_row = (y + height - 1) >> 3 ;//刷新终止行
	end_column = x + width - 1;				//刷新终止列
	
	//刷新第height_i行
	for(height_i = start_row;height_i <= end_row;height_i ++)		//刷新指定行
	{
		SetXY(start_column,height_i);				//从行首开始刷新
		//刷新该行的width列
		for(width_i = start_column;width_i <= end_column;width_i ++)	//刷新每行的指定列
		{
			LcdWrite(lcd_buf[height_i][width_i],DATA);
		}
	}
}
/*----清除缓冲区1字节数据----*/
void ClearPointofBuf(u8 x,u8 y)
{
	u8 column,column_bit;
	
	SetBufPtr(x,y);
	column = buf_ptr.y >> 3;							//要清除的缓冲区的字节行坐标
	column_bit = buf_ptr.y % 8;					//坐标在该字节8bit的bit位置
	lcd_buf[column][buf_ptr.x] &= ~(0x01 << (column_bit));	//清除buf_ptr指向的缓冲区字节
	if(column_bit) lcd_buf[column + 1][buf_ptr.x] &= ~(0xff >> (8 - column_bit));//字节跨两行就清除下一行
}
/*------写一个字节数据到缓冲区-------*/
void WriteBytetoBuf(u8 byte)
{
	u8 column,column_bit;
	
	column = buf_ptr.y >> 3;							//要写的缓冲区的字节行坐标
	column_bit = buf_ptr.y % 8;					//坐标在该字节8bit的bit位置
	lcd_buf[column][buf_ptr.x] |= byte << (column_bit);	//将1字节数据写入buf_ptr指向的缓冲区字节
	if(column_bit) lcd_buf[column + 1][buf_ptr.x] |= byte >> (8 - column_bit);//字节跨两行就写下一行
	if(buf_ptr.x >= LCD_X - 1)				//到达行尾缓冲区指针自动跳转到下一行行首
	{
		buf_ptr.x = 0;
		buf_ptr.y += 8;
		if(buf_ptr.y >= LCD_Y)				  //到达页面末尾，缓冲区指针跳到页面开头
			buf_ptr.y = 0;
	}
	else 
		buf_ptr.x++;										//缓冲区指针指向下一列
}

/*------清空整个缓冲区-------*/
void ClearAllBuf(void)
{
	u8 i,j;
	u8 y = 0;
	
	for(j = 0;j < LCD_COLUMN;j++)
	{
		for(i = 0;i < LCD_ROW;i++)
		{
			lcd_buf[j][i] = 0x00; 			//写0到缓冲区以清除该字节数据
		}
		y += 8;
	}
}
/*------清空整个缓冲区并刷新5110------*/
void BufClearAll(void)
{
	ClearAllBuf();							//清除缓冲区
	RefreshAll();								//刷新
}
/*-------清空缓冲区指定区域-------*/
void ClearBuf(u8 x,u8 y,u8 width,u8 height)
{
	u8 i,j;
	
	height = y + height;								//获得刷新的终止纵坐标
	width = width + x;									//获得刷新的终止横坐标
	for(j = y;j < height;j++)
	{
		for(i = x;i < width;i++)
		{
			ClearPointofBuf(i,j);						//写0到缓冲区以清除该字节数据
		}
	}
}
/*-------清空缓冲区指定区域并刷新------*/
void BufClear(u8 x,u8 y,u8 width,u8 height)
{
		ClearBuf(x,y,width,height);				//清空缓冲区指定区域
		Refresh(x,y,width,height);				//刷新
}


///////*------------------------------------------*////////
//*********				以下为写字符，汉字函数				*********//
///////*------------------------------------------*////////


//******        以下字符高度均为8              ******//

/*-------写一个ASCII字符(6*8)到缓冲区(不需要坐标，紧跟上个字符)--------*/
void WriteChartoBuf(u8 value)
{
	u8 i;
	
	start_ptr.x = buf_ptr.x;					//获得开始横坐标
	start_ptr.y = buf_ptr.y;					//获得开始纵坐标
	for(i = 0;i < 6;i ++)							//连续写六个字节
		WriteBytetoBuf(ASCII_6_8[value - 0x20][i]);
}
/*--------通过缓冲区写一个ASCII字符(6*8)到5110(不需要坐标，紧跟上个字符)--------*/
void BufWriteChar(u8 value)
{
	WriteChartoBuf(value);
	Refresh(start_ptr.x,start_ptr.y,6,8);			//刷新
}
/*----写一个ASCII字符(6*8)到缓冲区-----*/
void PutChartoBuf(u8 value,u8 x,u8 y)
{
	SetBufPtr(x,y);											//设置打印坐标
	WriteChartoBuf(value);
}
/*----通过缓冲区写一个ASCII字符(6*8)到5110-----*/
void BufPutChar(u8 value,u8 x,u8 y)
{
	PutChartoBuf(value,x,y);
	Refresh(start_ptr.x,start_ptr.y,6,8);
}	
/*------写一个字符串(高度8)到缓冲区(不需要坐标，紧跟上个字符)-----*/
u8 WriteStrtoBuf(char * str)
{
	u8 str_len = 0;
	
	while(*str)
	{
		WriteChartoBuf(*str ++);
		str_len++;
	}
	return str_len;										//返回字符串长度
}
/*----通过缓冲区写一个字符串(高度8)到5110(不需要坐标，紧跟上个字符)----*/
u8 BufWriteStr(char * str)
{
	u8 y,str_len,height;
	
	str_len = WriteStrtoBuf(str);
	height = (char)((str_len * 6 - start_ptr.x - 6) / LCD_X) + 2; //获得刷新高度
	y = start_ptr.y - 8 * (height - 1);		//获得字符串起始纵坐标
	Refresh(0,y,LCD_X,8 * height);
	return str_len;												//返回字符串长度
}
/*-----写一个字符串(高度8)到缓冲区------*/
u8 PutStrtoBuf(char * str,u8 x,u8 y)
{
	SetBufPtr(x,y);
	return WriteStrtoBuf(str);						//返回字符串长度
}
/*------通过缓冲区写一个字符串(高度8)到5110------*/
u8 BufPutStr(char * str,u8 x,u8 y)
{
	u8 str_len,height;
	
	str_len = PutStrtoBuf(str,x,y);
	height = (char)((str_len * 6 - (LCD_X - x) - 1) / LCD_X) + 2;//获得刷新高度
	Refresh(0,y,LCD_X,8 * height);
	return str_len;												//返回字符串长度
}

//*******        以下字符汉字高度均为16          ********//

/*------写一个ASCII字符(8*16)到缓冲区------*/
void PutWideChartoBuf(u8 value,u8 x,u8 y)
{
	u8 i;
	//写上面一行
	SetBufPtr(x,y);
	for(i = 0;i < 8;i ++)
		WriteBytetoBuf(ASCII_8_16[value - 0x20][i]);
	//写下面一行
	SetBufPtr(x,y + 8);
	for(i = 8;i < 16;i ++)
		WriteBytetoBuf(ASCII_8_16[value - 0x20][i]);
}
/*----通过缓冲区写一个ASCII字符(8*16)到5110------*/
void BufPutWideChar(u8 value,u8 x,u8 y)
{
	PutWideChartoBuf(value,x,y);
	Refresh(x,y,8,16);
}
/*-----写一个字符串(高度16)到缓冲区------*/
u8 PutWideStrtoBuf(char * str,u8 x,u8 y)
{
	u8 i = 0;
	u8 column = 1;
	
	while(*str)
	{
		//如果需要换行，进行坐标的自动换行
		if(x + (i << 3) > (LCD_X - 8))
		{
			x = 0;
			i = 0;
			y += 16;
			column++;			//刷新行数加1
		}
		//写一个char
		PutWideChartoBuf(*str,x + (i << 3),y);
		str++;
		i++;
	}
	return column;	//返回字符串长度
}
/*------通过缓冲区写字符串(高度16)到5110------*/
u8 BufPutWideStr(char *str,u8 x,u8 y)
{
	u8 column;
	
	column = PutWideStrtoBuf(str,x,y);			//获得刷新的行数
	Refresh(x,y,LCD_X,16 << (column - 1));
	return column;
}
/*-----写一个汉字(16*16)到缓冲区------*/
void PutHanzitoBuf(char Hanzi[],u8 x,u8 y)
{
	u8 i;
	//写上面一行
	SetBufPtr(x,y);
	for(i = 0;i < 16;i ++)
		WriteBytetoBuf(Hanzi[i]);
	//写下面一行
	SetBufPtr(x,y + 8);
	for(i = 16;i < 32;i ++)
		WriteBytetoBuf(Hanzi[i]);
}
/*-----通过缓冲区写一个汉字(16*16)到5110-------*/
void BufPutHanzi(char Hanzi[],u8 x,u8 y)
{
	PutHanzitoBuf(Hanzi,x,y);
	Refresh(x,y,16,16);
}
/*-------写一串汉字(高度16)到缓冲区(num为汉字个数)-------*/
u8 PutHanziStrtoBuf(char Hanzi[],u8 x,u8 y,u8 num)
{
	u8 num_i,i = 0;
	u8 column = 1;
	for(num_i = 0;num_i < num;num_i ++)
	{
		//如果需要换行，进行坐标换行
		if(x + (num_i << 4) > (LCD_X - 16))
		{
			x = 0;
			i = 0;
			y += 16;
			column++;
		}
		//写一个汉字
		PutHanzitoBuf(Hanzi + (num_i << 5),x + (i << 4),y);
		i ++;
	}
	return column;					//返回刷新行数
}
/*-----通过缓冲区写一串汉字(高度16)到5110(num为汉字个数)------*/
u8 BufPutHanziStr(char Hanzi[],u8 x,u8 y,u8 num)
{
	u8 column;
	column = PutHanziStrtoBuf(Hanzi,x,y,num);
	Refresh(x,y,LCD_X,16 << (column - 1));
	return column;
}	
	
//*********    以下为画点，线，折线，矩形，圆，bmp图片函数      *********//

/*------画一个点到缓冲区-------*/
void PutPointtoBuf(u8 x,u8 y)
{
	u8 row;
	
	row = y >> 3;													//获得点所在的行
	lcd_buf[row][x] |= 0x01 << (y % 8);
}
/*------通过缓冲区画一个点到5110------*/
void PutPoint(u8 x,u8 y)
{
	PutPointtoBuf(x,y);
	Refresh(x,y,1,8);
} 
/*-----获得两数中较小的数------*/
u8 Min(u8 a,u8 b)
{
	return a <= b? a : b;
}
/*------获得画线的单位增加大小------*/
short GetUnitAdd(u8 x1,u8 x2)
{
	if(x1 == x2)	
		return 0;
	else if(x2 > x1) 
		return 1;
	else 
		return (-1);
}
/*------画一条线到缓冲区--------*/		
void PutLinetoBuf(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 delt_x,delt_y,flag,i;
	short add_x,add_y,error;
	delt_x = Abs(x2 - x1);		//计算Δx
	delt_y = Abs(y2 - y1);		//计算Δy
	if(delt_y > delt_x) 					//如果斜率大于1，置位flag,交换delt_x和delt_y
	{
		flag = 1;										
		Swap(&delt_x,&delt_y);
	}
	else
		flag = 0;
	add_x = GetUnitAdd(x1,x2);		//x轴单位增量(-1,0或1)
	add_y = GetUnitAdd(y1,y2);		//y轴单位增量(-1,0或1)
	error = delt_y << 1 - delt_x;			//算法改进后由error=delt_y/delt_x-0.5;变为error=2*delt_x*(delt_y/delt_x-0.5)=2*delt_y-delt_x;
	for(i = 0;i <= delt_x;i++)				//横向扫描
	{
		PutPointtoBuf(x1,y1);							//画点
		if(error >= 0)										//进行error>0时的增量计算
		{
			if(flag)					//斜率大于1的话，x轴进行增量计算
				x1 += add_x;
			else 
				y1 += add_y;		//斜率小于1的话，y轴进行增量计算
			error -= (delt_x << 1);
		}
								//进行基本增良扑憧，即每次循环都会进行的增量计算
		if(flag)							//斜率大于1的话，y轴进行增量计算
			y1 += add_y;
		else 									//斜率小于1的话，x轴进行增量计算
			x1 += add_x;
		error += (delt_y << 1);
	}
}
/*-----通过缓冲区画一条线到5110-------*/	
void PutLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 x,y,width,height;
	
	width = Abs(x2 - x1) + 1;		//获得刷新宽度
	height = Abs(y2 - y1) + 1;	//获得刷新高度
	x = Min(x1,x2);							//获得刷新起始横坐标
	y = Min(y1,y2);							//获得刷新起始纵坐标
	PutLinetoBuf(x1,y1,x2,y2);
	Refresh(x,y,width,height);
}
/*--画折线到缓冲区(p为折线各个节点，line_num为折线的线段数目)--*/
void PutPolylinetoBuf(point *p,u8 line_num)
{
	u8 i;
	for(i = 0;i < line_num;i++)
		PutLinetoBuf(p[i].x,p[i].y,p[i + 1].x,p[i + 1].y);
}
/*--通过缓冲区画折线到5110(p为折线各个节点，line_num为折线的线段数目)--*/
void PutPolyline(point *p,u8 line_num)
{
	PutPolylinetoBuf(p,line_num);
	RefreshAll();
}
/*---------画矩形到缓冲区--------*/
void PutRecttoBuf(u8 x,u8 y,u8 width,u8 height)
{
	PutLinetoBuf(x,y,x + width,y);
	PutLinetoBuf(x + width,y,x + width,y + height);
	PutLinetoBuf(x,y,x,y + height);
	PutLinetoBuf(x,y + height,x + width,y + height);
}
/*--------通过缓冲区画矩形到5110--------*/
void PutRect(u8 x,u8 y,u8 width,u8 height)
{
	PutRecttoBuf(x,y,width,height);
	Refresh(x,y,width + 1,height + 1);
}
/*--------画圆到缓冲区--------*/
void PutCircletoBuf(u8 center_x,u8 center_y,u8 r)
{
	u8 x = 0,y = r;
	short d = 5 - (r << 2);		//判别式初值4*(1.25-r)			
	
	while(x <= y)
	{					//将圆分成对称8份分别画
		PutPointtoBuf(center_x + x,center_y + y);
		PutPointtoBuf(center_x + x,center_y - y);
		PutPointtoBuf(center_x - x,center_y + y);
		PutPointtoBuf(center_x - x,center_y - y);
		PutPointtoBuf(center_x + y,center_y + x);
		PutPointtoBuf(center_x + y,center_y - x);
		PutPointtoBuf(center_x - y,center_y + x);
		PutPointtoBuf(center_x - y,center_y - x);
		x++;
		if(d < 0) 						//如果d<0判别式d=d+4*(2*x+3)
			d += (x << 3) + 12;
		else									//如果d>=0判别式d=d+4*(2*(x-y)+5)
		{
			y--;
			d += (x << 3) - (y << 3) + 20;
		}
	}
}
/*--------通过缓冲区画圆到5110--------*/
void PutCircle(u8 center_x,u8 center_y,u8 r)
{
	PutCircletoBuf(center_x,center_y,r);
	Refresh(center_x - r,center_y - r,(r << 1) + 1,(r << 1) + 1);
}
/*----画一个图片到缓冲区-----*/
void DrawBmptoBuf(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u16 p = 0;
	u8 width_i,height_i;
	height = ((height - 1) >> 3) + 1;

	for(height_i = 0;height_i < height;height_i ++)//画第height_i行数据
	{
		SetBufPtr(x,y + (height_i << 3));//设置缓冲区指针指到行首
		for(width_i = 0;width_i < width;width_i ++)//画第height_i行的第width_i列的1字节数据
		{
			WriteBytetoBuf(bitmap[p++]);
		}
	}
	
}
/*-----通过缓冲区画一个图片到5110------*/
void BufDrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	DrawBmptoBuf(bitmap,x,y,width,height);
	Refresh(x,y,width,height);
}

#endif /*USE_BUF*/




