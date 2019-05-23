/***********************************************************************/
/*  FILE        :Uart.H                                                */
/*  DATE        :Mar, 2014                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :STM8L151G6     Crystal: 16M HSI                       */
/*  DESCRIPTION :                                                      */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/

void UART1_INIT(void);
void Send_char(unsigned char ch);
void UART1_RX_RXNE(void);
void PC_PRG(void);
unsigned char hex_asc(unsigned char hex);
unsigned char asc_hex_2(unsigned char asc1,unsigned char asc0);
unsigned char asc_hex(unsigned char asc);
void UART1_end(void);