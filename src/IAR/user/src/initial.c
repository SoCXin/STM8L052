#include  <iostm8l051f3.h>				// CPU�ͺ� 
#include "Pin_define.h"		// �ܽŶ���
#include "initial.h"		// ��ʼ��  Ԥ����
#include "ram.h"		// RAM����
#include "key_and_Other.h"		// ����

void RAM_clean(void)      // ���RAM 
{			
  asm("ldw X,#0");
  asm("clear_ram:");
  asm("clr (X)");
  asm("incw X");
  asm("cpw X,#0x3f0");	
  //asm("jrule clear_ram");
}  

void WDT_init(void)
{
//  IWDG_KR=0x55;
//  IWDG_PR=3;
//  IWDG_RLR=0xFF;
//  IWDG_KR=0xCC;
  
  IWDG_KR=0xCC;
  IWDG_KR=0x55;
  IWDG_PR=3;
  IWDG_KR=0xAA;    
}
void ClearWDT(void)
{
  IWDG_KR=0xAA;
}


void SysClock_Init( void )
{ 				// ϵͳʱ�ӣ��ⲿʱ�ӣ�
//	CLK_ICKR_HSIEN = 1;						// ʹ���ڲ�RC OSC
//	CLK_ECKR_HSEEN = 1;						// ���ⲿ���� 
//	while(( CLK_ECKR & 0x02 ) == 0 );		// ����ⲿ���� 
//	CLK_SWR = 0xb4;							// ָ��HSEΪ��ʱ�� 
//	while(( CLK_SWCR & 0x08 ) == 0 );		// �ȴ��ⲿ���� 
//	CLK_SWCR_SWEN = 1;						// ִ���л�
//	CLK_CKDIVR = 0x00;						// ����ʱ�ӷ�Ƶ������Ƶ��
//	//---------------------------------------- ����
//	CLK_PCKENR1 = 0xF6;						// T1~4,UART1,SPI
//	CLK_PCKENR2 = 0x08;						// ADC

	
	CLK_ICKCR_HSION = 1;				// ʹ���ڲ�RC OSC��16.00MHz��
	while(( CLK_ICKCR & 0x02 ) == 0 );		// ����ڲ����� 
	CLK_SWR = 0x01;					// ָ��HSIΪ��ʱ�� 
//	while(( CLK_SWCR & 0x08 ) == 0 );		// �ȴ�HSI�л� 
	CLK_SWCR_SWEN = 1;						// ִ���л�
	CLK_CKDIVR = 0x02;		// ����ʱ�ӷ�Ƶ  f HSI= f HSI RC���/4    f CPU= f MASTER
	//---------------------------------------- ����  
	//CLK_PCKENR1 = 0x84;						// T1,UART1
	CLK_PCKENR1 =0x66;// 0x64;	// T4,UART1,beep    2015.3.11����
	CLK_PCKENR2 = 0x03;						// ADC,T1	
	
	CLK_ICKCR_LSION = 1;				// ʹ���ڲ�LSI OSC��38KHz��
	while(CLK_ICKCR_LSIRDY == 0 );		// ����ڲ�LSI OSC 
}

void beep_init( void )
{ 	
     //BEEP_CSR=0x4E; 
  BEEP_CSR2=0;
  BEEP_CSR2_BEEPDIV=3;
  BEEP_CSR2_BEEPSEL=1; 
  CLK_CBEEPR_CLKBEEPSEL0=1;
  CLK_CBEEPR_CLKBEEPSEL1=0;
}
/****************�˿�����˵��***************************
CR1�Ĵ���  ��� Output��1=���졢0=OC��
           ���� Input��1=������0=������
***************end************************************/
void VHF_GPIO_INIT(void)   // CPU�˿�����
{
 /* ADF7012 register interface */  
  ADF7021_SCLK_direc = Output;   
  ADF7021_SCLK_CR1 = 1;	
  ADF7021_SCLK=0;
  
  ADF7021_SDATA_direc = Output;
  ADF7021_SDATA_CR1 = 1;
  ADF7021_SDATA=0;
  
  ADF7021_SLE_direc = Output;
  ADF7021_SLE_CR1 = 1;  
  ADF7021_SLE=0;
  
  ADF7021_POWER_direc = Output;
  ADF7021_POWER_CR1 = 1; 
  ADF7021_POWER=FG_NOT_allow_out;
  //ADF7021_CE_direc = Output;
  //ADF7021_CE_CR1 = 1; 
  //ADF7021_CE=0;
  
  ADF7021_MUXOUT_direc=Input;
  ADF7021_MUXOUT_CR1=1;    //ADF7012Ҫ���������
  
/* Other ADF7021 connections */
  ADF7021_DATA_tx_direc = Output; // Output   ����DATA��
  ADF7021_DATA_tx_CR1 = 1;
  ADF7021_DATA_tx=0;
  
  ADF7021_DATA_CLK_direc= Input;
  ADF7021_DATA_CLK_CR1= 1;

  PIN_KEY_OPEN_direc = Input; 	// ����  OPEN��
  PIN_KEY_OPEN_CR1 = 1;
  
  PIN_KEY_STOP_direc = Input;    // ����  STOP��
  PIN_KEY_STOP_CR1 = 1;
  
  PIN_KEY_CLOSE_direc = Input;   // ����  CLOSE��
  PIN_KEY_CLOSE_CR1 = 1; 
  
  PIN_KEY_VENT_direc = Input;   // ����  ������
  PIN_KEY_VENT_CR1 = 1;
  
  PIN_KEY_LOGIN_direc = Input;   // ����  LOGIN��
  PIN_KEY_LOGIN_CR1 = 1;

  PIN_LED_direc = Output;     // Output   LED��
  PIN_LED_CR1 = 1;

  PIN_BEEP_direc = Output;    // Output   ������
  PIN_BEEP_CR1 = 1;
  PIN_BEEP=0;
  
  PIN_POWER_CONTROL_direc = Output;    // Output   ��Դ����
  PIN_POWER_CONTROL_CR1 = 1;
  PIN_POWER_CONTROL=0;
  
  PIN_test_mode_direc=Input;    // ����     test��
  PIN_test_mode_CR1=1;
  
  PIN_POWER_AD_direc = Input;     // ����     ��Դ���AD�� 

}
void _Init_RAM(void)
{
  TB_100ms = BASE_100ms;
  //TB_5s=TB_50s;//50;
  TB_51s=26;//69;
  TB_5s=TB_51s-1;
  
  	/*		Timer		*/
								// General 1s timer
	
	/*		Key		*/
	_SetKeyChatterCount() ;									// Chatter counter set
	m_KeyNew = m_KeyOld = d_KeyNoPush ;
	m_KindOfKey          = d_IdleKey ;
	//mb_Keycheck          = d_Clear ;
	mb_NoPush            = d_On ;
	mb_NoPushWait        = d_Clear ;
	_ClearSpecialMultiKeyState() ;
	m_TimerKey = d_Clear ;
	m_TimerKeyMonitor = d_Clear ;
}
//===================Delayus()��ʱ===============//
void Delayus(unsigned char timer)
{
unsigned char x;                   //��ʱT=(timer-1)*1.5+3 us
 for(x=0;x<timer;x++)
     __asm("nop");
}