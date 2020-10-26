



#include "irrx.h"



_Bool	Flag_ir = 0;

volatile u8 Flag_ir_start = 0;//������־λ
volatile u16 IR_time = 0;
volatile u32 IR_data=0;


u8 IR_num = 0;

u8 IR_temp[4];


/*

void TIM1_Init(void)
{
	TIM1->CR1 = 0x00;  //���ϵļ��������жϼ�����ͣ
    
	TIM1->IER = 0x00;//��ֹ�ж�
    
	TIM1->PSCRH = 0x00;  //����8��Ƶ
	TIM1->PSCRL = 0x07;  //8Mhz/(7+1)=1Mhz-->1us
    
	TIM1->ARRH = 0xff;
	TIM1->ARRL = 0xff;   //20us
    
	TIM1->CR1 |=0x01;
    
}

*/

//���жϽ��պ�������
void EXIT_Init(void)
{
	//01��PA��23��PB��45��PC��67��PD����
	//P?IS[1:0] ���ô�����ʽ��10���½��ش���
	EXTI->CR1 &=(u8)(~(0x03<<7));  // PA�½��ش���
	EXTI->CR1 |= (0x02<<7);
//        EXTI_CR1 &=(u8)(~(0x03<<0));  // PA�½��ش���
//	EXTI_CR1 |= (0x02<<0);
}

void IR_Init(void)
{
	
	GPIOD->DDR &=(u8)(~(0x01<<2));
	//PA_CR1 &=(u8)(~(0x01<<3));
	GPIOD->CR1 |= (0x01<<2);//��������
	GPIOD->CR2 |= (0x01<<2);//���ⲿ�ж�
	
	EXIT_Init();//RISING and falling EDGE
//	TIM1_Init();
        Time2_Init  ();
        
}



void IR_rx(void)
{
	__IO static u16 IR_time = 0;
	__IO static u16 IR_time_befor=0,IR_time_buff=0;
	

	__IO static u8 Flag_end=0;
	
	
	u16 IR_H,IR_L;
	
	
	
	IR_H = TIM2->CNTRH;
	IR_L = TIM2->CNTRL;
	
	IR_time_buff = ((IR_H&0xff)<<8)|((IR_L&0xff)<<0);
	
	/******************************************************\
	
	@ �˴�ע�ʹ���ɺ���
	
	@ ����ʹ�õ�һ��С�������������ݵ��������ת��
	  ���˳����߼�����
	  ԭ������֮ǰ��STM32������ͨ�ö�ʱ��PWM����һ���ģ�
	  ��Ҫ��ͼ�����������ף�
	  ʵ��һ�°�:	
					(u16)(0)- (u16)(65535) = ?
					(u16)(1000)- (u16)(64535) = ?
	\******************************************************/
	IR_time = IR_time_buff - IR_time_befor;	//��¼�˴�ʱ���
	
	IR_time_befor = IR_time_buff;			//��¼�˴�ʱ��
	

	/****����ʶ��ʱ��****
	 ***����1��0�ı��**
	 
	//--------	IR_time	-->	1500~2000	Ϊ1------------	
	if((IR_time>=1500)&&(IR_time<=2000))//H
	{
		IR_time=0;
		ir_temp++;
		return ;
	}
	//--------	IR_time	-->	400~550		Ϊ0------------	
	if((IR_time>=400)&&(IR_time<=550))//L
	{
		IR_time=0;
		ir_temp++;
		return ;
	}
	*/
	
	
	//800us
	if(IR_time<=300)////�����½��ؼ��С��560u*2�����Ӳ�
	{
		IR_time=0;
		return ;
	}
	//13000us
	if(IR_time >= 25000)//13ms�������ɹ�
	{
		Flag_ir_start = 1;
		IR_num = 0;		//�������
		IR_data=0;
	}
	
	//receive data
	if((Flag_ir_start==1)&&(IR_time<=5000))//2600us
	{
		
		IR_data>>=1;
		if(IR_time>=4000)//1700us///1500
		{
			IR_data |= 0x80000000;
		}
		IR_num++;
		if(IR_num>31)
		{
			Flag_end = 1;
			Flag_ir_start=0;
			IR_time = 0;
		}
	}

	
	if(Flag_end==1)
	{
		Flag_end = 0;
		
		IR_temp[0] = (u8)((IR_data>>0)&0xff);
		IR_temp[1] = (u8)((IR_data>>8)&0xff);
		IR_temp[2] = (u8)((IR_data>>16)&0xff);
		IR_temp[3] = (u8)((IR_data>>24)&0xff);
		
		//
		IR_data=0;
		IR_num=0;
		
		if((IR_temp[0]!=(u8)(~IR_temp[1]))&&(IR_temp[2]!=(u8)(~IR_temp[3])))
		{
			//У��ʧ��
			//PA_ODR ^= (0x01<<3);
			
		}
		else//У��ɹ�
		{
			
			Flag_ir = 1;
		}
		
	}
	
	IR_time = 0;
	
}





