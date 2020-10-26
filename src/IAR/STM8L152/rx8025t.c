

#include "rx8025t.h"

#define  INIT_R8025_DATA    ////��ʼ������ʱ��
#define     RX8025_T
//#define RX8025_AC

#define  EEPROM_ADDRESS 0x64

u8  R8025_DataBuf[7];///����R8025��ȡ����

#ifdef INIT_R8025_DATA
static u8  R8025_Init_DataBuf[7] = {

    0x00, 0x48, 0x15, 0x00, 0x01, 0x03, 0x18  ///��ʼ������ʱ�䣺16-10-21 12:00:00

};
#endif
static void  delay_us(u16 num){
    while (num--);

}
void Rtc8025_Init(void)
{
   while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* ����ʼλ */
    I2C_GenerateSTART(I2C1,ENABLE);
    
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
    /* ����EV6 ��������������һ��Ӧ���ź�*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData(I2C1,(u8)(0X0E)); 
    /* ����EV8 ��������������һ��Ӧ���ź�*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* ���������豸д����*/

      
      I2C_SendData(I2C1,0x20); 
  

  
    /* ������λ*/
    I2C_GenerateSTOP(I2C1,ENABLE);
    
   
}
  

/*
 *��д������ο������΢��
 *http://blog.csdn.net/u012351051/article/details/50896822
 */
   
 void Rtc8025_ReadData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
   /* �ȴ����� */
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
  
    /* ����ʼλ */
    I2C_GenerateSTART(I2C1,ENABLE);
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* ����EV6 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
   
    I2C_SendData(I2C1,(u8)(addr)); 
    /* ����EV8 ��������������һ��Ӧ���ź�*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
    /* ����ʼλ */ 
    I2C_GenerateSTART(I2C1,ENABLE);
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* ����I2C��������ַ��I2C���豸Ϊ��ģʽ*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Receiver);
    /* ����EV6 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /*�����ڴӴ��豸��ȡ���� */
    
 //    I2C_AcknowledgeConfig(I2C1,ENABLE);//ADD TO HERE NO
  while(num)  
  {
  //          Delay(0xFFF); 
      /* ����EV6 */
    if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){      
      /*�� EEPROM ��ȡһ���ֽ�*/
            delay_us(10); 
      *buffer = I2C_ReceiveData(I2C1);
      /* ָ��ָ���¸�����ֽڵĵ�ַ*/
      buffer++;  
      
      /* �������һ���ֽ�*/
      if(num == 1){
      /* ����ҪӦ��*/
        I2C_AcknowledgeConfig(I2C1,DISABLE);
      /* ������λ*/
        I2C_GenerateSTOP(I2C1,ENABLE); 
          delay_us(10); 
      }
      else      
        /* �������һ���ֽ�����豸����Ӧ���ź�*/
       I2C_AcknowledgeConfig(I2C1,ENABLE);
      num--;
    }   
                delay_us(10); 
  }
  
        I2C_AcknowledgeConfig(I2C1,ENABLE);//TMD THIS DON'T FORGET!
}
void Rtc8025_WriteData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
  /* �ȴ����� */
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* ����ʼλ */
    I2C_GenerateSTART(I2C1,ENABLE);
    
    /* ����EV5 ��������������һ��Ӧ���ź�*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* ����I2C��������ַ��I2C���豸Ϊдģʽ*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
    /* ����EV6 ��������������һ��Ӧ���ź�*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData(I2C1,(u8)(addr)); 
    /* ����EV8 ��������������һ��Ӧ���ź�*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* ���������豸д����*/
    while(num--)  
    {
      
      I2C_SendData(I2C1,*buffer); 
  
      /*ָ��ָ����һ����ַ*/
      buffer++; 
    Delay(0x10);
      /* ����EV6 ��������������һ��Ӧ���ź�*/
     while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_AcknowledgeConfig(I2C1,ENABLE);
    }
  
    /* ������λ*/
    I2C_GenerateSTOP(I2C1,ENABLE);
}
void R8025AC_Read(u8 sadd, u8 *buf, u8 len)  
{  
    u8 i;  
    IIC_RX8025_Start();  
    IIC_RX8025_Send_Byte(0x64); 
#ifdef RX8025_AC    
    IIC_RX8025_Send_Byte(sadd<<4);////////////|0x01);
#elif defined RX8025_T    
    IIC_RX8025_Send_Byte(sadd);
#endif    
    IIC_RX8025_Start();  
    IIC_RX8025_Send_Byte(0x65);  
    for(i=0;i<len-1;i++)  
    {  
        buf[i] = IIC_RX8025_Read_Byte(1);  
        IIC_RX8025_Ack(); ///�л�Ӧ 
    }  
    buf[i] = IIC_RX8025_Read_Byte(0);  
    IIC_RX8025_NAck(); ///�޻�Ӧ 
    IIC_RX8025_Stop();  
}  
  
void R8025AC_Write(u8 sadd,u8 *buf,u8 len)  
{  
    u8 i;  
      
    IIC_RX8025_Start();  
    IIC_RX8025_Send_Byte(0X64); 
    
#ifdef RX8025_AC
    IIC_RX8025_Send_Byte(sadd<<4);  
#elif defined RX8025_T    
    IIC_RX8025_Send_Byte(sadd); 
#endif    
    for(i=0;i<len;i++)  
    {  
        IIC_RX8025_Send_Byte(buf[i]);  
    }  
    IIC_RX8025_Stop();  
}  


   
void R8025AC_Init(HOUR_MODE the_mode)
{
    IIC_RX8025_Init();

    IIC_RX8025_Start();  
    IIC_RX8025_Send_Byte(0x64); 
#ifdef RX8025_AC
    IIC_RX8025_Send_Byte(0x0E<<4);
#elif defined RX8025_T    
    IIC_RX8025_Send_Byte(0x0E);
    IIC_RX8025_Send_Byte(the_mode); 
#endif    
    IIC_RX8025_Stop();
    
    //////////////////////////////////////////////////////////////
#ifdef INIT_R8025_DATA
    R8025AC_Write(RX8025_SECO_ADDR, R8025_Init_DataBuf, 7);
#endif 
}
  







