#include  <iostm8l051f3.h>				// CPU�ͺ� 
#include "Pin_define.h"		// �ܽŶ���
#include "initial.h"		// ��ʼ��  Ԥ����
#include "ram.h"		// RAM����
#include "uart.h"		// uart
#include "eeprom.h"		// eeprom
#include "adf7012.h"		// RF IC
#include "ad.h"		       //  A/D


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void START_AD_SAMPLER(void){	// ����ADת�����ж�ģʽ��
        ADC1_TRIGR1_VREFINTON =1;              //�ڲ�1.2V�ο���ѹ 
        ADC1_CR1_ADON = 1;			// ADC����
        ADC1_CR1_CONT =0;              //����ѭ��
        ADC1_CR1_RES = 0;		// AD����λ        
        ADC1_CR3_SMTP2= 1;
        
        ADC1_SQR1_CHSEL_S28=1;
        
//ADC1_TRIGR1_TRIG28=1;
//ADC1_CR3_CHSEL = 11;//28;//11;					// ADC��������AIN11        	        
//ADC1_SR_EOC = 0;					// ���EOCת��������־
//ADC1_CR1_ADON = 1;	                            // 
//ADC1_CR1_EOCIE =1;
        
        ADC1_CR1_START =1;
}
				// ADC�жϷ������
void ADC2_EOC_INT(void){
	ADC1_SR_EOC = 0;	 // ��ADC�ж�
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
//BAT_out=0   ������ѹ  >=2.4V
//       =1   Ƿѹ      2.2v~2.4v  bi.bi.bi....
//       =2   ����������ѹ����Ϊ��������Ƶ��ƫ�ƹ���  <2.2v   ʲô��������
void AD_control(void){
      if(TIME_power_on_AD)
      {
          if(BAT_Voltage_value>2400)BAT_out=0;
          else if((BAT_Voltage_value<=2400)&&(BAT_Voltage_value>2200))BAT_out=1;
          else BAT_out=2; 
      }
}

