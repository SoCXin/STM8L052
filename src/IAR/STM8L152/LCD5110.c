#include "LCD5110.h"

#ifdef USE_BUF

u8 lcd_buf[LCD_COLUMN][LCD_ROW];

point buf_ptr;
point start_ptr;

#endif /*USE_BUF*/



/*------��ʼ��5110�˿ں�����-------
void LcdPortInit(void)
{
	
  
  
  
  _InitTypeDef        GPIO_InitStructure;
	
  RCC_AHBPeriphClockCmd(RCC_LCD5110_PORT, ENABLE);	//ʹ�ܶ˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = LCD_SCLK_BIT | LCD_SDIN_BIT | LCD_DC_BIT | LCD_SCE_BIT | LCD_REST_BIT;//ѡ��Ҫ���õ�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��������Ϊ���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//��������Ϊ����
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;	//������������ٶ�
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//�����������������
  GPIO_Init(LCD5110_PORT, &GPIO_InitStructure);
  
  
}
-*/



/*-----------------------------------------------------------------------
LCD_init          : 3310LCD��ʼ��

��д����          ��2004-8-10 
����޸�����      ��2004-8-10 
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
/////*				����Ϊ��ʹ�û�����д�ֻ�ͼ����غ���				*//////
//*--------------------------------------------------------*//
/////////////////////////////////////////////////////////////////////////////////////////////////////


//*******				�����ǻ��������ͳ�ʼ������				*****//

/*-------дһ���ֽڵ�5110(d_c���������ݻ�������)----------*/
void LcdWrite(u8 byte,u8 d_c)
{
	u8 i;
	
	LCD_SCE_L;									//ʹ��Ƭѡ
	//Delayus(1);
	if(d_c == DATA)							//ѡ���͵������ݻ�������
		LCD_DC_H;
	else 
		LCD_DC_L;
	//Delayus(1);
	for(i = 0;i < 8;i ++)				//����һ���ֽ�
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
	LCD_SCE_H;										//ƬѡʧЧ
	//Delayus(1);
}
/*----------��ʼ��5110-----------*/
void InitLcd(void)
{
	 Delay(0x0F);			//��ʱ1ms
	LcdPortInit();			//��ʼ��5110�˿ں�����
	LCD_REST_L;					//���͸�λ�źų�ʼ��5110�ڲ��Ĵ���
	Delay(1);
	LCD_REST_H;
	 Delay(0x0F);
	//LCD_SCE_L;				//ʹ��Ƭѡ
	//Delayus(1);
	//LCD_SCE_H;				//ƬѡʧЧ
	//Delayus(1);
	//��չָ��(function set PD = 0 and V = 0, select extended instruction set(H = 1 mode))
	LcdWrite(0x21,CMD);
	//����Һ����ѹ(set VOP: VOP is set to a + 65 �� b [V])
	LcdWrite(0xc8,CMD);				//VCC��5V
	//LcdWrite(0xbf,CMD);			  //VCC��3V
	//�¶�У��
	LcdWrite(0x06,CMD);
	//���û����(RECOMMENDED MUX RATE(1:10),you can chose other MUX RATE)
	LcdWrite(0x13,CMD);				//VCC��5V
	//LcdWrite(0x17,CMD);			  //VCC��3V
	//��׼ָ��(function set PD = 0 and V = 0, select normal instruction set(H = 0 mode))
	LcdWrite(0x20,CMD);
	//��׼��ʾģʽ(display control set normal mode (D=1andE=0))
	LcdWrite(0x0c,CMD);
	//����
	LcdClearAll();				
}
/*---����5110�к�������----*/
void SetXY(u8 row,u8 column)
{
	if(row >= LCD_ROW) row = 0;						//��������������
	if(column >= LCD_COLUMN) column = 0;	//��������������
	
	LcdWrite(0x40 | column,CMD);					//����������
	LcdWrite(0x80 | row,CMD);							//����������
}

/*-------�������5110��Ļ--------*/
void LcdClearAll(void)
{
	u16 i;
	
	SetXY(0,0);									//����ص�ԭ��
	for(i = 0;i < LCD_AREA_BYTE;i ++)	
	{
		LcdWrite(0,DATA);					//д0��5110��������ֽ�����
	}
}

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
void WriteChar(u8 value)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(ASCII_6_8[value - 0x20][i],DATA);
}
/*---ֱ��дһ��ASCII�ַ�(6*8)��5110---*/
void PutChar(u8 value,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteChar(value);
}
/*---ֱ��дһ���ַ���(6*8)��5110(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteStr(char * str)
{
	while(*str)
	{
		WriteChar(*str ++);
	}
}
/*--ֱ��дһ���ַ���(6*8)��5110--*/
void PutStr(char * str,u8 column,u8 row)
{
	if(row > 13) row = 0;
	SetXY(6 * row,column);
	WriteStr(str);
}
/*---��sign(6*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteSign(char * sign)
{
	u8 i;
	for(i = 0;i < 6;i ++)
	LcdWrite(sign[i],DATA);
}
/*----��sign(6*8)----*/
void PutSign(char * sign,u8 column,u8 row)
{
	SetXY(row,column);
	WriteSign(sign);
}
/*---��Icon(10*8)(����Ҫ���꣬�����ϸ��ַ���ͼ��)---*/
void WriteIcon(char * icon)
{
	u8 i;
	for(i = 0;i < 10;i ++)
	LcdWrite(icon[i],DATA);
}
/*--��Icon(10*8),row(0~74)--*/
void PutIcon(char * icon,u8 column,u8 row)
{
	if(row > (LCD_ROW - 10))//�����Ҫ����
	{
		row = 0;
		column += 1;
	}
	SetXY(row,column);
	WriteIcon(icon);
}

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
void PutWideChar(u8 value,u8 column,u8 row)
{
	u8 i;
	//д�ϰ���
	SetXY(row,column << 1);
	for(i = 0;i < 8;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
	//д�°���
	SetXY(row,(column << 1) + 1);
	for(i = 8;i < 16;i ++)
		LcdWrite(ASCII_8_16[value - 0x20][i],DATA);
}
/*--ֱ��дһ���ַ���(6*8)��5110���Զ����У�row(0~76)--*/
void PutWideStr(char * str,u8 column,u8 row)
{
	u8 i = 0;
	
	while(*str)
	{	
		if(row + (i << 3) > (LCD_ROW - 8))//�����Ҫ����
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

/*-----ֱ��дһ������(16*16)��5110------*/
void PutHanzi(char Hanzi[],u8 column,u8 row)
{
	u8 i;
	//д�ϰ���
	SetXY(row,column << 1);
	for(i = 0;i < 16;i ++)
	LcdWrite(Hanzi[i],DATA);
	//д�°���
	SetXY(row,(column << 1) + 1);
	for(i = 16;i < 32;i ++)
		LcdWrite(Hanzi[i],DATA);
}
/*--дһ������(16*16),�Զ�����,row(0~58)--*/
void PutHanziStr(char Hanzi[],u8 column,u8 row,u8 num)
{
	u8 num_i,i = 0;
	for(num_i = 0;num_i < num;num_i ++)
	{
		if(row + (num_i << 4) > (LCD_ROW - 16))	//�����Ҫ����
		{
			row = 0;
			i = 0;
			column += 1;
		}
		PutHanzi(Hanzi + (num_i << 5),column,row + (i << 4));
		i ++;
	}
}

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
void DrawPicture(char bitmap[])
{
	u16 i;
	for(i = 0;i < LCD_AREA_BYTE;i ++)
		LcdWrite(bitmap[i],DATA);
}

/*---��picture(�߶ȱ�����8�ı���)---*/
void DrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	height = (height >> 3);
	//����height_i��
	for(height_i = 0;height_i < height;height_i ++)
	{
		SetXY(x,y + height_i);
		//�����е�width��
		for(width_i = 0;width_i < width;width_i ++)
		{
			LcdWrite(bitmap[width_i + height_i * width],DATA);
		}
	}
}
#ifdef USE_BUF
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

/*----���һ��8λ�������ľ���ֵ----*/
u8 Abs(char a)
{
	if((u8)a >= 128)
		return 256 - a;
	else 
		return a;
}
/*----��������8λ������----*/
void Swap(u8 *a,u8 *b)
{
	u8 temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
}
/*-----���û�����ָ������-------*/
void SetBufPtr(u8 x,u8 y)
{
	if(y >= LCD_Y) y= 0;				//������������������
	if(x >= LCD_X) x = 0;				//�����������������
	
	buf_ptr.y = y;							//����������
	buf_ptr.x = x;							//���ú�����
}
/*----������������ˢ�µ�5110��-----*/
void RefreshAll(void)
{
	u16 i,j;
	SetXY(0,0);
	for(i = 0;i < LCD_COLUMN;i ++)
		for(j = 0;j < LCD_X;j++)
		LcdWrite(lcd_buf[i][j],DATA);
}
/*-------��������ָ������ˢ�µ�5110��Ӧ����-------*/
void Refresh(u8 x,u8 y,u8 width,u8 height)
{
	u8 width_i,height_i;
	u8 start_row,start_column,end_row,end_column;
	start_row = y >> 3;					//ˢ����ʼ��
	start_column = x;						//ˢ����ʼ��
	end_row = (y + height - 1) >> 3 ;//ˢ����ֹ��
	end_column = x + width - 1;				//ˢ����ֹ��
	
	//ˢ�µ�height_i��
	for(height_i = start_row;height_i <= end_row;height_i ++)		//ˢ��ָ����
	{
		SetXY(start_column,height_i);				//�����׿�ʼˢ��
		//ˢ�¸��е�width��
		for(width_i = start_column;width_i <= end_column;width_i ++)	//ˢ��ÿ�е�ָ����
		{
			LcdWrite(lcd_buf[height_i][width_i],DATA);
		}
	}
}
/*----���������1�ֽ�����----*/
void ClearPointofBuf(u8 x,u8 y)
{
	u8 column,column_bit;
	
	SetBufPtr(x,y);
	column = buf_ptr.y >> 3;							//Ҫ����Ļ��������ֽ�������
	column_bit = buf_ptr.y % 8;					//�����ڸ��ֽ�8bit��bitλ��
	lcd_buf[column][buf_ptr.x] &= ~(0x01 << (column_bit));	//���buf_ptrָ��Ļ������ֽ�
	if(column_bit) lcd_buf[column + 1][buf_ptr.x] &= ~(0xff >> (8 - column_bit));//�ֽڿ����о������һ��
}
/*------дһ���ֽ����ݵ�������-------*/
void WriteBytetoBuf(u8 byte)
{
	u8 column,column_bit;
	
	column = buf_ptr.y >> 3;							//Ҫд�Ļ��������ֽ�������
	column_bit = buf_ptr.y % 8;					//�����ڸ��ֽ�8bit��bitλ��
	lcd_buf[column][buf_ptr.x] |= byte << (column_bit);	//��1�ֽ�����д��buf_ptrָ��Ļ������ֽ�
	if(column_bit) lcd_buf[column + 1][buf_ptr.x] |= byte >> (8 - column_bit);//�ֽڿ����о�д��һ��
	if(buf_ptr.x >= LCD_X - 1)				//������β������ָ���Զ���ת����һ������
	{
		buf_ptr.x = 0;
		buf_ptr.y += 8;
		if(buf_ptr.y >= LCD_Y)				  //����ҳ��ĩβ��������ָ������ҳ�濪ͷ
			buf_ptr.y = 0;
	}
	else 
		buf_ptr.x++;										//������ָ��ָ����һ��
}

/*------�������������-------*/
void ClearAllBuf(void)
{
	u8 i,j;
	u8 y = 0;
	
	for(j = 0;j < LCD_COLUMN;j++)
	{
		for(i = 0;i < LCD_ROW;i++)
		{
			lcd_buf[j][i] = 0x00; 			//д0����������������ֽ�����
		}
		y += 8;
	}
}
/*------���������������ˢ��5110------*/
void BufClearAll(void)
{
	ClearAllBuf();							//���������
	RefreshAll();								//ˢ��
}
/*-------��ջ�����ָ������-------*/
void ClearBuf(u8 x,u8 y,u8 width,u8 height)
{
	u8 i,j;
	
	height = y + height;								//���ˢ�µ���ֹ������
	width = width + x;									//���ˢ�µ���ֹ������
	for(j = y;j < height;j++)
	{
		for(i = x;i < width;i++)
		{
			ClearPointofBuf(i,j);						//д0����������������ֽ�����
		}
	}
}
/*-------��ջ�����ָ������ˢ��------*/
void BufClear(u8 x,u8 y,u8 width,u8 height)
{
		ClearBuf(x,y,width,height);				//��ջ�����ָ������
		Refresh(x,y,width,height);				//ˢ��
}


///////*------------------------------------------*////////
//*********				����Ϊд�ַ������ֺ���				*********//
///////*------------------------------------------*////////


//******        �����ַ��߶Ⱦ�Ϊ8              ******//

/*-------дһ��ASCII�ַ�(6*8)��������(����Ҫ���꣬�����ϸ��ַ�)--------*/
void WriteChartoBuf(u8 value)
{
	u8 i;
	
	start_ptr.x = buf_ptr.x;					//��ÿ�ʼ������
	start_ptr.y = buf_ptr.y;					//��ÿ�ʼ������
	for(i = 0;i < 6;i ++)							//����д�����ֽ�
		WriteBytetoBuf(ASCII_6_8[value - 0x20][i]);
}
/*--------ͨ��������дһ��ASCII�ַ�(6*8)��5110(����Ҫ���꣬�����ϸ��ַ�)--------*/
void BufWriteChar(u8 value)
{
	WriteChartoBuf(value);
	Refresh(start_ptr.x,start_ptr.y,6,8);			//ˢ��
}
/*----дһ��ASCII�ַ�(6*8)��������-----*/
void PutChartoBuf(u8 value,u8 x,u8 y)
{
	SetBufPtr(x,y);											//���ô�ӡ����
	WriteChartoBuf(value);
}
/*----ͨ��������дһ��ASCII�ַ�(6*8)��5110-----*/
void BufPutChar(u8 value,u8 x,u8 y)
{
	PutChartoBuf(value,x,y);
	Refresh(start_ptr.x,start_ptr.y,6,8);
}	
/*------дһ���ַ���(�߶�8)��������(����Ҫ���꣬�����ϸ��ַ�)-----*/
u8 WriteStrtoBuf(char * str)
{
	u8 str_len = 0;
	
	while(*str)
	{
		WriteChartoBuf(*str ++);
		str_len++;
	}
	return str_len;										//�����ַ�������
}
/*----ͨ��������дһ���ַ���(�߶�8)��5110(����Ҫ���꣬�����ϸ��ַ�)----*/
u8 BufWriteStr(char * str)
{
	u8 y,str_len,height;
	
	str_len = WriteStrtoBuf(str);
	height = (char)((str_len * 6 - start_ptr.x - 6) / LCD_X) + 2; //���ˢ�¸߶�
	y = start_ptr.y - 8 * (height - 1);		//����ַ�����ʼ������
	Refresh(0,y,LCD_X,8 * height);
	return str_len;												//�����ַ�������
}
/*-----дһ���ַ���(�߶�8)��������------*/
u8 PutStrtoBuf(char * str,u8 x,u8 y)
{
	SetBufPtr(x,y);
	return WriteStrtoBuf(str);						//�����ַ�������
}
/*------ͨ��������дһ���ַ���(�߶�8)��5110------*/
u8 BufPutStr(char * str,u8 x,u8 y)
{
	u8 str_len,height;
	
	str_len = PutStrtoBuf(str,x,y);
	height = (char)((str_len * 6 - (LCD_X - x) - 1) / LCD_X) + 2;//���ˢ�¸߶�
	Refresh(0,y,LCD_X,8 * height);
	return str_len;												//�����ַ�������
}

//*******        �����ַ����ָ߶Ⱦ�Ϊ16          ********//

/*------дһ��ASCII�ַ�(8*16)��������------*/
void PutWideChartoBuf(u8 value,u8 x,u8 y)
{
	u8 i;
	//д����һ��
	SetBufPtr(x,y);
	for(i = 0;i < 8;i ++)
		WriteBytetoBuf(ASCII_8_16[value - 0x20][i]);
	//д����һ��
	SetBufPtr(x,y + 8);
	for(i = 8;i < 16;i ++)
		WriteBytetoBuf(ASCII_8_16[value - 0x20][i]);
}
/*----ͨ��������дһ��ASCII�ַ�(8*16)��5110------*/
void BufPutWideChar(u8 value,u8 x,u8 y)
{
	PutWideChartoBuf(value,x,y);
	Refresh(x,y,8,16);
}
/*-----дһ���ַ���(�߶�16)��������------*/
u8 PutWideStrtoBuf(char * str,u8 x,u8 y)
{
	u8 i = 0;
	u8 column = 1;
	
	while(*str)
	{
		//�����Ҫ���У�����������Զ�����
		if(x + (i << 3) > (LCD_X - 8))
		{
			x = 0;
			i = 0;
			y += 16;
			column++;			//ˢ��������1
		}
		//дһ��char
		PutWideChartoBuf(*str,x + (i << 3),y);
		str++;
		i++;
	}
	return column;	//�����ַ�������
}
/*------ͨ��������д�ַ���(�߶�16)��5110------*/
u8 BufPutWideStr(char *str,u8 x,u8 y)
{
	u8 column;
	
	column = PutWideStrtoBuf(str,x,y);			//���ˢ�µ�����
	Refresh(x,y,LCD_X,16 << (column - 1));
	return column;
}
/*-----дһ������(16*16)��������------*/
void PutHanzitoBuf(char Hanzi[],u8 x,u8 y)
{
	u8 i;
	//д����һ��
	SetBufPtr(x,y);
	for(i = 0;i < 16;i ++)
		WriteBytetoBuf(Hanzi[i]);
	//д����һ��
	SetBufPtr(x,y + 8);
	for(i = 16;i < 32;i ++)
		WriteBytetoBuf(Hanzi[i]);
}
/*-----ͨ��������дһ������(16*16)��5110-------*/
void BufPutHanzi(char Hanzi[],u8 x,u8 y)
{
	PutHanzitoBuf(Hanzi,x,y);
	Refresh(x,y,16,16);
}
/*-------дһ������(�߶�16)��������(numΪ���ָ���)-------*/
u8 PutHanziStrtoBuf(char Hanzi[],u8 x,u8 y,u8 num)
{
	u8 num_i,i = 0;
	u8 column = 1;
	for(num_i = 0;num_i < num;num_i ++)
	{
		//�����Ҫ���У��������껻��
		if(x + (num_i << 4) > (LCD_X - 16))
		{
			x = 0;
			i = 0;
			y += 16;
			column++;
		}
		//дһ������
		PutHanzitoBuf(Hanzi + (num_i << 5),x + (i << 4),y);
		i ++;
	}
	return column;					//����ˢ������
}
/*-----ͨ��������дһ������(�߶�16)��5110(numΪ���ָ���)------*/
u8 BufPutHanziStr(char Hanzi[],u8 x,u8 y,u8 num)
{
	u8 column;
	column = PutHanziStrtoBuf(Hanzi,x,y,num);
	Refresh(x,y,LCD_X,16 << (column - 1));
	return column;
}	
	
//*********    ����Ϊ���㣬�ߣ����ߣ����Σ�Բ��bmpͼƬ����      *********//

/*------��һ���㵽������-------*/
void PutPointtoBuf(u8 x,u8 y)
{
	u8 row;
	
	row = y >> 3;													//��õ����ڵ���
	lcd_buf[row][x] |= 0x01 << (y % 8);
}
/*------ͨ����������һ���㵽5110------*/
void PutPoint(u8 x,u8 y)
{
	PutPointtoBuf(x,y);
	Refresh(x,y,1,8);
} 
/*-----��������н�С����------*/
u8 Min(u8 a,u8 b)
{
	return a <= b? a : b;
}
/*------��û��ߵĵ�λ���Ӵ�С------*/
short GetUnitAdd(u8 x1,u8 x2)
{
	if(x1 == x2)	
		return 0;
	else if(x2 > x1) 
		return 1;
	else 
		return (-1);
}
/*------��һ���ߵ�������--------*/		
void PutLinetoBuf(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 delt_x,delt_y,flag,i;
	short add_x,add_y,error;
	delt_x = Abs(x2 - x1);		//���㦤x
	delt_y = Abs(y2 - y1);		//���㦤y
	if(delt_y > delt_x) 					//���б�ʴ���1����λflag,����delt_x��delt_y
	{
		flag = 1;										
		Swap(&delt_x,&delt_y);
	}
	else
		flag = 0;
	add_x = GetUnitAdd(x1,x2);		//x�ᵥλ����(-1,0��1)
	add_y = GetUnitAdd(y1,y2);		//y�ᵥλ����(-1,0��1)
	error = delt_y << 1 - delt_x;			//�㷨�Ľ�����error=delt_y/delt_x-0.5;��Ϊerror=2*delt_x*(delt_y/delt_x-0.5)=2*delt_y-delt_x;
	for(i = 0;i <= delt_x;i++)				//����ɨ��
	{
		PutPointtoBuf(x1,y1);							//����
		if(error >= 0)										//����error>0ʱ����������
		{
			if(flag)					//б�ʴ���1�Ļ���x�������������
				x1 += add_x;
			else 
				y1 += add_y;		//б��С��1�Ļ���y�������������
			error -= (delt_x << 1);
		}
								//���л���������㿣���ÿ��ѭ��������е���������
		if(flag)							//б�ʴ���1�Ļ���y�������������
			y1 += add_y;
		else 									//б��С��1�Ļ���x�������������
			x1 += add_x;
		error += (delt_y << 1);
	}
}
/*-----ͨ����������һ���ߵ�5110-------*/	
void PutLine(u8 x1,u8 y1,u8 x2,u8 y2)
{
	u8 x,y,width,height;
	
	width = Abs(x2 - x1) + 1;		//���ˢ�¿��
	height = Abs(y2 - y1) + 1;	//���ˢ�¸߶�
	x = Min(x1,x2);							//���ˢ����ʼ������
	y = Min(y1,y2);							//���ˢ����ʼ������
	PutLinetoBuf(x1,y1,x2,y2);
	Refresh(x,y,width,height);
}
/*--�����ߵ�������(pΪ���߸����ڵ㣬line_numΪ���ߵ��߶���Ŀ)--*/
void PutPolylinetoBuf(point *p,u8 line_num)
{
	u8 i;
	for(i = 0;i < line_num;i++)
		PutLinetoBuf(p[i].x,p[i].y,p[i + 1].x,p[i + 1].y);
}
/*--ͨ�������������ߵ�5110(pΪ���߸����ڵ㣬line_numΪ���ߵ��߶���Ŀ)--*/
void PutPolyline(point *p,u8 line_num)
{
	PutPolylinetoBuf(p,line_num);
	RefreshAll();
}
/*---------�����ε�������--------*/
void PutRecttoBuf(u8 x,u8 y,u8 width,u8 height)
{
	PutLinetoBuf(x,y,x + width,y);
	PutLinetoBuf(x + width,y,x + width,y + height);
	PutLinetoBuf(x,y,x,y + height);
	PutLinetoBuf(x,y + height,x + width,y + height);
}
/*--------ͨ�������������ε�5110--------*/
void PutRect(u8 x,u8 y,u8 width,u8 height)
{
	PutRecttoBuf(x,y,width,height);
	Refresh(x,y,width + 1,height + 1);
}
/*--------��Բ��������--------*/
void PutCircletoBuf(u8 center_x,u8 center_y,u8 r)
{
	u8 x = 0,y = r;
	short d = 5 - (r << 2);		//�б�ʽ��ֵ4*(1.25-r)			
	
	while(x <= y)
	{					//��Բ�ֳɶԳ�8�ݷֱ�
		PutPointtoBuf(center_x + x,center_y + y);
		PutPointtoBuf(center_x + x,center_y - y);
		PutPointtoBuf(center_x - x,center_y + y);
		PutPointtoBuf(center_x - x,center_y - y);
		PutPointtoBuf(center_x + y,center_y + x);
		PutPointtoBuf(center_x + y,center_y - x);
		PutPointtoBuf(center_x - y,center_y + x);
		PutPointtoBuf(center_x - y,center_y - x);
		x++;
		if(d < 0) 						//���d<0�б�ʽd=d+4*(2*x+3)
			d += (x << 3) + 12;
		else									//���d>=0�б�ʽd=d+4*(2*(x-y)+5)
		{
			y--;
			d += (x << 3) - (y << 3) + 20;
		}
	}
}
/*--------ͨ����������Բ��5110--------*/
void PutCircle(u8 center_x,u8 center_y,u8 r)
{
	PutCircletoBuf(center_x,center_y,r);
	Refresh(center_x - r,center_y - r,(r << 1) + 1,(r << 1) + 1);
}
/*----��һ��ͼƬ��������-----*/
void DrawBmptoBuf(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	u16 p = 0;
	u8 width_i,height_i;
	height = ((height - 1) >> 3) + 1;

	for(height_i = 0;height_i < height;height_i ++)//����height_i������
	{
		SetBufPtr(x,y + (height_i << 3));//���û�����ָ��ָ������
		for(width_i = 0;width_i < width;width_i ++)//����height_i�еĵ�width_i�е�1�ֽ�����
		{
			WriteBytetoBuf(bitmap[p++]);
		}
	}
	
}
/*-----ͨ����������һ��ͼƬ��5110------*/
void BufDrawBmp(char bitmap[],u8 x,u8 y,u8 width,u8 height)
{
	DrawBmptoBuf(bitmap,x,y,width,height);
	Refresh(x,y,width,height);
}

#endif /*USE_BUF*/




