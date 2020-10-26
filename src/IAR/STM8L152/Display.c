#include "Display.h"


void Display(void){

}
void Display_coordinate (u8 longy,u8 offset){// the position of x and y
	u8 x1,x2,y1,y2;
	y1= LCD_Y- offset;
	y2=	y1-longy;
	x1=0;
	x2=0;

	PutLinetoBuf(x1,y1,x2,y2);
	x1=0;
	x2=LCD_X;
	y1=LCD_Y-offset;
	y2=y1;

	PutLinetoBuf(x1,y1,x2,y2);

}

void Display_tempdot(u16 temp,u8 zerolevel){
	static u8 numberoftemp;
	u8 x,y,i;
        if (numberoftemp>LCD_X)	{
          for (i=1;i<LCD_X-1;i++){
 
              lcd_buf[2][i]=lcd_buf[2][i+1];
              lcd_buf[3][i]=lcd_buf[3][i+1];
              lcd_buf[4][i]=lcd_buf[4][i+1];                       
              lcd_buf[5][i]=lcd_buf[5][i+1];   
          }
              x=LCD_X-1;
              Refresh(0,16,LCD_X,32);
        }
                      
        else x=numberoftemp++;
	y= LCD_Y-temp/10+zerolevel;
        for (i=2;i<5;i++){         //clear buf
            lcd_buf[i][LCD_X-1]=0;
        
        }
        PutPoint(x,y);
//        PutPoint(x,10);

        

}