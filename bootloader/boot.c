
#include"iostm8l151k4.h"


int main(void)
{ 
    FLASH_DUKR = 0xae;
    asm("nop");
    FLASH_DUKR = 0x56;                  // ���д����
    asm("nop");
    while(!(FLASH_IAPSR & 0x08));       // �ȴ�����
    asm("nop");
    FLASH_CR2 = 0x80;                   // ��ѡ���ֽڽ���д����
    asm("nop");
    *((unsigned char *)0x480b) = 0x55;
    asm("nop");
    *((unsigned char *)0x480c) = 0xaa;  // д��ѡ���ֽ�
    while(1);
}
