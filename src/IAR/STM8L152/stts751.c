#include "stts751.h"

 int temp10;
static void  delay_us(u16 num){
    while (num--);

}

u16 STTSRead(u8 reg)
{
	u16 res;
        delay_us(10);
  /* µ»¥˝ø’œ– */
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    /* ∑¢∆ ºŒª */
    I2C_GenerateSTART(I2C1,ENABLE);
    /* ≤‚ ‘EV5 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
  
    /* …Ë÷√I2C¥”∆˜º˛µÿ÷∑£¨I2C÷˜…Ë±∏Œ™–¥ƒ£ Ω*/
    I2C_Send7bitAddress(I2C1,STTS751_ADDRESS, I2C_Direction_Transmitter);
    /* ≤‚ ‘EV6 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    I2C_SendData(I2C1,(u8)(reg)); 
    /* ≤‚ ‘EV8 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
        delay_us(10);   
    /* ∑¢∆ ºŒª */ 
    I2C_GenerateSTART(I2C1,ENABLE);
            delay_us(10); 
    /* ≤‚ ‘EV5 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
          delay_us(10); 
    /* …Ë÷√I2C¥”∆˜º˛µÿ÷∑£¨I2C÷˜…Ë±∏Œ™∂¡ƒ£ Ω*/
    I2C_Send7bitAddress(I2C1,STTS751_ADDRESS, I2C_Direction_Receiver);
            delay_us(10); 
    /* ≤‚ ‘EV6 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
        delay_us(100);    
    /*∂¡»° ˝æ› */
     
        res =   I2C_ReceiveData(I2C1);      
       
       I2C_AcknowledgeConfig(I2C1,DISABLE);
       delay_us(100);
      /* ∑¢Ω· ¯Œª*/
       I2C_GenerateSTOP(I2C1,ENABLE);  
       delay_us(10);
       I2C_AcknowledgeConfig(I2C1,ENABLE);
        
	return res;		
}


u8 STTSSet(u8 reg,u8 data) 				 
{   delay_us(10);
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* ∑¢∆ ºŒª */
    I2C_GenerateSTART(I2C1,ENABLE);
    
    /* ≤‚ ‘EV5 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)); 
    
    /* …Ë÷√I2C¥”∆˜º˛µÿ÷∑£¨I2C÷˜…Ë±∏Œ™–¥ƒ£ Ω*/
    I2C_Send7bitAddress(I2C1,STTS751_ADDRESS, I2C_Direction_Transmitter);
  
    /* ≤‚ ‘EV6 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
  while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
  
    I2C_SendData(I2C1,(u8)reg); 
    /* ≤‚ ‘EV8 £¨ºÏ≤‚¥”∆˜º˛∑µªÿ“ª∏ˆ”¶¥–≈∫≈*/
    while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING));  
  
    /* ≤ª∂œÕ˘¥”…Ë±∏–¥ ˝æ›*/

      
      I2C_SendData(I2C1,data);
      delay_us(10);
      while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
      I2C_AcknowledgeConfig(I2C1,ENABLE);
//      I2C_SendData(I2C1,LSB);   
      delay_us(10);      

    I2C_GenerateSTOP(I2C1,ENABLE);
      I2C_AcknowledgeConfig(I2C1,ENABLE);
    return 0;
}


unsigned char initStts751(void){
	u16 res;
	res=STTSRead(STTMID);
	if (res==0x53){
		STTSSet(STTCONFIG,0X00);
		STTSSet(STTTHERMLIMIT,0X55);

		return 0;
	}


	else return 1;



}
//temp10=  temp*10 return 0 is welldone
unsigned char ReadSttstemp(void){
	unsigned char tmp_H,tmp_L;
	int sttstemp;
	tmp_L=STTSRead(STTTEMP_L);
	tmp_H=STTSRead(STTTEMP_H);
	sttstemp=(u16)tmp_H*10;
	sttstemp+=(tmp_L>>4);
	temp10=sttstemp;
	return 0;

}

