/***********************************************************************/
/*  FILE        :ad.H                                                */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/

void ADC2_EOC_INT(void);
void START_AD_SAMPLER(void);
void AD_control(void);

#define VREF 		1224    //1.224V
#define ADC_CONV 	4096