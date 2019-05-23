/***********************************************************************/
/*  FILE        :initial.h                                             */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8S003     Crystal: 4M HSI                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
/***********************ϵͳԤ����**********************/
#define	UINT8	    unsigned char
#define	INT8	    char
#define	UINT16	    unsigned int
#define	INT16	    int
#define	UINT32	    unsigned long
#define	INT32	    long

#define uchar       unsigned char 
#define uint        unsigned int 
#define ulong       unsigned long

#define _NOP()      asm( " nop " )
#define _EI()       asm( " rim " )
#define _DI()       asm( " sim " )

// ������ƼĴ���1��1=�����0=���룩
#define Input				0
#define Output				1

#define FG_allow_out			1
#define FG_NOT_allow_out		0
/*******************ϵͳԤ����  end**********************/
//˵��  byte��0��----->���ֽ�
//      ������������������
//      byte��3��----->���ֽ�

  typedef union{
        UINT32  whole_reg;
        UINT8   byte[4];    // Warning: Be endian-specific when accessing bytes

  } ADF70XX_REG_T;

  typedef union {												// ID No.
	UINT32	IDL ;
	UINT8	IDB[4];
  }uni_rom_id;
  
  typedef union {
        UINT16	ui ;
	UINT8	uc[2] ;
  }uni_i;


void RAM_clean(void);		// ���RAM 
void VHF_GPIO_INIT(void);	// CPU�˿�����
void SysClock_Init( void ); 	// ϵͳʱ�ӣ��ⲿʱ�ӣ�
void beep_init( void );
void Delayus(unsigned char timer);
void _Init_RAM(void);
void WDT_init(void);
void ClearWDT(void);
