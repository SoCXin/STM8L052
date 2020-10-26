/*******************************************************************************
*******************************蓝旗嵌入式系统***********************************
*************************http://lenchimcu.taobao.com****************************
********************************************************************************
****例程简介:STM8L152K4库例程GPIO例程
*******************************************************************************/
#include "stm8l15x.h"
#include "24c0x.h"
#include  "rx8025t.h"
#include  "irrx.h"
#include  "lcd5110.h"
#include "stts751.h"
#include "Display.h"

//定义LED与按键接口
#define LED1_GPIO_PORT  GPIOE
#define LED1_GPIO_PINS  GPIO_Pin_3
#define IR_GPIO_PORT  GPIOB
#define IR_GPIO_PINS  GPIO_Pin_1


/*
#define LED2_GPIO_PORT  GPIOB
#define LED2_GPIO_PINS  GPIO_Pin_0
#define LED3_GPIO_PORT  GPIOC
#define LED3_GPIO_PINS  GPIO_Pin_4
*/
unsigned char WBuf[]=
 {

    0x00, 0x00, 0x19, 0x05, 0x26, 0x06, 0x20  ///初始化日期时间：16-10-21 12:00:00

};
#define BufferSize  30
unsigned char RBuf[BufferSize];
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：
****版权信息：蓝旗嵌入式系统
*******************************************************************************/

void displaytime(void){
   char R8025_tmp[15], R8025_date[9],R8025_time[7];/////////////////////////////////////////////////////
  char i ;
  for(i=0;i<7;i++){
    R8025_tmp[i*2]=R8025_DataBuf[i]&0x0F;
     R8025_tmp[i*2]+=   '0';
    R8025_tmp[i*2+1]=(R8025_DataBuf[i]&0XF0)>>4;    
           R8025_tmp[i*2+1]+=   '0';
           

    
  }
  for(i=0;i<9;i++) {
    R8025_date[i]=R8025_tmp[14-1];
  }
  R8025_date[8]='\0';
  
  R8025_time[0]=R8025_tmp[5];
  R8025_time[1]=R8025_tmp[4]  ;  
    R8025_time[2]   =':';
   R8025_time[3]=R8025_tmp[3]  ;
    R8025_time[4]=R8025_tmp[2] ;     
    R8025_time[5] ='\0';    
      PutStr(R8025_date,3,0); 
       SetXY(0,0);
  PutWideStr(R8025_time,0,30);
    PutWideStr(R8025_time,1,30);    
}
  
  

int main( void )
{
    u8 timechar[14];
    GPIO_Init(LED1_GPIO_PORT, LED1_GPIO_PINS, GPIO_Mode_Out_PP_Low_Slow);//初始化LED1接口
    InitLcd ();
    CLK_PeripheralClockConfig (CLK_Peripheral_I2C1,ENABLE);//开启IIC1时钟
//    CLK_PeripheralClockConfig (CLK_Peripheral_TIM2,ENABLE); //使能外设时钟，STM8L默认所有外设时钟初始时关闭，使用前徐开启
 //   TIM2_TimeBaseInit(TIM2_Prescaler_1,TIM2_CounterMode_Up, 0xffff);//16M/8/128=15.625K，0xff=255,255*（1/15.625）=0.01632S，大约61次中断是1S
 //   GPIO_Init(IR_GPIO_PORT, IR_GPIO_PINS, GPIO_Mode_In_FL_IT);//初始化按键，GPB6上拉中断输入
//    TIM2_ARRPreloadConfig(DISABLE);	/* 预装载不使能 */
//    EXTI_DeInit (); //恢复中断的所有设置
//    TIM2_Cmd(ENABLE);         
//    EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);//外部中断6，下降沿触发，中断向量号14
//    enableInterrupts();//使能中断
        

    //用MAC写代码
     //UART_Init();  
  //Send_Str("STM8S IIC读写测试:\r\n");
  
  InitIIC();
 Rtc8025_Init();
 initStts751();
  Display_coordinate(30,5);
  RefreshAll();
  
  
// Rtc8025_WriteData(WBuf,0x00,7);
 
  //EEPROM_WriteData(WBuf,EEPROM_BASE_ADDRESS,BufferSize);
 //EEPROM_ReadData(RBuf,EEPROM_BASE_ADDRESS,BufferSize);
 
  //Send_Str(RBuf);
    while(1)
    {
        GPIO_ToggleBits(LED1_GPIO_PORT, LED1_GPIO_PINS);//翻转LED1接口电平，实现LED1闪烁
        Delay(0x00F);//延时，便于观察实验现象
        //    Rtc8025_ReadData(R8025_DataBuf,0x00,7);
 //       displaytime();
        ReadSttstemp();
        Display_tempdot(temp10,10);
        
    }   

}

//texted with textcode editor

/*******************************************************************************
*******************************蓝旗嵌入式系统***********************************
*************************http://lenchimcu.taobao.com****************************
*******************************************************************************/
