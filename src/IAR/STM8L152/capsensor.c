#include "capsensor.h"
unsigned char Capchargeflag;

void Capinit (void){
	GPIO_Init(  Cap_Port,Cap_Charge_Pin,     GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(  Cap_Port,Cap_Detect_Pin,     GPIO_Mode_In_PU_IT);
	TIM2_TimeBaseInit(TIM2_Prescaler_1,TIM2_CounterMode_Up, 0xffff);//16M/8/128=15.625K，0xff=255,255*（1/15.625）=0.01632S，大约61次中断是1S
    GPIO_Init(IR_GPIO_PORT, IR_GPIO_PINS, GPIO_Mode_In_FL_IT);//初始化按键，GPB6上拉中断输入
    TIM2_ARRPreloadConfig(DISABLE);	/* 预装载不使能 */
    TIM2_Cmd(ENABLE);         
    EXTI_SetPinSensitivity (Cap_Detect_Pin,EXTI_Trigger_Falling);

}
void CapCharge(void){

	GPIO_ResetBits(Cap_Port,Cap_Charge_Pin);
	delayms(10);
	GPIO_SetBits(Cap_Port,Cap_Charge_Pin);

}

void GetCap(void){
	CapCharge();	//Pull down capcharger Pin 
	//Wait the interrupt
	Capchargeflag=1;

}