/***********************************************************************/
/*  FILE        :Timer.h                                               */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8S003     Crystal: 4M HSI                          */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/


////============================================ Timer 1
//void TIM1_init(void);					
//void TIM1_OVR_UIF(void);	

void TIM3_init(void);   //2015.3.11ÐÞÕý
void Tone_OFF(void);    //2015.3.11ÐÞÕý
//============================================ Timer 4
void TIM4_Init(void);					
void TIM4_UPD_OVF(void);	