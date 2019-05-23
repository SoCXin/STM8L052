#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义
#include "key_and_Other.h"		// 按键

void RAM_clean(void)      // 清除RAM 
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
{ 				// 系统时钟（外部时钟）
//	CLK_ICKR_HSIEN = 1;						// 使能内部RC OSC
//	CLK_ECKR_HSEEN = 1;						// 打开外部晶振 
//	while(( CLK_ECKR & 0x02 ) == 0 );		// 检查外部晶振 
//	CLK_SWR = 0xb4;							// 指定HSE为主时钟 
//	while(( CLK_SWCR & 0x08 ) == 0 );		// 等待外部晶振 
//	CLK_SWCR_SWEN = 1;						// 执行切换
//	CLK_CKDIVR = 0x00;						// 设置时钟分频（不分频）
//	//---------------------------------------- 外设
//	CLK_PCKENR1 = 0xF6;						// T1~4,UART1,SPI
//	CLK_PCKENR2 = 0x08;						// ADC

	
	CLK_ICKCR_HSION = 1;				// 使能内部RC OSC（16.00MHz）
	while(( CLK_ICKCR & 0x02 ) == 0 );		// 检查内部晶振 
	CLK_SWR = 0x01;					// 指定HSI为主时钟 
//	while(( CLK_SWCR & 0x08 ) == 0 );		// 等待HSI切换 
	CLK_SWCR_SWEN = 1;						// 执行切换
	CLK_CKDIVR = 0x02;		// 设置时钟分频  f HSI= f HSI RC输出/4    f CPU= f MASTER
	//---------------------------------------- 外设  
	//CLK_PCKENR1 = 0x84;						// T1,UART1
	CLK_PCKENR1 =0x66;// 0x64;	// T4,UART1,beep    2015.3.11修正
	CLK_PCKENR2 = 0x03;						// ADC,T1	
	
	CLK_ICKCR_LSION = 1;				// 使能内部LSI OSC（38KHz）
	while(CLK_ICKCR_LSIRDY == 0 );		// 检查内部LSI OSC 
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
/****************端口设置说明***************************
CR1寄存器  输出 Output（1=推挽、0=OC）
           输入 Input（1=上拉、0=浮动）
***************end************************************/
void VHF_GPIO_INIT(void)   // CPU端口设置
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
  ADF7021_MUXOUT_CR1=1;    //ADF7012要求必须下拉
  
/* Other ADF7021 connections */
  ADF7021_DATA_tx_direc = Output; // Output   调制DATA线
  ADF7021_DATA_tx_CR1 = 1;
  ADF7021_DATA_tx=0;
  
  ADF7021_DATA_CLK_direc= Input;
  ADF7021_DATA_CLK_CR1= 1;

  PIN_KEY_OPEN_direc = Input; 	// 输入  OPEN键
  PIN_KEY_OPEN_CR1 = 1;
  
  PIN_KEY_STOP_direc = Input;    // 输入  STOP键
  PIN_KEY_STOP_CR1 = 1;
  
  PIN_KEY_CLOSE_direc = Input;   // 输入  CLOSE键
  PIN_KEY_CLOSE_CR1 = 1; 
  
  PIN_KEY_VENT_direc = Input;   // 输入  换气键
  PIN_KEY_VENT_CR1 = 1;
  
  PIN_KEY_LOGIN_direc = Input;   // 输入  LOGIN键
  PIN_KEY_LOGIN_CR1 = 1;

  PIN_LED_direc = Output;     // Output   LED灯
  PIN_LED_CR1 = 1;

  PIN_BEEP_direc = Output;    // Output   蜂鸣器
  PIN_BEEP_CR1 = 1;
  PIN_BEEP=0;
  
  PIN_POWER_CONTROL_direc = Output;    // Output   电源控制
  PIN_POWER_CONTROL_CR1 = 1;
  PIN_POWER_CONTROL=0;
  
  PIN_test_mode_direc=Input;    // 输入     test脚
  PIN_test_mode_CR1=1;
  
  PIN_POWER_AD_direc = Input;     // 输入     电源监测AD脚 

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
//===================Delayus()延时===============//
void Delayus(unsigned char timer)
{
unsigned char x;                   //延时T=(timer-1)*1.5+3 us
 for(x=0;x<timer;x++)
     __asm("nop");
}