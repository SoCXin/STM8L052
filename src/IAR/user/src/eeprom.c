#include  <iostm8l051f3.h>				// CPU型号 
#include "Pin_define.h"		// 管脚定义
#include "initial.h"		// 初始化  预定义
#include "ram.h"		// RAM定义
#include "eeprom.h"		// eeprom

/***********************************************************************/
/*                    FLASH & EEPROM 寄存器及控制位                    */
/***********************************************************************/
#define FIRST_SECURITY_KEY        0xAE
#define SECOND_SECURITY_KEY       0x56 
#define ADD_EEPROM_S8             0x1000      

///* FLASH_CR2 */ 
//#define OPT               7   /* 对选项字节进行写操作 */ 
//#define WPRG              6   /* 字编程 */ 
//#define ERASE             5   /* 块擦除 */ 
//#define FPRG              4   /* 快速块编程 */  
////#define NC              3 
////#define NC              2 
////#define NC              1 
//#define PRG               0   /* 标准块编程 */ 
//
///* FLASH_NCR2 */ 
//#define NOPT              7   /* 对选项字节进行写操作 */ 
//#define NWPRG             6   /* 字编程 */ 
//#define NERASE            5   /* 块擦除 */ 
//#define NFPRG             4   /* 快速块编程 */ 
////#define NC              3 
////#define NC              2 
////#define NC              1 
//#define NPRG              0   /* 标准块编程 */ 
//
///* FLASH_FPR */ 
////#define NC              7 
////#define NC              6 
//#define WPB5              5   /* 用户启动代码保护位 */ 
//#define WPB4              4 
//#define WPB3              3
//#define WPB2              2
//#define WPB1              1
//#define WPB0              0 
//
///* FLASH_NFPR */  
////#define NC              7 
////#define NC              6 
//#define NWPB5             5   /* 用户启动代码保护位 */ 
//#define NWPB4             4
//#define NWPB3             3
//#define NWPB2             2
//#define NWPB1             1
//#define NWPB0             0 
//
///* FLASH_PUKR */ 
//#define MASS_PRG_KEY7     7   /* 主程序存储器解锁密钥 */ 
//#define MASS_PRG_KEY6     6
//#define MASS_PRG_KEY5     5
//#define MASS_PRG_KEY4     4
//#define MASS_PRG_KEY3     3
//#define MASS_PRG_KEY2     2
//#define MASS_PRG_KEY1     1
//#define MASS_PRG_KEY0     0
//
///* FLASH_DUKR */ 
//#define MASS_DATA_KEY7    7   /* DATA EEPROM解锁密钥 */
//#define MASS_DATA_KEY6    6
//#define MASS_DATA_KEY5    5
//#define MASS_DATA_KEY4    4
//#define MASS_DATA_KEY3    3
//#define MASS_DATA_KEY2    2
//#define MASS_DATA_KEY1    1
//#define MASS_DATA_KEY0    0
//
/* FLASH_IAPSR */ 
//#define NC              7 
#define HVOFF             6   /* 高压结束标志 */ 
//#define NC              5 
//#define NC              4 
#define DUL               3   /* DATA EEPROM区域解锁标志 */ 
#define EOP               2   /* 编程结束(写或擦除操作)标志 */ 
#define PUL               1   /* 快速程序存储器结束标志 */ 
#define WR_PG_DIS         0   /* 试图向被保护页进行写操作的标志 */ 

#define FLASH_CR1_RESET_VALUE   (( uchar )0x00 )
#define FLASH_CR2_RESET_VALUE   (( uchar )0x00 )
#define FLASH_NCR2_RESET_VALUE  (( uchar )0xFF )
#define FLASH_IAPSR_RESET_VALUE (( uchar )0x40 )
#define FLASH_PUKR_RESET_VALUE  (( uchar )0x00 )
#define FLASH_DUKR_RESET_VALUE  (( uchar )0x00 )



#define FLASH_PUKR_PUK          ((uchar)0xFF) /*!< Flash Program memory unprotection mask */
#define FLASH_DUKR_DUK          ((uchar)0xFF) /*!< Data EEPROM unprotection mask */

//#define UNLOCK_FLASH_TYPE       (( uchar )0x00 )
//#define UNLOCK_EEPROM_TYPE      (( uchar )0x01 )

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void InitialFlashReg( void ){					// 初始化闪存寄存器组
	FLASH_CR1 = FLASH_CR1_RESET_VALUE;
	FLASH_CR2 = FLASH_CR2_RESET_VALUE;
	//FLASH_NCR2 = FLASH_NCR2_RESET_VALUE;
	FLASH_IAPSR &= ( uchar )(~( 1 << DUL ));	// 清除只读DATA区解锁
	FLASH_IAPSR &= ( uchar )(~( 1 << PUL ));	// 清除程序区解锁
}
//------------------------------------------------
//  注: 2个密钥的操作序列正好相反  
void UnlockFlash(unsigned char Type){			// 解锁flash
	if( Type == UNLOCK_FLASH_TYPE){				// 解锁程序区
		FLASH_DUKR = SECOND_SECURITY_KEY;
		FLASH_DUKR = FIRST_SECURITY_KEY;  
	}
	else{										// 解锁eeprom
		FLASH_DUKR = FIRST_SECURITY_KEY;
		FLASH_DUKR = SECOND_SECURITY_KEY;
	}
}
//------------------------------------------------
void LockFlash(unsigned char Type ){			// 锁定存储器
	if( Type == UNLOCK_FLASH_TYPE ){
		FLASH_IAPSR &= ~( 1 << PUL );  
	}
	else{
		FLASH_IAPSR &= ~( 1 << DUL );
	}
}
//------------------------------------------------
uchar ReadByteEEPROM( ulong Addr ){				// 从eeprom中读取1字节
	return(*(( __far uchar* ) Addr ));			// Read byte
}
//------------------------------------------------
void WriteByteToFLASH(ulong Addr, uchar Dat){	// 写入一字节到eeprom
	*(( __far uchar * ) Addr ) = Dat;
}
//------------------------------------------------
void EraseByteFLASH( uint Addr){				// 擦除eeprom中内容
	*(( __near uchar * ) Addr) = 0x00; 
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void eeprom_save(void){
	unsigned char	i,eeprom_sys_buff;  
	
	UnlockFlash( UNLOCK_EEPROM_TYPE );
	for (i=0;i<16;i++)
		WriteByteToFLASH( addr_eeprom_sys+i, eeprom_sys_buff);
	LockFlash( UNLOCK_EEPROM_TYPE );
}  
void eeprom_sys_load(void){
	//unsigned char	i;  
	//for (i=0;i<0X1F;i++)
	//	eeprom_sys_buff[i] = ReadByteEEPROM( addr_eeprom_sys+i );
	//--------------------------------------
  
//    UINT16 i,j;
//    UINT8 xm[3]={0};
//    uni_rom_id xn;
//    for(i=0;i<256;i++)ID_Receiver_DATA[i]=0;//ID_Receiver_DATA[ID_DATA_PCS]=0;
//    xm[0] = ReadByteEEPROM( addr_eeprom_sys+0x3FE );
//    xm[1] = ReadByteEEPROM( addr_eeprom_sys+0x3FF );
//    ID_DATA_PCS=xm[0]*256+xm[1];
//    if(ID_DATA_PCS==0xFFFF)ID_DATA_PCS=0;
//    else if(ID_DATA_PCS>256)ID_DATA_PCS=256;
//    for(i=0;i<ID_DATA_PCS;i++){
//        j=3*i;
//        xm[0] = ReadByteEEPROM( addr_eeprom_sys+j);
//	j++;
//        xm[1] = ReadByteEEPROM( addr_eeprom_sys+j);
//	j++;
//        xm[2] = ReadByteEEPROM( addr_eeprom_sys+j);	
//        xn.IDB[0]=0;
//        xn.IDB[1]=xm[0];
//        xn.IDB[2]=xm[1];
//        xn.IDB[3]=xm[2];
//        ID_Receiver_DATA[i]=xn.IDL;
//    }
  
  uni_rom_id Init_ID_data;
  UINT8 i,j;
  
  rom_KeyOpt= ReadByteEEPROM( addr_eeprom_sys+0x70 );
  if(rom_KeyOpt>1)rom_KeyOpt=0;
  
  
  Init_ID_data.IDB[0] =0;  
  Init_ID_data.IDB[1] = ReadByteEEPROM( addr_eeprom_sys+0x10 );
  Init_ID_data.IDB[2] = ReadByteEEPROM( addr_eeprom_sys+0x11 );
  Init_ID_data.IDB[3] = ReadByteEEPROM( addr_eeprom_sys+0x12 );  
  if(Init_ID_data.IDL==0xFFFFFF)Init_ID_data.IDL=0;
  ID_data.IDL=Init_ID_data.IDL;
  //ID_data.IDL=0xFFFFFE;
  
  for (i=0;i<4;i++)
  {
    j=0x20+i*4;
    ROM_adf7012_value[i].byte[0]= ReadByteEEPROM( addr_eeprom_sys+j );
    ROM_adf7012_value[i].byte[1]= ReadByteEEPROM( addr_eeprom_sys+j+1 );
    ROM_adf7012_value[i].byte[2]= ReadByteEEPROM( addr_eeprom_sys+j+2 );
    ROM_adf7012_value[i].byte[3]= ReadByteEEPROM( addr_eeprom_sys+j+3 );
    if((ROM_adf7012_value[i].whole_reg==0)||(ROM_adf7012_value[i].whole_reg==0xFFFFFFFF))ROM_adf7012_value[i]=Default_adf7012_value[i];
  }
  
} 


void KeyOpt_EEPROM_write(void)
{
//    UINT8 xm[3]={0};
//    UINT16 m1;
//    uni_rom_id xn;
//     ID_DATA_PCS++;
//     xm[0]=ID_DATA_PCS%256;
//     xm[1]=ID_DATA_PCS/256;
//     
//     UnlockFlash( UNLOCK_EEPROM_TYPE );
//	WriteByteToFLASH( addr_eeprom_sys+0x3FE, xm[1]);
//	WriteByteToFLASH( addr_eeprom_sys+0x3FF, xm[0]);
//     LockFlash( UNLOCK_EEPROM_TYPE );
//	
//     ID_Receiver_DATA[ID_DATA_PCS-1]=ID_Receiver_Login;
//     xn.IDL=ID_Receiver_Login;
//     xm[0]=xn.IDB[1];
//     xm[1]=xn.IDB[2];
//     xm[2]=xn.IDB[3];
//     m1=(ID_DATA_PCS-1)*3;
//     UnlockFlash( UNLOCK_EEPROM_TYPE );
//	WriteByteToFLASH( addr_eeprom_sys+m1, xm[0]);
//	m1++;
//	WriteByteToFLASH( addr_eeprom_sys+m1, xm[1]);
//	m1++;
//	WriteByteToFLASH( addr_eeprom_sys+m1, xm[2]);
//     LockFlash( UNLOCK_EEPROM_TYPE );

  
     UnlockFlash( UNLOCK_EEPROM_TYPE );
	WriteByteToFLASH( addr_eeprom_sys+0x70, rom_KeyOpt);
     LockFlash( UNLOCK_EEPROM_TYPE );  
  
}
