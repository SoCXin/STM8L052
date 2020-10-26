/***********************************************************************/
/*  FILE        :ram.h                                                 */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8S003f3     Crystal: 4M HSI                        */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/

extern volatile union{
	unsigned char BYTE;	
	struct { 
		unsigned char	Bit0:	1;
		unsigned char	Bit1:	1;
		unsigned char	Bit2:	1;
		unsigned char	Bit3:	1;
		unsigned char	Bit4:	1;
		unsigned char	Bit5:	1;
		unsigned char	Bit6:	1;
		unsigned char	Bit7:	1;
	}BIT; 	
}RAM_OP1;
	//************************************************
	#define 	FLAG_BYTE0		RAM_OP1.BYTE	 
	//------------------------------------------------
	#define		FLAG_APP_TX		RAM_OP1.BIT.Bit0	// 
	#define 	FG_KEY_OPEN		RAM_OP1.BIT.Bit1	//
	#define 	FG_KEY_STOP		RAM_OP1.BIT.Bit2	//
	#define 	FG_KEY_CLOSE		RAM_OP1.BIT.Bit3	//
	#define 	FG_KEY_LOGIN		RAM_OP1.BIT.Bit4	//
	#define 	FG_PWRON		RAM_OP1.BIT.Bit5	//
	#define 	FG_1ms		        RAM_OP1.BIT.Bit6	//
	#define 	FG_100ms		RAM_OP1.BIT.Bit7	//
	//************************************************


extern volatile union{
	unsigned char BYTE;	
	struct { 
		unsigned char	Bit0:	1;
		unsigned char	Bit1:	1;
		unsigned char	Bit2:	1;
		unsigned char	Bit3:	1;
		unsigned char	Bit4:	1;
		unsigned char	Bit5:	1;
		unsigned char	Bit6:	1;
		unsigned char	Bit7:	1;
	}BIT; 	
}RAM_OP2;
	//************************************************
	#define 	FLAG_BYTE1		RAM_OP2.BYTE	 
	//------------------------------------------------
	#define		FLAG_beep		RAM_OP2.BIT.Bit0	// 
	#define 	FG_beep_on		RAM_OP2.BIT.Bit1	//
	#define 	FG_beep_off		RAM_OP2.BIT.Bit2	//
	#define 	FG_LED_on		RAM_OP2.BIT.Bit3	//
	#define 	mb_AutoTxOnOff		RAM_OP2.BIT.Bit4	//
	#define 	mb_AutoTxInhibit	RAM_OP2.BIT.Bit5	//
	#define 	FG_test1		RAM_OP2.BIT.Bit6	//
	#define 	FG_test_mode		RAM_OP2.BIT.Bit7	//
	//************************************************

extern volatile union{
	unsigned char BYTE;	
	struct { 
		unsigned char	Bit0:	1;
		unsigned char	Bit1:	1;
		unsigned char	Bit2:	1;
		unsigned char	Bit3:	1;
		unsigned char	Bit4:	1;
		unsigned char	Bit5:	1;
		unsigned char	Bit6:	1;
		unsigned char	Bit7:	1;
	}BIT; 	
}RAM_RegSW;
	//************************************************
	#define 	FLAG_BYTE_RegSW		RAM_RegSW.BYTE	 
	//------------------------------------------------
	#define		mb_RegSw		RAM_RegSW.BIT.Bit0	// 
	#define 	mb_RegStopSw		RAM_RegSW.BIT.Bit1	//
	#define 	mb_RegOpenSw		RAM_RegSW.BIT.Bit2	//
	#define 	mb_RegCloseSw		RAM_RegSW.BIT.Bit3	//
	#define 	mb_RegVentSw		RAM_RegSW.BIT.Bit4	//
	#define 	m_KeyOptSetOpenStop	RAM_RegSW.BIT.Bit5	//
	#define 	mb_NoPush		RAM_RegSW.BIT.Bit6	//
	#define 	mb_NoPushWait		RAM_RegSW.BIT.Bit7	//
	//************************************************

extern volatile union{
	unsigned char BYTE;	
	struct { 
		unsigned char	Bit0:	1;
		unsigned char	Bit1:	1;
		unsigned char	Bit2:	1;
		unsigned char	Bit3:	1;
		unsigned char	Bit4:	1;
		unsigned char	Bit5:	1;
		unsigned char	Bit6:	1;
		unsigned char	Bit7:	1;
	}BIT; 	
}RAM_SW;
	//************************************************
	#define 	FLAG_BYTE_SW		RAM_SW.BYTE	 
	//------------------------------------------------
	#define		mb_OpenSw		RAM_SW.BIT.Bit0	// 
	#define 	mb_StopSw		RAM_SW.BIT.Bit1	//
	#define 	mb_CloseSw		RAM_SW.BIT.Bit2	//
	#define 	FG_d_StopKey		RAM_SW.BIT.Bit3	//
	#define 	BIT_SIO		        RAM_SW.BIT.Bit4	//
	#define 	FG_10s        		RAM_SW.BIT.Bit5	// 2015.1.31修正3
	#define 	FG_BAT		        RAM_SW.BIT.Bit6	//
	#define 	FG_Complex_Single_shot	RAM_SW.BIT.Bit7	//2015.1.31修正2
	//************************************************


#define BASE_100ms	 100
#define BASE_976us	 3

extern UINT8 TB_51s;
extern UINT8 TB_sum_5s;
//#define TB_50s	 68//50
//#define TB_51s	 69//51
#define TB_60s	 78//60
#define TB_20s	 20


//extern UINT8  m_RFNormalBuf[35];
extern UINT8  m_RFNormalBuf[40];
extern uni_rom_id ID_data;
extern uni_rom_id ID_data_add;
extern UINT8 Control_code;
extern UINT16 txphase;
extern UINT8 txphase_Repeat;
extern UINT8 ID_INT_CODE;
extern UINT16 txphase_end;

extern UINT16 TIME_BEEP_on;
extern UINT8 TIME_BEEP_off;
extern UINT16 BASE_TIME_BEEP_on;
extern UINT8 BASE_TIME_BEEP_off;
extern UINT8 TIME_BEEP_freq;

extern UINT8 TB_976us;
extern UINT8 TB_100ms;
extern UINT8 TB_5s;


extern UINT8 SIO_cnt;
extern UINT8 SIO_buff[16];
extern UINT8 SIO_DATA[16];
extern ADF70XX_REG_T ROM_adf7012_value[4];
extern const ADF70XX_REG_T Default_adf7012_value[4];
//extern UINT16 BAT_value;




#define d_Idle	 0//0x8A
#define d_Clear	 0
#define d_On	 1
#define d_Off	 0
#define d_OK	 0
#define d_NG	 1
#define d_RegAppend	 1
#define d_RegDelete	 2
#define d_KeyNoPush  0xFF
#define d_Time50ms  50
#define d_DupliTime4s  4000
#define d_DupliTime1s  1000
#define d_DupliTime3s  3000
#define d_DupliTime10s 10000
#define d_Time5s 5000
#define d_Time9s 9000
#define d_D1stTime3s 3000
#define d_Time3s  3000
#define d_Time10s 10000
#define d_Time1min 60000
/*		kind of key		*/
#define		d_IdleKey		0					// Idle(N/A)
#define		d_OpenKey		1
#define		d_StopKey		2
#define		d_CloseKey		3
#define		d_VentKey		4
#define		d_RegKey		5
#define		d_ReqStopReg            10

#define		d_ReqIdle		0
#define		d_ReqOpen		1
#define		d_ReqStop		2
#define		d_ReqClose		3
#define		d_ReqVent		4
#define		d_ReqOpenStop	5
#define		d_ReqOpenClose	6
#define		d_ReqOpenVent	7
#define		d_ReqOpenReg	8
#define		d_ReqStopClose	9
#define		d_ReqStopReg 	10
#define		d_ReqCloseReg	11
#define		d_ReqCloseVent	12
#define		d_ReqVentReg    13
#define		d_ReqAutoTxStart 14
#define		d_ReqAutoTxStop	15



#define _LedOnOff( d_LedOff )   PIN_LED=0;
extern UINT8 m_KeyNew;
extern UINT8 m_KindOfKey;
extern UINT8 m_KeyOld;
extern UINT16 m_ChatterCount;
extern UINT16 m_TimerKey;
extern UINT8 m_KeyNo;
extern UINT8 m_KeyOptSetMode;
extern UINT8 m_KeyOpenCount;
extern UINT8 m_KeyCloseCount;
extern UINT16 m_KeyDupli1stTimer;
extern UINT8 m_KeyOptSetMode;
extern UINT16 m_KeyDupliSetTimeout;
extern UINT8 rom_KeyOpt;
extern UINT16 time_led;
extern UINT16 m_TimerKeyMonitor;
extern UINT8 m_KeyCount;
extern UINT8 m_RegMode;
extern UINT8 m_RegID[9];
extern UINT8 m_RegDigit;
extern UINT16 m_TimerRegMode;

extern UINT8 AD_DATA_BUFF[2];
extern UINT32 RAM_BAT_SUM;
extern UINT16 RAM_BAT_AVG;
extern UINT8 RAM_BAT_CNT;
extern UINT8 BAT_out;
extern UINT32 BAT_Voltage_value;

extern UINT16 TIME_Once_twice_switch;   //2015.1.31修正4
extern UINT16 TIME_10s;   //2015.1.31修正3
extern UINT16 key_Value;   //2015.1.31修正3

extern UINT8 TIME_2s_RestTX;  //2015.4.13修正
extern UINT16 TIME_power_on_AD;


