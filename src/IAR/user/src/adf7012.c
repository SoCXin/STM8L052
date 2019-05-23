#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义

void dd_write_7021_reg(unsigned char* reg_bytes)
{
    UINT8 i, j;
    UINT8 byte;

    ADF7021_SLE = 0;
    ADF7021_SCLK = 0;


    /* Clock data out MSbit first */
//#ifdef _BIG_ENDIAN_
//    for (i=0; i<=3; i++)
//#else
//    for (i=3; i>=0; i--)
//#endif
    for (i=0; i<=3; i++)
    {
        byte = reg_bytes[i];

        for (j=8; j>0; j--)
        {
            ADF7021_SCLK = 0;
            if (byte & 0x80) ADF7021_SDATA = 1;
            else ADF7021_SDATA = 0;
            ADF7021_SCLK = 1;
            byte += byte; // left shift 1
        }
        ADF7021_SCLK = 0;
    }


    /* Strobe the latch */

    ADF7021_SLE = 1;
    ADF7021_SLE = 1; // Slight pulse extend
    ADF7021_SDATA = 0;
    ADF7021_SLE = 0;

    /* All port lines left low */

}

void dd_set_ADF7021_Power_on_Init(void)
{
  ADF70XX_REG_T register_value;
	if (ADF7021_POWER == FG_NOT_allow_out)
	{
		ADF7021_POWER = FG_allow_out;  
		//ADF7021_CE=1;
		//Delayus(65);             //delay 100us
                Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
	//write R3
            //register_value.whole_reg = 0x0021d043;    //BATTERY MEASURE IS < 2.35V
	    register_value.whole_reg = ROM_adf7012_value[3].whole_reg & 0xFFFFFFF3; 
            dd_write_7021_reg(&register_value.byte[0]);
            Delayus(25);  //delay 40us		
	}
}


void dd_set_ADF7021_Power_on(void)
{
//	if (ADF7021_CE == 0)
//	{
//		ADF7021_CE = 1;
	if (ADF7021_POWER == FG_NOT_allow_out)
	{
		ADF7021_POWER = FG_allow_out;  
		//ADF7021_CE=1;
		//Delayus(65);             //delay 100us
                Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us
		Delayus(130);             //delay 200us  
	}
}


void dd_set_TX_mode(void)
{
//	ADF70XX_REG_T register_value;       FSK mode
//        //write R0
//	register_value.whole_reg = 0x01082000;
//	dd_write_7021_reg(&register_value.byte[0]);
//	Delayus(79);		//delay 40us
//        
//	//write R1
//	register_value.whole_reg = 0x000aa6e1;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(79);		//delay 40us
//
//	//write R2
//	register_value.whole_reg = 0x00001aa2;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(79);		//delay 40us	
//	
//	//write R3
//	register_value.whole_reg = 0x0022a0cf;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(79);		//delay 40us	


  
//	ADF70XX_REG_T register_value;      GFSK mode
//        //write R0
//	register_value.whole_reg = 0x01084000;
//	dd_write_7021_reg(&register_value.byte[0]);
//	Delayus(25);		//delay 40us
//        
//	//write R1
//	register_value.whole_reg = 0x00154dc1;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(25);		//delay 40us
//
//	//write R2
//	register_value.whole_reg = 0x01120aa6;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(25);		//delay 40us	
//	
//	//write R3
//	register_value.whole_reg = 0x0022a0cf;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(25);		//delay 40us	



////modify  2014 6 5
//      ADF70XX_REG_T register_value;
//
//      //write R0
//      register_value.whole_reg = 0x0884000;
//      dd_write_7021_reg(&register_value.byte[0]);
//      Delayus(25);  //delay 40us
//        
//      //write R1
//      register_value.whole_reg = 0x00154dc1;
//      dd_write_7021_reg(&register_value.byte[0]);
//      Delayus(25);  //delay 40us
// 
//      //write R2
//      register_value.whole_reg = 0x011209a6;
//      dd_write_7021_reg(&register_value.byte[0]);
//      Delayus(25);  //delay 40us 
// 
//      //write R3
//      //register_value.whole_reg = 0x0021a04f;    //DIGITAL LOCK DETECT
//      register_value.whole_reg = 0x0021d04f;    //BATTERY MEASURE IS < 2.35V
//      dd_write_7021_reg(&register_value.byte[0]);
//      Delayus(25);  //delay 40us   
  
  
  
//modify  2014 7 10
      ADF70XX_REG_T register_value;

      //write R0
      register_value.whole_reg = ROM_adf7012_value[0].whole_reg;
      dd_write_7021_reg(&register_value.byte[0]);
      Delayus(25);  //delay 40us
        
      //write R1
      register_value.whole_reg = ROM_adf7012_value[1].whole_reg;
      dd_write_7021_reg(&register_value.byte[0]);
      Delayus(25);  //delay 40us
 
      //write R2
      register_value.whole_reg = ROM_adf7012_value[2].whole_reg;
      dd_write_7021_reg(&register_value.byte[0]);
      Delayus(25);  //delay 40us 
 
      //write R3
      //register_value.whole_reg = 0x0021a04f;    //DIGITAL LOCK DETECT
      register_value.whole_reg = ROM_adf7012_value[3].whole_reg;    //BATTERY MEASURE IS < 2.35V
      dd_write_7021_reg(&register_value.byte[0]);
      Delayus(25);  //delay 40us    
}