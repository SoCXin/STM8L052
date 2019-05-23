/************************************************/
/*  FILE        :eeprom.h                       */
/*  DATE        :June, 2011                     */
/*  DESCRIPTION :routine for VHF2011            */
/*  CPU TYPE    :STM8S207C8                     */
/*  Programmer	:Gong Dong Sheng                */
/*  Mark        :0x4000~0x47FF                  */
/************************************************/
void InitialFlashReg(void);
void UnlockFlash(unsigned char Type);			// ����flash
void LockFlash(unsigned char Type );			// �����洢��

unsigned char ReadByteEEPROM( unsigned long Addr );				// ��ȡ1�ֽ�
void WriteByteToFLASH(unsigned long Addr, unsigned char Dat);	// д��һ�ֽ�
void EraseByteFLASH( unsigned int Addr);						// ����eeprom

void eeprom_save(void);
void eeprom_sys_load(void);



void KeyOpt_EEPROM_write(void);
/************************************************/

#define UNLOCK_FLASH_TYPE   ((unsigned char)0x00 )
#define UNLOCK_EEPROM_TYPE  ((unsigned char)0x01 )


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%                  EEPROM MAP                  %
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define addr_eeprom_sys			0x1000		// SYS=0x4000~0x400F




