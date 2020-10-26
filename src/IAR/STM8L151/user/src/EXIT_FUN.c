#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义



void SetTxData(UINT8 count_set ,uni_rom_id ID_data_set,UINT8 Control_code_set);
UINT16 SetFixedLengthCode(UINT8 data );



void EXIT_init(void){
   //EXTI_CR1=0X02;             //PORT A 的中断触发位
   EXTI_CR2=0x02;             //PORT B4 的中断触发位
   ADF7021_DATA_CLK_CR2=1;     //使能该I/O口中断  PA1
//   EXTI_CR2=0X00;   
//   PIN_PD7_CR2=1;      
}

void EXTI_PORTA1(void){
 // if(PIN_KEY_LOGIN==0)ADF7021_DATA_tx=!ADF7021_DATA_tx;
  if(FLAG_APP_TX==1){
    if(txphase%8==0)ID_INT_CODE=m_RFNormalBuf[txphase/8];
    if	(ID_INT_CODE & 0x80)ADF7021_DATA_tx=1;
    else ADF7021_DATA_tx=0;
    ID_INT_CODE<<=1;
    txphase++;
     //if(txphase>=224){
        if(txphase>=txphase_end){
        txphase=0;
        txphase_Repeat++;
        //if(txphase_Repeat>=3){FLAG_APP_TX=0;PIN_TX_LED=0;ADF7021_CE=0;ADF7021_POWER=1;}
	if(txphase_Repeat>=3){FLAG_APP_TX=0;PIN_TX_LED=0;ADF7021_POWER=FG_NOT_allow_out;ADF7021_DATA_tx=0;}
    }    
  }
  EXTI_SR1_bit.P4F=1;
  //EXTI_SR2_bit.PBF=1;
}


//void SendTxData(void)
//{ 
//  UINT8 i;
//       m_RFNormalBuf[0]=0xFF;
//       m_RFNormalBuf[1]=0xFF;
//       for(i=2;i<=14;i++)m_RFNormalBuf[i]=0x55;
//       PIN_TX_LED=1;
//       SetTxData();
//       txphase=0;
//       txphase_Repeat=0;
//       ID_INT_CODE=0;
//       FLAG_APP_TX=1;
//}
//
//
//void SetTxData(void)
//{
//    uni_i unii,unij,unik;
//  	/*	ID set	*/
//        m_RFNormalBuf[15]=0x15;
//	unii.ui = SetFixedLengthCode(ID_data.IDB[3]) ;
//	m_RFNormalBuf[16] = unii.uc[0] ;
//	m_RFNormalBuf[17] = unii.uc[1] ;
//	unii.ui = SetFixedLengthCode(ID_data.IDB[2]) ;
//	m_RFNormalBuf[18] = unii.uc[0] ;
//	m_RFNormalBuf[19] = unii.uc[1] ;
//	unii.ui = SetFixedLengthCode(ID_data.IDB[1]) ;
//	m_RFNormalBuf[20] = unii.uc[0] ;
//	m_RFNormalBuf[21] = unii.uc[1] ;
//	/*	Control code set	*/
//	unii.ui = SetFixedLengthCode(Control_code) ;
//	m_RFNormalBuf[22] = unii.uc[0] ;
//	m_RFNormalBuf[23] = unii.uc[1] ;
//	/*	SUM set	*/
//	unii.uc[1] = ID_data.IDB[1] ;
//	unii.uc[0] = Control_code;
//	unij.uc[1] = ID_data.IDB[3] ;
//	unij.uc[0] = ID_data.IDB[2] ;
//	unik.ui = unii.ui + unij.ui ;
//	unii.ui = SetFixedLengthCode(unik.uc[1]) ;
//	m_RFNormalBuf[24] = unii.uc[0] ;
//	m_RFNormalBuf[25] = unii.uc[1] ;
//	unii.ui = SetFixedLengthCode(unik.uc[0]) ;
//	m_RFNormalBuf[26] = unii.uc[0] ;
//	m_RFNormalBuf[27] = unii.uc[1] ;    
//}


//void SendTxData(void)
//{
//  UINT8 i;
//       m_RFNormalBuf[0]=0xFF;
//       m_RFNormalBuf[1]=0xFF;
//       for(i=2;i<=14;i++)m_RFNormalBuf[i]=0x55;
//       m_RFNormalBuf[15]=0x15;
//       PIN_TX_LED=1;
//       if(m_RegMode==0){
//	 txphase_end=224;
//	 SetTxData(16,ID_data,Control_code);
//       }
//       else {
//	 txphase_end=320;
//	 SetTxData(16,ID_data,0x80);
//	 if(m_RegMode==1)SetTxData(28,ID_data_add,0xFF);    //"1"是追加
//	 else SetTxData(28,ID_data_add,0);    //"2"是抹消
//       }
//       txphase=0;
//       txphase_Repeat=0;
//       ID_INT_CODE=0;
//       FLAG_APP_TX=1;
//}


void SendTxData(void)
{
  UINT8 i;
       m_RFNormalBuf[0]=0xFF;
       for(i=1;i<=13;i++)m_RFNormalBuf[i]=0x55;
       m_RFNormalBuf[14]=0x15;
       PIN_TX_LED=1;
       if(m_RegMode==0){
	 txphase_end=224;
	 SetTxData(15,ID_data,Control_code);
         m_RFNormalBuf[27]=0xFF;
       }
       else {
	 txphase_end=320;
	 SetTxData(15,ID_data,0x80);
	 if(m_RegMode==1)SetTxData(27,ID_data_add,0xFF);    //"1"是追加
	 else SetTxData(27,ID_data_add,0);    //"2"是抹消
         m_RFNormalBuf[39]=0xFF;
       }
       txphase=0;
       txphase_Repeat=0;
       ID_INT_CODE=0;
       FLAG_APP_TX=1;
}

void SetTxData(UINT8 count_set ,uni_rom_id ID_data_set,UINT8 Control_code_set)
{
    uni_i unii,unij,unik;
  	/*	ID set	*/
	unii.ui = SetFixedLengthCode(ID_data_set.IDB[3]) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;
	unii.ui = SetFixedLengthCode(ID_data_set.IDB[2]) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;
	unii.ui = SetFixedLengthCode(ID_data_set.IDB[1]) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;
	/*	Control code set	*/
	unii.ui = SetFixedLengthCode(Control_code_set) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;
	/*	SUM set	*/
	unii.uc[1] = ID_data_set.IDB[1] ;
	unii.uc[0] = Control_code_set;
	unij.uc[1] = ID_data_set.IDB[3] ;
	unij.uc[0] = ID_data_set.IDB[2] ;
	unik.ui = unii.ui + unij.ui ;
	unii.ui = SetFixedLengthCode(unik.uc[1]) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;
	unii.ui = SetFixedLengthCode(unik.uc[0]) ;
	m_RFNormalBuf[count_set++] = unii.uc[0] ;
	m_RFNormalBuf[count_set++] = unii.uc[1] ;    
}

UINT16 SetFixedLengthCode(UINT8 data )
{
	UINT16	code ;
	UINT8	i ;

	for	(i=0; i<8; i++)
	{
		code <<=2 ;
		if	( data & 0x01)		// '1' ?
		{					// Yes
			code |= 0x0002 ;
		}
		else
		{
			code |= 0x0001 ;      // '0'

		}
		data >>= 1 ;
	}
	return(code) ;
}