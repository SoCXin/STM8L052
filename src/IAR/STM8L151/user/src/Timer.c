#include  <iostm8l051f3.h>				// CPU�ͺ� 
#include "Pin_define.h"		// �ܽŶ���
#include "initial.h"		// ��ʼ��  Ԥ����
#include "ram.h"		// RAM����

////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Timer 1 start
//void TIM1_init(void){						// 0.8333ms��ʱ��
//	TIM1_PSCRH = 0x00;				// 10Mϵͳʱ�Ӿ�Ԥ��Ƶf=fck/(PSCR+1)
//	TIM1_PSCRL =0x00; 		     // PSCR=0x1F3F��f=3.6864M/(0x1200)=800Hz����������1.25ms
//	TIM1_ARRH = 0x20;					// �Զ����ؼĴ���ARR=5
//	TIM1_ARRL = 0x8c;		//78.125*(3+1)=312.5us		// ÿ2.5ms����һ���ж�
//	TIM1_IER = 0x01;						// ��������ж�
//	TIM1_CR1 = 0x01;					// ������ʹ�ܣ���ʼ����
//}
//
//void TIM1_OVR_UIF(void){        
//	TIM1_SR1 = 0;						// ����жϱ��
//}


void TIM3_init(void){			// 2015.3.11����
        TIM3_CCMR2 = TIM3_CCMR2 | 0x70;
        TIM3_CCER1 = TIM3_CCER1 | 0x30;
	TIM3_ARRH = 0x01;//0x02;					// �Զ����ؼĴ���ARR=5
	TIM3_ARRL = 0xF8;//0x00;		//78.125*(3+1)=312.5us		// ÿ2.5ms����һ���ж�
	//TIM2_IER = 0x01;						// ��������ж�
        TIM3_CCR2H= 0x00;//0x01;
        TIM3_CCR2L= 0xFC;//0x00;
        TIM3_PSCR = 0x02;				// 10Mϵͳʱ�Ӿ�Ԥ��Ƶf=fck/(2��PSCR�η�)
        //TIM3_EGR_bit.UG=1;
	//TIM2_CR1 = 0x01;					// ������ʹ�ܣ���ʼ����
        TIM3_CR1 = TIM3_CR1 | 0x01;
        TIM3_BKR= 0x80;
}
void Tone_OFF(void){		// �ر�Tone   2015.3.11����
	TIM3_CR1_CEN = 0;		// Timer 3 Disable
        PIN_BEEP=0;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Timer 4 start   1ms
void TIM4_Init(void){				
	TIM4_PSCR = 0x04;	// Timer 4 prescaler  ������ʱ��Ƶ��  f CK_CNT  =f CK_PSC  / 2��N�η�
        //TIM4_PSCR = 0x08;	// Timer 4 prescaler  ������ʱ��Ƶ��  f CK_CNT  = f CK_PSC/ 2(PSC[3:0])
	TIM4_ARR  = 0xF9;						// Timer 4 period
	TIM4_CR1 |= 0x01;						// Timer 4 Enable
	TIM4_IER |= 0x01;						// Timer 4 OVR interrupt
}

void TIM4_UPD_OVF(void){
        FG_1ms = 1;
	if (TB_100ms)	--TB_100ms;
        else{                            
	  TB_100ms = BASE_100ms;
	  FG_100ms = 1;	      // 100mS FLAG
	}
	TIM4_SR1_bit.UIF=0;						// ����жϱ��
}


////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%Timer 4 start   0.244ms
//void TIM4_Init(void){				
//	TIM4_PSCR = 0x04;	// Timer 4 prescaler  ������ʱ��Ƶ��  f CK_CNT  =f CK_PSC  / 2��N�η�
//	TIM4_ARR  = 0x3C;						// Timer 4 period
//	TIM4_CR1 |= 0x01;						// Timer 4 Enable
//	TIM4_IER |= 0x01;						// Timer 4 OVR interrupt
//}
//
//void TIM4_UPD_OVF(void){
//        if (TB_976us)--TB_976us;
//        else{                            
//	  TB_976us = BASE_976us;
//	  FG_976us = 1;	      // 976us FLAG
//	}
//        if(FLAG_beep==1)PIN_BEEP=!PIN_BEEP;
//	
//	TIM4_SR_bit.UIF=0;						// ����жϱ��
//}