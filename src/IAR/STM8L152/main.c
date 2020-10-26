/*******************************************************************************
*******************************����Ƕ��ʽϵͳ***********************************
*************************http://lenchimcu.taobao.com****************************
********************************************************************************
****���̼��:STM8L152K4������GPIO����
*******************************************************************************/
#include "stm8l15x.h"
#include "24c0x.h"
#include  "rx8025t.h"
#include  "irrx.h"
#include  "lcd5110.h"
#include "stts751.h"
#include "Display.h"

//����LED�밴���ӿ�
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

    0x00, 0x00, 0x19, 0x05, 0x26, 0x06, 0x20  ///��ʼ������ʱ�䣺16-10-21 12:00:00

};
#define BufferSize  30
unsigned char RBuf[BufferSize];
/*******************************************************************************
****��ڲ�������
****���ڲ�������
****������ע����ʱ����
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
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
****��ڲ�������
****���ڲ�������
****������ע��
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
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
    GPIO_Init(LED1_GPIO_PORT, LED1_GPIO_PINS, GPIO_Mode_Out_PP_Low_Slow);//��ʼ��LED1�ӿ�
    InitLcd ();
    CLK_PeripheralClockConfig (CLK_Peripheral_I2C1,ENABLE);//����IIC1ʱ��
//    CLK_PeripheralClockConfig (CLK_Peripheral_TIM2,ENABLE); //ʹ������ʱ�ӣ�STM8LĬ����������ʱ�ӳ�ʼʱ�رգ�ʹ��ǰ�쿪��
 //   TIM2_TimeBaseInit(TIM2_Prescaler_1,TIM2_CounterMode_Up, 0xffff);//16M/8/128=15.625K��0xff=255,255*��1/15.625��=0.01632S����Լ61���ж���1S
 //   GPIO_Init(IR_GPIO_PORT, IR_GPIO_PINS, GPIO_Mode_In_FL_IT);//��ʼ��������GPB6�����ж�����
//    TIM2_ARRPreloadConfig(DISABLE);	/* Ԥװ�ز�ʹ�� */
//    EXTI_DeInit (); //�ָ��жϵ���������
//    TIM2_Cmd(ENABLE);         
//    EXTI_SetPinSensitivity (EXTI_Pin_1,EXTI_Trigger_Falling);//�ⲿ�ж�6���½��ش������ж�������14
//    enableInterrupts();//ʹ���ж�
        

    //��MACд����
     //UART_Init();  
  //Send_Str("STM8S IIC��д����:\r\n");
  
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
        GPIO_ToggleBits(LED1_GPIO_PORT, LED1_GPIO_PINS);//��תLED1�ӿڵ�ƽ��ʵ��LED1��˸
        Delay(0x00F);//��ʱ�����ڹ۲�ʵ������
        //    Rtc8025_ReadData(R8025_DataBuf,0x00,7);
 //       displaytime();
        ReadSttstemp();
        Display_tempdot(temp10,10);
        
    }   

}

//texted with textcode editor

/*******************************************************************************
*******************************����Ƕ��ʽϵͳ***********************************
*************************http://lenchimcu.taobao.com****************************
*******************************************************************************/
