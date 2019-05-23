#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义
#include "uart.h"		// uart
#include "eeprom.h"		// eeprom
#include "adf7012.h"		// RF IC
#include "ad.h"		       //  A/D


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void START_AD_SAMPLER(void){	// 启动AD转换（中断模式）
        ADC1_TRIGR1_VREFINTON =1;              //内部1.2V参考电压 
        ADC1_CR1_ADON = 1;			// ADC开启
        ADC1_CR1_CONT =0;              //连续循环
        ADC1_CR1_RES = 0;		// AD数据位        
        ADC1_CR3_SMTP2= 1;
        
        ADC1_SQR1_CHSEL_S28=1;
        
//ADC1_TRIGR1_TRIG28=1;
//ADC1_CR3_CHSEL = 11;//28;//11;					// ADC输入引脚AIN11        	        
//ADC1_SR_EOC = 0;					// 清除EOC转换结束标志
//ADC1_CR1_ADON = 1;	                            // 
//ADC1_CR1_EOCIE =1;
        
        ADC1_CR1_START =1;
}
				// ADC中断服务程序
void ADC2_EOC_INT(void){
	ADC1_SR_EOC = 0;	 // 关ADC中断
        AD_DATA_BUFF[1] = ADC1_DRH;
        AD_DATA_BUFF[0] = ADC1_DRL;
	RAM_BAT_SUM += AD_DATA_BUFF[1]*256+AD_DATA_BUFF[0];
        RAM_BAT_CNT++;
        if(RAM_BAT_CNT>=20){
		RAM_BAT_CNT = 0;
		RAM_BAT_AVG = RAM_BAT_SUM/20;
                BAT_Voltage_value=5013504/RAM_BAT_AVG;
		RAM_BAT_SUM = 0;		// clean sum
	}  
        START_AD_SAMPLER();
}
//BAT_out=0   正常电压  >=2.4V
//       =1   欠压      2.2v~2.4v  bi.bi.bi....
//       =2   不允许工作电压（因为发射中心频率偏移过大）  <2.2v   什么都不动作
void AD_control(void){
      if(TIME_power_on_AD)
      {
          if(BAT_Voltage_value>2400)BAT_out=0;
          else if((BAT_Voltage_value<=2400)&&(BAT_Voltage_value>2200))BAT_out=1;
          else BAT_out=2; 
      }
}

