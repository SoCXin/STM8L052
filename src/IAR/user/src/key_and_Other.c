#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义
#include "adf7012.h"		// RF IC
#include "EXIT_FUN.h"		// 外部EXIT中断
#include "key_and_Other.h"		// 按键
#include "eeprom.h"		// eeprom
#include "uart.h"		// uart
#include "ad.h"		// ad
#include "Timer.h"		// 定时器

void key_check(void)
{
//    if (TB_100ms)--TB_100ms;
//    else{                            
//	  TB_100ms = BASE_100ms;
//	  FG_100ms = 1;	      // 100mS FLAG
//	}
  
  
   if(FG_1ms){
    FG_1ms=0; 
    if(TIME_power_on_AD)TIME_power_on_AD--;
    if(m_TimerRegMode)--m_TimerRegMode;        
    if(m_KeyDupliSetTimeout)--m_KeyDupliSetTimeout;
    if(m_TimerKeyMonitor)--m_TimerKeyMonitor;
    if(m_KeyDupli1stTimer)--m_KeyDupli1stTimer;
    else FG_d_StopKey=0;
    if	( FG_d_StopKey &&m_KeyDupli1stTimer){
      time_led++;
      if(time_led>=100){time_led=0;PIN_LED=!PIN_LED;}
    }
    if(m_TimerKey)--m_TimerKey;
    
    if(TIME_BEEP_on){
      --TIME_BEEP_on;
      if(FG_beep_on==0){FG_beep_on=1;FG_beep_off=0;TIM3_init();//BEEP_CSR2_BEEPEN=1; //2015.3.11修正
                        if(FG_LED_on)PIN_LED=1;
                       }
    }
    else if(TIME_BEEP_off){
      --TIME_BEEP_off;
      if(FG_beep_off==0){FG_beep_off=1;FG_beep_on=0;Tone_OFF();//BEEP_CSR2_BEEPEN=0;  //2015.3.11修正
                        if(FG_LED_on)PIN_LED=0;
                       }
    }
    else if(TIME_BEEP_freq){
      --TIME_BEEP_freq;
      TIME_BEEP_on=BASE_TIME_BEEP_on;
      TIME_BEEP_off=BASE_TIME_BEEP_off;
      if(FG_beep_on==0){FG_beep_on=1;FG_beep_off=0;TIM3_init();//BEEP_CSR2_BEEPEN=1;  //2015.3.11修正
                        if(FG_LED_on)PIN_LED=1;
                       }      
    }
    else  FG_LED_on=0; 
    
//    if(FG_BAT_value==0){START_AD_SAMPLER();FG_BAT_value=1;}
//    else {ADC_read();FG_BAT_value=0;}
    _KeyInTx();
    if(FG_10s==1)return;   // 2015.1.31修正3
    _RegistrationMode();
    _DupliFuncSetMode();
    ADC2_EOC_INT();
    ClearWDT(); // Service the WDT    
  }
}


void time_control(void)
{
  if(FG_100ms){
    FG_100ms=0; 
    if(TIME_2s_RestTX)--TIME_2s_RestTX;    //2015.4.13修正    
    if(FG_PWRON==1){
    if ((TB_5s)&&(m_KeyOptSetMode==0))	--TB_5s;
    }    
  }
  
}

//
/****************************************/
/*										*/
/*			Check Key for Tx			*/
/*										*/
/*			  in  : none				*/
/*			  out : none				*/
/*										*/
/****************************************/
//
/*	Key data table	*/
//		||||||||
//		|||||||+ Auto Tx Stop
//		||||||++ Auto Tx Start
//		|||||+++ Reg.
//		||||++++ Vent.
//		|||+++++ Close
//		||++++++ Stop
//		|+++++++ Open
//		++++++++ 1
//
const	uchar	ct_KeyDataTable[]
={ 
	/*	Reg. (0)	*/
0xFB,//	0b11111011,
//	/*	Open (1)	*/
0xBF,//	0b10111111,
//	/*	Stop (2)	*/
0xDF,//	0b11011111,
//	/*	Close (3)	*/
0xEF,//	0b11101111,
//	/*	Vent. (4)	*/
0xF7,//	0b11110111,
//	/*	Open + Stop (5)	*/
0x9F,//	0b10011111,
//	/*	Open + Close (6)	*/
0xAF,//	0b10101111,
//	/*	Open + Vent. (7)	*/
0xB7,//	0b10110111,
//	/*	Open + Reg. (8)	*/
0xBB,//	0b10111011,
//	/*	Stop + Close (9)	*/
0xCF,//	0b11001111,
//	/*	Stop + Reg. (10)	*/
0xDB,//	0b11011011,
//	/*	Close + Reg. (11)	*/
0xEB,//	0b11101011,
//	/*	Close + Vent. (12)	*/
0xE7,//	0b11100111,
//	/*	Vent. + Reg. (13)	*/
0xF3,//	0b11110011,
//	/*	Auto Tx start (14)	*/
0xFD,//	0b11111101,
//	/*	Auto Tx stop (15)	*/
0xFE,//	0b11111110,
//	/*	No push (16)	*/
0xFF,//	0b11111111,
} ;

//
void	_KeyInTx( void )
{
	uchar	i ;
	
	m_KindOfKey = d_Idle ;
        
	m_KeyNew = 0xFF ;
        if(BAT_out==2)return;
		
	/*	Registration sw		*/
	_SwIn( PIN_KEY_OPEN ) ;
	/*	Auto Tx Start sw		*/
	_SwIn( PIN_KEY_STOP ) ;
	/*	Auto Tx Stop sw		*/
	_SwIn( PIN_KEY_CLOSE ) ;
	/*	Auto Tx Vent. sw		*/
	_SwIn( PIN_KEY_VENT ) ;	
	//_SwIn( 1 ) ;
	/*	Auto Tx Reg. sw		*/
	_SwIn( PIN_KEY_LOGIN ) ;	
	/*	Auto Tx Auto Tx Start sw		*/
	_SwIn( 1 ) ;	
	/*	Auto TxAuto Tx Stop sw		*/
	_SwIn( 1 ) ;	
	
        
	if	( m_KeyNew != m_KeyOld )				// Key in
	{												// Different with the last state
		m_KeyOld = m_KeyNew ;
		_SetKeyChatterCount() ;						// Chatter counter set
		_ClearSpecialMultiKeyState() ;				// Clear key continue state
                TIME_Once_twice_switch=5000;   //2015.1.31修正4
                TIME_10s=10100;  //2015.1.31修正3
		return ;
	}
	
	if	( --m_ChatterCount )						// Chattering ok ?
	{
		return ;
	}
	m_ChatterCount = 1 ;
	
	/*		For generate Batt. Low tone		*/
	if	( m_KeyNew != d_KeyNoPush )			// Is key data no push ?
	{												// No, any key push
//		if	( m_BattState )							// Batt. Low ?
//		{											// Yes
//			_ReqBuzzer(d_BuzBattLow) ;				// Request
//		}
	    //if(BAT_value>1000){
           if(TIME_Once_twice_switch)--TIME_Once_twice_switch;    //2015.1.31修正4
           if(TIME_10s)--TIME_10s;    //2015.1.31修正3
	   dd_set_ADF7021_Power_on_Init();
	   //if((ADF7021_MUXOUT)||(FG_BAT)){
           if((BAT_out==1)||(FG_BAT)||(TIME_10s==0)){   //2015.1.31修正3
	      if(FG_BAT==0){
		        FG_BAT=1;
			if(TIME_10s==0){      //2015.1.31修正3
                           BASE_TIME_BEEP_on=103;
                           BASE_TIME_BEEP_off=103;
                        }
                        else {
                           BASE_TIME_BEEP_on=40;
                           BASE_TIME_BEEP_off=60;
                        }
                        TIME_BEEP_on=BASE_TIME_BEEP_on;
                        TIME_BEEP_off=BASE_TIME_BEEP_off;
                        TIME_BEEP_freq=2;	
	      }
	      else if(TIME_BEEP_freq<=1)TIME_BEEP_freq=2;
	    }
	}
	        
	/*		Search of valid key		*/
	for	( i=0; i<17;i++)
	{
		if	( m_KeyNew == ct_KeyDataTable[i] )	// Match ?
		{
			m_KeyNo = i ;
			break ;									// Yes
		}
	}
        key_Value=i;   // 2015.1.31修正3
        if(FG_10s==1)return;   // 2015.1.31修正3
        
	if	( i == 17)			// Found ?
	{												// No
		mb_NoPush = d_Clear ;						// No push clear(Push on)
		_ClearSpecialMultiKeyState() ;				// Special multi key status clear
		mb_NoPushWait = d_On ;						// Set no push wait
		_DupliFuncClear() ;							// Duplicate key function clear
		return ;
	}
	
	if	( mb_NoPushWait )							// Wait for no push ?
	{												// Yes
		if	( m_KeyNew != d_KeyNoPush )		// Is key data no push ?
		{
                        if(((i==5)||(i==6)||(i==7)||(i==9)||(i==12))&&(FLAG_APP_TX==0));    //解决复数输出，按键未松一直发送 
			else return ;								// No
		}
	}
	
	switch	( i )    // Jumo to key function
	{
		case 0 :
		        _FuncReg();
			break ;		
		case 1 :
		case 2 :  
		case 3 : 
		case 4 :   
		        if(FLAG_APP_TX==0)_FuncStop();   //2015.1.31修正4
			break ;	
		case 5 :  
		case 6 :  
		case 7 :  
		case 9 :  
		case 12 :  
		        _FuncOpenStop();
			break ;		
		case 8 :  
		        _FuncOpenReg();
			break ;
		case 10 :  
		        _FuncStopReg();
			break ;
		case 11 :  
		        _FuncCloseReg();
			break ;
		case 13 :  
		        _FuncVentReg();
			break ;
		case 14 :  
		        _FuncAutoTxStart();
			break ;
		case 15 :  
		        _FuncAutoTxStop();
			break ;
		case 16 :  
		        _FuncNoPush();
			break ;
	}
}
/*----------------------------------*/
/*									*/
/*		Open + Stop(5) function		*/
/*		Open + Close(6) function	*/
/*		Open + Vent.(7) function	*/
/*		Stop + Close(9) function	*/
/*		Close + Vent.(12) function	*/
/*									*/
/*----------------------------------*/
//
void	_FuncOpenStop( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	m_KindOfKey = d_Idle ;
	_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
	_ClearSpecialMultiKeyState() ;					// Special multi key status clear
	mb_NoPush = d_Clear ;
	mb_NoPushWait = d_On ;
}
/*----------------------------------*/
/*									*/
/*		Open + Reg. function		*/
/*									*/
/*----------------------------------*/
//
void	_FuncOpenReg( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	mb_NoPush = d_Clear ;
	if	( !mb_RegOpenSw )							// Continue push ?
	{												// No
		mb_RegOpenSw = d_On ;
		m_TimerKey = d_Time3s ;	// Set 3sec key timer
		if(FG_PWRON==0){
	           FG_PWRON=1;
	           PIN_POWER_CONTROL=1;
	           TB_5s=TB_60s;//60;  //5.1秒
                }
		return ;
	}
	
	_Pass3secKey( d_ReqOpenReg ) ;
}
/*----------------------------------*/
/*									*/
/*		Close + Reg. function		*/
/*									*/
/*----------------------------------*/
//
void	_FuncCloseReg( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	if	( !mb_RegCloseSw )							// Continue push ?
	{												// No
		mb_RegCloseSw = d_On ;
		m_TimerKey = d_Time3s ;						// Set 3sec key timer
		if(FG_PWRON==0){
	           FG_PWRON=1;
	           PIN_POWER_CONTROL=1;
	           TB_5s=TB_60s;//60;  //5.1秒
                }
		return ;
	}
	
	_Pass3secKey( d_ReqCloseReg ) ;
}
/*----------------------------------*/
/*									*/
/*		Vent. + Reg. function		*/
/*									*/
/*----------------------------------*/
//
void	_FuncVentReg( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	if	( !mb_RegVentSw )							// Continue push ?
	{												// No
		mb_RegVentSw = d_On ;
		m_TimerKey = d_Time3s ;						// Set 3sec key timer
		if(FG_PWRON==0){
	           FG_PWRON=1;
	           PIN_POWER_CONTROL=1;
	           TB_5s=TB_60s;//60;  //5.1秒
                }
		return ;
	}
	
	_Pass3secKey( d_ReqVentReg ) ;
}
/*----------------------------------*/
/*									*/
/*		Stop + Reg. function		*/
/*									*/
/*----------------------------------*/
//
void	_FuncStopReg( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	if	( !mb_RegStopSw )							// Continue push ?
	{												// No
		mb_RegStopSw = d_On ;
		m_TimerKey = d_Time3s ;						// Set 3sec key timer
		if(FG_PWRON==0){
	           FG_PWRON=1;
	           PIN_POWER_CONTROL=1;
	           TB_5s=TB_60s;//60;  //5.1秒
                }
		return ;
	}
	
	_Pass3secKey( d_ReqStopReg ) ;
}
void	_Pass3secKey( uchar req )
{
	if	( m_TimerKey )								// 3sec passed ?
	{
		if	( !--m_TimerKey )
		{											// Yes
			_ReqTxdEdit( req,0 ) ;
                        FG_Complex_Single_shot=1;   //2015.1.31修正2
			return ;
		}
	}
}
/*------------------------------------------*/
/*											*/
/*		Open/Stop/Close/Vent. function		*/
/*											*/
/*------------------------------------------*/
void	_FuncReg( void )
{
	_DupliFuncClear() ;								// Duplicate key function clear
	if	( mb_NoPush )								// No push before ?
	{												// Yes
		mb_NoPush = d_Clear ;
		m_KindOfKey = d_RegKey ;
		_ClearSpecialMultiKeyState() ;				// Special multi key status clear
		mb_RegSw = d_On ;
		m_TimerKey = d_Time3s ;						// Set 3sec key timer
		if	( !m_TimerKeyMonitor )					// 1st entry ?
		{	// Yes
		          if(FG_PWRON==0){
	                     FG_PWRON=1;
	                     PIN_POWER_CONTROL=1;
			     TB_5s=51;//TB_51s;//51;  //5.1秒
                          }	
			m_TimerKeyMonitor = d_Time10s ;			// Set 10sec key timer
			m_KeyCount = 2 ;
			return ;
		}
		if	( !--m_KeyCount )						// ID registration mode ?
		{
			_SetRegistrationMode( d_RegAppend ) ;
			m_KindOfKey = d_Idle ;
			mb_NoPushWait = d_On ;					// Set no push wait
		}
		return ;
	}
	
	/*		Reg.sw push		*/
	if	( mb_RegSw )								// Continue push ?
	{												// Yes
		if	( m_TimerKey )							// 3sec passed ?
		{
			if	( !--m_TimerKey )
			{										// Yes
				_SetRegistrationMode( d_RegDelete ) ;	// ID delete mode set
				m_KindOfKey = d_Idle ;
				mb_NoPushWait = d_On ;				// Set no push wait
			}
			return ;								// No
		}
	}  
}
void	_FuncStop( void )
{
	if	( _GetNoPushState() )						// No push before ?
	{												// No
		if	( mb_OpenSw || mb_StopSw || mb_CloseSw )// Continue push ?
		{											// Yes
			if	(( !m_TimerKey )&&(FG_BAT==0)&&(TIME_Once_twice_switch==0))  //2015.1.31修正4	  // 5sec passed ?
			{
//				if	( !--m_TimerKey )
//				{									// Yes
					if	( mb_OpenSw || mb_CloseSw )				// Close sw ?
					{
						m_KeyOptSetMode = 10 ;				// Yes
						//_ReqBuzzer( d_BuzOpt5 ) ;
						//_ReqBuzzer(103,103,100);
						m_KindOfKey = d_Idle ;
						mb_NoPushWait = d_On ;			// Set no push wait
						return ;
					}
					m_KeyOptSetMode = 1 ;			// Opetion setting mode
					//_ReqBuzzer( d_BuzOpt1 ) ;
					_ReqBuzzer(103,103,1);
						FG_LED_on=1;
						if(FG_PWRON==0){
	                                            FG_PWRON=1;
	                                            PIN_POWER_CONTROL=1;
	                                            TB_5s=TB_51s;//51;  //5.1秒
                                                }
					m_KeyDupliSetTimeout = d_DupliTime4s ;
					m_KindOfKey = d_Idle ;
					mb_NoPush=d_Off;
					mb_NoPushWait = d_On ;			// Set no push wait
					m_KeyOptSetOpenStop = 1 ;
					if	( mb_OpenSw )				// Open ?
					{
						m_KeyOptSetOpenStop = 0 ;	// Yes
					}
//				}
//				return ;							// No
			}
			return ;
		}
		return ;
	}
	
	m_KindOfKey = m_KeyNo ;
	if	( m_KeyOptSetMode ||m_RegMode)							// Option setting mode ?
	{
		return ;									// Yes
	}
	
	switch	( m_KindOfKey )
	{
		case d_OpenKey :
			mb_OpenSw = d_On ;
			m_TimerKey = d_Time5s ;					// Set 5sec key timer
			break;
			
		case d_StopKey :
			mb_StopSw = d_On ;
			m_TimerKey = d_Time5s ;					// Set 5sec key timer
			break;
			
		case d_CloseKey :
			mb_CloseSw = d_On ;
			m_TimerKey = d_Time5s ;//d_Time9s ;					// Set 5sec key timer
			break;
	}
        
          /********2015.1.31追加  按一次模式********/
	if	( !rom_KeyOpt || m_KindOfKey == d_VentKey  )// Single push option or Vent. key ?
        
//          /********2015.1.31追加  按2次模式********/
//	if	( rom_KeyOpt || m_KindOfKey == d_VentKey  )// Single push option or Vent. key ?        
	{												// Yes
		_DupliFuncClear() ;							// Duplicate key function clear
		_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
		m_TimerKeyMonitor = d_Clear ;
		return ;
	}
	
	/*	Multi push option	*/
	if	( m_KeyOptSetMode || m_RegMode )
	{
		return ;
	}
	
	if	( m_KindOfKey == d_StopKey )
	{
		_DupliFuncClear() ;							// Duplicate key function clear
		_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
		m_TimerKeyMonitor = d_Clear ;
		FG_d_StopKey=1;
		m_KeyDupli1stTimer = d_D1stTime3s ;
		time_led=0;
		return ;
	}
	
	switch	( m_KindOfKey )
	{
//		case d_OpenKey :
//			m_KeyCloseCount = 2 ;
//			if	( !--m_KeyOpenCount )
//			{
//				_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
//				m_TimerKeyMonitor = d_Clear ;
//				_DupliFuncClear() ;					// Duplicate key function clear
//				return ;
//			}
//			m_KeyDupli1stTimer = d_D1stTime3s ;
//			break;
//			
//		case d_CloseKey :
//			m_KeyOpenCount = 2 ;
//			if	( !--m_KeyCloseCount )
//			{
//				_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
//				m_TimerKeyMonitor = d_Clear ;
//				_DupliFuncClear() ;					// Duplicate key function clear
//				return ;
//			}
//			m_KeyDupli1stTimer = d_D1stTime3s ;
//			break;
	  
		case d_OpenKey :
		case d_CloseKey :  
			if	( FG_d_StopKey && m_KeyDupli1stTimer)
			{
			        FG_d_StopKey=0;
				m_KeyDupli1stTimer=0;
				_ReqTxdEdit( m_KeyNo,m_KeyNo ) ;
				m_TimerKeyMonitor = d_Clear ;
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			break;
					
	}  
}

void	_FuncAutoTxStart( void )
{
  
}
void	_FuncAutoTxStop( void )
{
  
}
void	_FuncNoPush( void )
{
	mb_OpenSw = d_Off ;							// For duplicate keyvfunction
	mb_StopSw = d_Off ;
	mb_CloseSw = d_Off ;
	FG_BAT=0;
	
	_ClearSpecialMultiKeyState() ;				// Special multi key status clear
	mb_NoPush     = d_On ;						// No push on
	mb_NoPushWait = d_Off ;						// No push wait clear
	m_KindOfKey = d_KeyNoPush;//d_NoPushKey ;
//	if	( m_BuzzerMode == d_BuzBattLow )			// Batt.Low sound out ?
//	{
//		_BuzIdle() ;
//	}  
}
void	_SetKeyChatterCount( void )
{
	m_ChatterCount = d_Time50ms ;						// Chatter counter set
	m_TimerKey = 0 ;
}
/*==============================================*/
/*												*/
/*		Clear special multi key push status		*/
/*												*/
/*			 in  : none							*/
/*			 out : none							*/
/*												*/
/*==============================================*/
//
void	_ClearSpecialMultiKeyState( void )
{
	mb_RegSw      = d_Clear ;
	mb_RegStopSw  = d_Clear ;
	mb_RegOpenSw  = d_Clear ;
	mb_RegCloseSw = d_Clear ;
	mb_RegVentSw  = d_Clear ;
}
/****************************************/
/*										*/
/*		Key duplicate function clear	*/
/*										*/
/*			  in  : none				*/
/*			  out : none				*/
/*										*/
/****************************************/
//
void	_DupliFuncClear( void )
{
	m_KeyOptSetMode = d_Idle ;
	m_KeyOpenCount = 2 ;
	m_KeyCloseCount = 2 ;
	//mb_OpenSw = d_Off ;
	//mb_StopSw = d_Off ;
	if	( m_KeyDupli1stTimer )
	{
		m_KeyDupli1stTimer = d_Clear ;
		//_LedOnOff( d_LedOff ) ;				// Led off
	}
}
/*==============================================*/
/*												*/
/*			No Push(Last time) check			*/
/*												*/
/*			 in  : none							*/
/*			 out : 0: No push					*/
/*				   1: any push					*/
/*												*/
/*==============================================*/
//
uchar	_GetNoPushState( void )
{
	if	( !mb_NoPush )								// No push before ?
	{												// No
		return(d_NG) ;
	}
	
	mb_NoPush = d_Clear ;
//	/*		Set Auto Tx inhibit		*/
//	mb_AutoTxInhibit = d_On ;				// Inhibit
//	mb_AutoTxOnOff   = d_Off ;				// Off
//	
	m_TimerKey = d_Clear ;
	_ClearSpecialMultiKeyState() ;					// Special multi key status clear
	return(d_OK) ;
}
void	_ReqTxdEdit( uchar txreq , uchar buzreq )  // Tx data edit request
{
  UINT8 time_key;
  if((TB_sum_5s<69)&&(FG_PWRON==1)&&(TB_5s<25)){      //计算剩余的时间，总共时间不能超过69，以25为间隔。
    time_key=25-TB_5s;
    TB_sum_5s=TB_sum_5s+time_key;
    if((69-TB_sum_5s)>=27)TB_5s=25;
    
  }
  if((TB_5s>=25)||(TIME_2s_RestTX==0)){   //2015.4.13修正
        if(FG_PWRON==0){
	FG_PWRON=1;
	PIN_POWER_CONTROL=1;
	TB_5s=TB_51s;//51;  //5.1秒
        }
  	switch	( txreq )    // Jumo to key function
	{
		case 1 :
		        Control_code=0x08;     //open
			break ;
		case 2 :	
		        Control_code=0x04;    //stop
			break ;
		case 3 :	
		        Control_code=0x02;    //close
			break ;
		case 4 :	
		        Control_code=0x01;    //vnet 换气
			break ;
		case 10 :	              //Stop + Reg
		        Control_code=0x14;
			break ;	
		case 5 :                      //Open + Stop    
		        Control_code=0x0C;
		        break ;	
		case 6 :                   //Open + Close
		       Control_code=0x0A;
		        break ;	
		case 7 :                   //Open + Vent
		       Control_code=0x09;
		        break ;	
		case 9 :                //Stop + Close
		       Control_code=0x06;
		        break ;	
		case 12 :              //Close + Vent.
		       Control_code=0x03;
		        break ;	
	}
  	switch	( buzreq )    // Jumo to key function
	{
		case 1 :
		       _ReqBuzzer(103,103,0);
//			BASE_TIME_BEEP_on=103;
//                        BASE_TIME_BEEP_off=103;
//			TIME_BEEP_freq=0;
			break ;
		case 2 :
		       _ReqBuzzer(103,103,1);
//			BASE_TIME_BEEP_on=103;
//                        BASE_TIME_BEEP_off=103;
//			TIME_BEEP_freq=1;
			break ;
		case 3 :	
		      _ReqBuzzer(103,103,2);
//			BASE_TIME_BEEP_on=103;
//                        BASE_TIME_BEEP_off=103;
//			TIME_BEEP_freq=2;
			break ;
		case 4 :	
		      _ReqBuzzer(500,1,0);
		      break ;
		case 20 :	
		       _ReqBuzzer(1000,1,0);
//			BASE_TIME_BEEP_on=1000;
//                        BASE_TIME_BEEP_off=1;
//			TIME_BEEP_freq=0;
			break ;
	}	
        dd_set_ADF7021_Power_on();
        dd_set_TX_mode();	
//	TIME_BEEP_on=BASE_TIME_BEEP_on;
//        TIME_BEEP_off=BASE_TIME_BEEP_off;
        SendTxData();
        TIME_2s_RestTX=25;       //2015.4.13修正        
  }
  else PIN_LED=0;
}

//
/************************************************/
/*												*/
/*		Key duplicate function setting mode		*/
/*				Every 10ms						*/
/*												*/
/*			  in  : none						*/
/*			  out : none						*/
/*												*/
/************************************************/
//
void	_DupliFuncSetMode( void )
{
  UINT8 m_KeyOpt;
	switch	( m_KeyOptSetMode )
	{
		case 1 :
			if	( !mb_NoPushWait )					// No push ?
			{
				m_KeyDupliSetTimeout = d_DupliTime1s ;	// Yes
				++m_KeyOptSetMode ;
				return ;
			}
			if	( !m_KeyDupliSetTimeout )			// Pushing 9s over ?
			{
				m_KeyOptSetMode = 10 ;				// Yes
				//_ReqBuzzer( d_BuzOpt5 ) ;
				//_ReqBuzzer(103,103,100);
				
			}
			break ;
			
		case 2 :
			if	( !m_KeyDupliSetTimeout )			// Wait 1s ok ?
			{										// Yes
				++m_KeyOptSetMode;
				m_KeyDupliSetTimeout = d_DupliTime3s ;
				//_ReqBuzzer( d_BuzOpt2 ) ;
				_ReqBuzzer(103,103,0);
				FG_LED_on=1;
			}
			
			if	( m_KindOfKey != d_Idle && m_KindOfKey != d_KeyNoPush )
			{
				_DupliFuncClear() ;					// Duplicate key function clear
			}
			break ;
			
		case 3 :									// Sw in wait
			if	( !m_KeyDupliSetTimeout )
			{
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			
			if	( m_KindOfKey == d_Idle || m_KindOfKey == d_KeyNoPush )
			{
				return ;
			}
			
			if	( m_KeyOptSetOpenStop )				// Setting by stop ?
			{										// Yes
				if	( m_KindOfKey == d_StopKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime1s ;
					++m_KeyOptSetMode ;
					//mb_NoPushWait = d_On ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			else
			{
				if	( m_KindOfKey == d_OpenKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime1s ;
					++m_KeyOptSetMode ;
					
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			break ;
			
		case 4 :
			if	( !m_KeyDupliSetTimeout )			// Wait 1s ok ?
			{										// Yes
				++m_KeyOptSetMode ;
				m_KeyDupliSetTimeout = d_DupliTime3s ;
				//_ReqBuzzer( d_BuzOpt2 ) ;
				_ReqBuzzer(103,103,0);
				FG_LED_on=1;
			}
//			
//			if	( m_KindOfKey != d_Idle && m_KindOfKey != d_KeyNoPush  )
//			{										// Error
//				_DupliFuncClear() ;					// Duplicate key function clear
//			}
			break ;
			
		case 5 :									// Sw in wait
			if	( !m_KeyDupliSetTimeout )
			{
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			
			if	( m_KindOfKey == d_Idle || m_KindOfKey == d_KeyNoPush )
			{
				return ;
			}
			
			if	( m_KeyOptSetOpenStop )				// Setting by stop ?
			{										// Yes
				if	( m_KindOfKey == d_StopKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime1s ;
					++m_KeyOptSetMode ;
					mb_NoPushWait = d_On ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			else
			{
				if	( m_KindOfKey == d_OpenKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime1s ;
					++m_KeyOptSetMode ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			break ;
			
		case 6 :
			if	( !m_KeyDupliSetTimeout )			// Wait 1s ok ?
			{										// Yes
				++m_KeyOptSetMode ;
				m_KeyDupliSetTimeout = d_DupliTime3s ;
				//_ReqBuzzer( d_BuzOpt3 ) ;
				_ReqBuzzer(103,103,2);
                                FG_LED_on=1;				
			}
			
//			if	( m_KindOfKey != d_Idle && m_KindOfKey != d_KeyNoPush )
//			{										// Error
//				_DupliFuncClear() ;					// Duplicate key function clear
//			}
			break ;
			
		case 7 :									// Sw in wait
			if	( !m_KeyDupliSetTimeout )
			{
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			
			if	( m_KindOfKey == d_Idle || m_KindOfKey == d_KeyNoPush )
			{
				return ;
			}
			
			if	( m_KeyOptSetOpenStop )				// Setting by stop ?
			{										// Yes
				if	( m_KindOfKey == d_StopKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime10s ;
					++m_KeyOptSetMode ;
					mb_NoPushWait = d_On ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			else
			{
				if	( m_KindOfKey == d_OpenKey )
				{
					m_KeyDupliSetTimeout = d_DupliTime10s ;
					++m_KeyOptSetMode ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			break ;
			
		case 8 :									// Sw in wait
		case 9 :									// Sw in wait
			if	( !m_KeyDupliSetTimeout )
			{
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			
			if	( m_KindOfKey == d_Idle || m_KindOfKey == d_KeyNoPush )
			{
				return ;
			}
			
			if	( m_KeyOptSetOpenStop )				// Setting by stop ?
			{										// Yes
				if	( m_KindOfKey == d_StopKey )
				{
					if	( m_KeyOptSetMode == 9 )
					{
						m_KeyOpt = rom_KeyOpt ;
						if	( m_KeyOpt )
						{
							m_KeyOpt = 0 ;
						}
						else
						{
							m_KeyOpt = 1 ;
						}
						rom_KeyOpt=m_KeyOpt;
						KeyOpt_EEPROM_write();
						//_FlashWriteID() ;				// Write data to flash rom
						
						//_ReqBuzzer( d_BuzOpt4 ) ;
						_ReqBuzzer(103,103,4);
                                                FG_LED_on=1;	
						FG_d_StopKey=0;
						TB_5s=TB_20s;//20;
						_DupliFuncClear() ;				// Duplicate key function clear
						return ;
					}
					++m_KeyOptSetMode ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			else
			{
				if	( m_KindOfKey == d_OpenKey )
				{
					if	( m_KeyOptSetMode == 9 )
					{
						m_KeyOpt = rom_KeyOpt ;
						if	( m_KeyOpt )
						{
							m_KeyOpt = 0 ;
						}
						else
						{
							m_KeyOpt = 1 ;
						}
						rom_KeyOpt=m_KeyOpt;
						KeyOpt_EEPROM_write();				// Write data to flash rom
						
						//_ReqBuzzer( d_BuzOpt4 ) ;
						_ReqBuzzer(103,103,4);
                                                FG_LED_on=1;
						FG_d_StopKey=0;
						_DupliFuncClear() ;				// Duplicate key function clear
						
						return ;
					}
					++m_KeyOptSetMode ;
				}
				else
				{									// Error
					_DupliFuncClear() ;				// Duplicate key function clear
					return ;
				}
			}
			break ;
			
		case 10 :
			if	( !mb_NoPushWait )					// No push ?
			{
				//_BuzIdle() ;
				_DupliFuncClear() ;					// Duplicate key function clear
				return ;
			}
			break ;
			
	}
}
/****************************************/
/*										*/
/*		Registration mode procedure		*/
/*										*/
/*			  in  : none				*/
/*			  out : none				*/
/*										*/
/****************************************/
//
void	_RegistrationMode( void )
{
	if	( !_GetRegMode() )							// Reg. mode Idle ?
	{												// Yes
		return ;
	}

        if(FG_PWRON==0){
	FG_PWRON=1;
	PIN_POWER_CONTROL=1;
        }	
	if(m_TimerRegMode){
	  TB_5s=51;//TB_51s;//51;  //5.1秒
          time_led++;
          if(time_led>=500){time_led=0;PIN_LED=!PIN_LED;}	  
	}
	else {
	        //_ReqBuzzer(500,250,3);
	        _ReqBuzzer(500,250,2);
		PIN_LED=0;
		m_RegMode = d_Idle ;
	}
	/*	Led control	*/
//	if	( mb_LedOnOff )								// Led on timing ?
//	{												// Yes
//		_LedOnOff( d_LedOn ) ;						// Led on
//	}
//	else
//	{
//		_LedOnOff( d_LedOff ) ;						// Led off
//	}
	
	switch	( m_KindOfKey )
	{
		case	d_OpenKey :
			if	( ++m_RegID[m_RegDigit] > '9' )		// No. up
			{
				m_RegID[m_RegDigit] = '0' ;
			}
			//_ReqBuzzer(d_BuzOpen) ;
			_ReqBuzzer(103,103,0);
			m_TimerRegMode = d_Time1min ;			// 1min. set (1s base)
			break ;
			
		case	d_StopKey :						// Next digit
			if	( ++m_RegDigit > 8 )
			{
				m_RegDigit= 8 ;
			}
/*		Add on 2007/5/28		*/
			else
			{
				//_ReqBuzzer(d_BuzStop) ;
			  _ReqBuzzer(103,103,1);
			}
/*********************************/
/*		Deleted on 2007/5/28		*/
//			_ReqBuzzer(d_BuzStop) ;
/*********************************/
			m_TimerRegMode = d_Time1min ;			// 1min. set (1s base)
			break ;
			
		case	d_CloseKey :
			_IdClear() ;							// ID clear
			//_ReqBuzzer(d_BuzReg) ;
			_ReqBuzzer(1000,1,0);
			m_TimerRegMode = d_Time1min ;			// 1min. set (1s base)
			break ;
			
		case	d_RegKey :						// Send ID
/*		Add on 2007/5/28		*/
			if	( m_RegDigit < 8 )
			{
				return ;
			}
/*********************************/

			//_SetRegModeIdle() ;
			//m_RegMode = d_Idle ;
			ID_data_add.IDL = (ulong)atol(m_RegID) ;
/*		Modified on 2007/5/28		*/
//			if	( m_RFID.ID > 16777215 )			// Over ?
			if	( ID_data_add.IDL > 16777214 )			// Over ?
/*********************************/
			{
				//_ReqBuzzer(d_BuzRegEnd) ;
			        _ReqBuzzer(100,100,3);		  
			}
			else
			{
				_ReqTxdEdit(20,20) ;
			      
			}
			m_RegMode = d_Idle ;
			break ;
	}
}
//



UINT32 atol (unsigned char* m_RegID_x)
{
  UINT8 i,j;
  UINT32 m_ID=0;
    
  for(i=0;i<8;i++){
    j=m_RegID_x[i]-'0';
    m_ID=m_ID*10+j;
  }
  return(m_ID);
}


/************************************************/
/*												*/
/*				Set registration mode			*/
/*												*/
/*		in  : mode( 0:Idle/1:Append/2:Delete )	*/
/*		out : none								*/
/*												*/
/************************************************/
//
void	_SetRegistrationMode( uchar mode )
{
	if	( _GetRegMode() )							// Reg. mode Idle ?
	{												// No
		return ;
	}
	
	m_RegMode = mode ;
//	m_RFAddDelCode = 0x00 ;						// Set delete
//	if	( m_RegMode == d_RegAppend )			// Append ?
//	{											// Yes
//		m_RFAddDelCode = 0xFF ;					// Set Append
//	}
	
	_IdClear() ;								// ID clear
	
//	_LedOnOff( d_LedOn ) ;						// Led on
//	m_BlkTimer = d_Time500ms ;					// Blink start
//	mb_LedOnOff = d_On ;
//	
//	_ReqBuzzer(d_BuzReg) ;
	_ReqBuzzer(1000,1,0);
	m_TimerRegMode = d_Time1min ;				// 1min. set (1s base)
}
uchar	_GetRegMode( void )
{
	return(	m_RegMode ) ;
}
/********************************************/
/*											*/
/*					ID clear				*/
/*											*/
/********************************************/
//
void	_IdClear( void )
{
	register	uchar	i ;
	
	m_RegDigit = 0 ;							// Digit pointer clear
	//m_RFID.ID  = 0 ;							// ID Number clear
	for	( i=0; i<8; i++ )						// ID clear
	{
		m_RegID[i] = '0' ;
	}
	m_RegID[8] = 0 ;							// NULL set
	
}

//
/*--------------------------------------*/
/*					*/
/*			Sw data input	*/
/*					*/
/*			  in  : sw	*/
/*			  out : none	*/
/*					*/
/*--------------------------------------*/
//
void	_SwIn( uchar sw )
{
	m_KeyNew<<= 1 ;
	if	( sw )
	{
		m_KeyNew |= d_On ;
	}
}

void _ReqBuzzer(UINT16 BEEP_on_SET,UINT8 BEEP_off_SET,UINT8 BEEP_freq_SET)
{
  if(FG_BAT==0){
     BASE_TIME_BEEP_on=BEEP_on_SET;
     BASE_TIME_BEEP_off=BEEP_off_SET;
     TIME_BEEP_on=BASE_TIME_BEEP_on;
     TIME_BEEP_off=BASE_TIME_BEEP_off;
     TIME_BEEP_freq=BEEP_freq_SET;
  }
}

//void START_AD_SAMPLER(void){	// 启动AD转换（查询模式）
//        ADC1_CR3=0x0B;					// ADC输入引脚AIN7
//	ADC1_CR1 = 0x01;							// ADC开启
//	ADC1_SR_EOC=0;						// 清除EOC转换结束标志
//	ADC1_CR1 = 0x01;							// ADC开启
//	ADC1_CR1_START=1;
//}
//
//void ADC_read(void){
//	while(ADC1_SR_EOC == 0);				// 等待转换完成
//	ADC1_SR_EOC=0;
//	BAT_value = ((int)ADC1_DRH<<4);				// 先读高8位，默认设置数据左对齐
//	BAT_value |= ADC1_DRL;						// 再读低8位
//}














void test_mode_control(void)
{

 while(PIN_test_mode==0){  
  ClearWDT(); // Service the WDT 
  if((PIN_KEY_OPEN==0)&&(FG_KEY_OPEN==0)){
    FG_KEY_OPEN=1;
    dd_set_ADF7021_Power_on();
    dd_set_TX_mode();
    FG_test_mode=0;
    ADF7021_DATA_tx=0;
  }
  if(PIN_KEY_OPEN==1)FG_KEY_OPEN=0;
  
   if((PIN_KEY_STOP==0)&&(FG_KEY_STOP==0)){
    FG_KEY_STOP=1;
    //ADF7021_CE = 0;
    ADF7021_POWER=FG_NOT_allow_out;
    FG_test_mode=0;
    ADF7021_DATA_tx=0;
  }
  if(PIN_KEY_STOP==1)FG_KEY_STOP=0; 
  
  if((PIN_KEY_CLOSE==0)&&(FG_KEY_CLOSE==0)){
    FG_KEY_CLOSE=1;
    dd_set_ADF7021_Power_on();
    dd_set_TX_mode();
    FG_test_mode=1;
  }
  if(PIN_KEY_CLOSE==1)FG_KEY_CLOSE=0;  
  
	
  if((ADF7021_DATA_CLK==1)&&(FG_test_mode==1)&&(FG_test1==0)){
     ADF7021_DATA_tx=!ADF7021_DATA_tx;
     FG_test1=1;
  }
  if(ADF7021_DATA_CLK==0)FG_test1=0;	 
  
 
  PC_PRG();	       // PC控制  
 }  
  UART1_end();
  PIN_POWER_CONTROL=0;
  PIN_TX_LED=0;
  FG_KEY_OPEN=0;
  FG_KEY_STOP=0;
  FG_KEY_CLOSE=0;  
}
