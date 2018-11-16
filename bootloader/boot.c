
#include"iostm8l151k4.h"


int main(void)
{ 
    FLASH_DUKR = 0xae;
    asm("nop");
    FLASH_DUKR = 0x56;                  // 解除写保护
    asm("nop");
    while(!(FLASH_IAPSR & 0x08));       // 等待解锁
    asm("nop");
    FLASH_CR2 = 0x80;                   // 对选项字节进行写操作
    asm("nop");
    *((unsigned char *)0x480b) = 0x55;
    asm("nop");
    *((unsigned char *)0x480c) = 0xaa;  // 写入选项字节
    while(1);
}
