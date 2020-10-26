/*******************************************************************************
****����Ƕ��ʽϵͳ STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****�汾:V1.0
****����:14-2-2014
****˵��:��оƬSTM8S105C4T6
********************************************************************************/
#include "stm8l15x.h"
#include "24c0x.h"
/*******************************************************************************
****��������:IIC��ʼ������
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void InitIIC(void)
{
  I2C_DeInit(I2C1);
  I2C_Init(I2C1,IICSPEED, EEPROM_ADDRESS, I2C_Mode_I2C,I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);
  I2C_Cmd(I2C1,ENABLE);
}

/*******************************************************************************
****��������:��ʱ����
****��������:
****�汾:V1.0
****����:14-2-2014     
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void WaiteI2C_SDA_Idle(void )
{
  unsigned int nCount=0xfff;
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

/*******************************************************************************
****��������:��EEPROMд��һҳ����
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-д�����ݻ�����addr-��ַnum-Ҫд���������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_WriteOnePage(unsigned char *buffer,unsigned char addr,unsigned char num)
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
    
      /* ����EV6 ��������������һ��Ӧ���ź�*/
     while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_AcknowledgeConfig(I2C1,ENABLE);
    }
  
    /* ������λ*/
    I2C_GenerateSTOP(I2C1,ENABLE);
}
/*******************************************************************************
****��������:��EEPROMд������
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-д�����ݻ�����addr-��ַnum-Ҫд���������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_WriteData(unsigned char *buffer,unsigned char addr,unsigned char num)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = addr % EEPROM_Page_Byte_Size;/*����һҳ�Ŀ�ʼд�ĵ�ַ*/
  count = EEPROM_Page_Byte_Size - Addr;/*����һҳ�ĵ�ַʣ������*/
  NumOfPage =  num / EEPROM_Page_Byte_Size;/*д��������ҳ��*/
  NumOfSingle = num % EEPROM_Page_Byte_Size;/*д������ҳʣ�������*/
 
  /* д���ĵ�ַ����ҳ���׵�ַ  */
  if(Addr == 0){ 
    /*д�����ֽ�������һҳ*/
    if(NumOfPage == 0) 
      EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
     
    /*д�����ֽ�������һҳ*/
    else{
      while(NumOfPage--){
        EEPROM_WriteOnePage(buffer, addr, EEPROM_Page_Byte_Size ); /*дһҳ*/
        addr +=  EEPROM_Page_Byte_Size ;
        buffer+= EEPROM_Page_Byte_Size ;
        WaiteI2C_SDA_Idle();
      }/*д����ҳ*/
      if(NumOfSingle!=0){/*дβ��*/
        EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
      }
    }
  }
  /* ����д���ĵ�ַ����ҳ���׵�ַ*/
  else { 
  
    if (NumOfPage == 0) {/*д�����ֽ�������һҳ */
    
      if (NumOfSingle > count){
       /*Ҫд������ҳʣ����������ڲ���һҳ�ĵ�ַʣ������*/
        temp = NumOfSingle - count;
        EEPROM_WriteOnePage(buffer, addr, count);/*�ѵ�ǰҳ�ĵ�ַд��*/
        WaiteI2C_SDA_Idle();
        addr +=  count;
        buffer += count;
        EEPROM_WriteOnePage(buffer, addr, temp);/*���µ�һҳдʣ����ֽ�*/
      }
      else
      {
        EEPROM_WriteOnePage(buffer, addr, num);
      }
    }
    else{ /*д�����ֽ�������һҳ*/
    
      num -= count;
      NumOfPage =  num / EEPROM_Page_Byte_Size;
      NumOfSingle = num % EEPROM_Page_Byte_Size;

      EEPROM_WriteOnePage(buffer, addr, count);/*�ѵ�ǰҳ�ĵ�ַд��*/
      WaiteI2C_SDA_Idle();
      addr +=  count;
      buffer += count;

      while (NumOfPage--)
      {
        EEPROM_WriteOnePage(buffer, addr, EEPROM_Page_Byte_Size);
        WaiteI2C_SDA_Idle();
        addr +=  EEPROM_Page_Byte_Size;
        buffer += EEPROM_Page_Byte_Size;
      }

      if (NumOfSingle != 0)
      {
        EEPROM_WriteOnePage(buffer, addr, NumOfSingle);
      }
    }
  }
  WaiteI2C_SDA_Idle();
}
/*******************************************************************************
****��������:��EEPROM��������
****��������:
****�汾:V1.0
****����:14-2-2014
****��ڲ���:buffer-�������ݻ�����addr-��ַnum-Ҫ������������Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void EEPROM_ReadData(unsigned char *buffer,unsigned char addr,unsigned char num)
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
  while(num)  
  {

      /* ����EV6 */
    if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){      
      /*�� EEPROM ��ȡһ���ֽ�*/
      *buffer = I2C_ReceiveData(I2C1);
      /* ָ��ָ���¸�����ֽڵĵ�ַ*/
      buffer++;  
      /* �������һ���ֽ�*/
      if(num == 1){
      /* ����ҪӦ��*/
        I2C_AcknowledgeConfig(I2C1,DISABLE);
      /* ������λ*/
        I2C_GenerateSTOP(I2C1,ENABLE);  
      }
      else      
        /* �������һ���ֽ�����豸����Ӧ���ź�*/
        I2C_AcknowledgeConfig(I2C1,ENABLE);
      num--;
    }   
  }
}