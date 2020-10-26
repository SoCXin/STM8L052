

#include "rx8025t.h"

#define  INIT_R8025_DATA    ////初始化日期时间
#define     RX8025_T
//#define RX8025_AC

#define  EEPROM_ADDRESS 0x64

u8  R8025_DataBuf[7];///定义R8025读取数组

#ifdef INIT_R8025_DATA
static u8  R8025_Init_DataBuf[7] = {

    0x00, 0x48, 0x15, 0x00, 0x01, 0x03, 0x18  ///初始化日期时间：16-10-21 12:00:00

};
#endif
static void  delay_us(u16 num){
    while (num--);

}
void Rtc8025_Init(void)
{
   while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* 发起始位 */
    I2C_GenerateSTART(I2C1,ENABLE);
    
    /* 测试EV5 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* 设置I2C从器件地址，I2C主设备为写模式*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
    /* 测试EV6 ，检测从器件返回一个应答信号*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData(I2C1,(u8)(0X0E)); 
    /* 测试EV8 ，检测从器件返回一个应答信号*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* 不断往从设备写数据*/

      
      I2C_SendData(I2C1,0x20); 
  

  
    /* 发结束位*/
    I2C_GenerateSTOP(I2C1,ENABLE);
    
   
}
  

/*
 *读写例程请参考下面的微博
 *http://blog.csdn.net/u012351051/article/details/50896822
 */
   
 void Rtc8025_ReadData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
   /* 等待空闲 */
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
  
    /* 发起始位 */
    I2C_GenerateSTART(I2C1,ENABLE);
    /* 测试EV5 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* 设置I2C从器件地址，I2C主设备为写模式*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
    /* 测试EV6 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
   
    I2C_SendData(I2C1,(u8)(addr)); 
    /* 测试EV8 ，检测从器件返回一个应答信号*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
    /* 发起始位 */ 
    I2C_GenerateSTART(I2C1,ENABLE);
    /* 测试EV5 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* 设置I2C从器件地址，I2C主设备为读模式*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Receiver);
    /* 测试EV6 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    /*不断在从从设备读取数据 */
    
 //    I2C_AcknowledgeConfig(I2C1,ENABLE);//ADD TO HERE NO
  while(num)  
  {
  //          Delay(0xFFF); 
      /* 测试EV6 */
    if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){      
      /*从 EEPROM 读取一个字节*/
            delay_us(10); 
      *buffer = I2C_ReceiveData(I2C1);
      /* 指针指向下个存放字节的地址*/
      buffer++;  
      
      /* 读到最后一个字节*/
      if(num == 1){
      /* 不需要应答*/
        I2C_AcknowledgeConfig(I2C1,DISABLE);
      /* 发结束位*/
        I2C_GenerateSTOP(I2C1,ENABLE); 
          delay_us(10); 
      }
      else      
        /* 不是最后一个字节向从设备发送应答信号*/
       I2C_AcknowledgeConfig(I2C1,ENABLE);
      num--;
    }   
                delay_us(10); 
  }
  
        I2C_AcknowledgeConfig(I2C1,ENABLE);//TMD THIS DON'T FORGET!
}
void Rtc8025_WriteData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
  /* 等待空闲 */
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* 发起始位 */
    I2C_GenerateSTART(I2C1,ENABLE);
    
    /* 测试EV5 ，检测从器件返回一个应答信号*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* 设置I2C从器件地址，I2C主设备为写模式*/
    I2C_Send7bitAddress(I2C1,EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
    /* 测试EV6 ，检测从器件返回一个应答信号*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData(I2C1,(u8)(addr)); 
    /* 测试EV8 ，检测从器件返回一个应答信号*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* 不断往从设备写数据*/
    while(num--)  
    {
      
      I2C_SendData(I2C1,*buffer); 
  
      /*指针指向下一个地址*/
      buffer++; 
    Delay(0x10);
      /* 测试EV6 ，检测从器件返回一个应答信号*/
     while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_AcknowledgeConfig(I2C1,ENABLE);
    }
  
    /* 发结束位*/
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
        IIC_RX8025_Ack(); ///有回应 
    }  
    buf[i] = IIC_RX8025_Read_Byte(0);  
    IIC_RX8025_NAck(); ///无回应 
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
  







