#include "string.h"			//strlen
#include "stdlib.h"			//_itoa
//#include "math.h"			//exp, etc
#include "stm8l051f3.h"		//consolidated definitions

//** these are global. watch out. **
//supposedly there's a 5 clock penalty for loading from the stack / instruction
//globals are placed in "section 0" in close ram, 8bit addresses = faster

// general counters
uint8_t ia;
uint8_t ib;
uint16_t i16;	//for timer
char buf[4];	//buffer for _itoa: max '255\0' = string 4 bytes long

uint8_t puts(const char *str) {								//ripped from stm8-examples-sdcc
	uint8_t i;
	for(i = 0; i < strlen(str); i++) {						//iterate through str
		while ( ValBit(USART1->SR,USART_SR_TXE) == 0 ) {}	//TXE flag: wait for DR availability
		USART1->DR = str[i];								//load char into data register
	}
	while ( ValBit(USART1->SR,USART_SR_TC) == 0 ) {}		//TC flag: wait for transmission completion
	return(i); 												//bytes sent
}

void PinInit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef pin, GPIO_Pin_TypeDef dir, GPIO_Pin_TypeDef a, 
		GPIO_Pin_TypeDef b, uint8_t VAL) {
								//0 | 1
	Bit(GPIOx->DDR,pin,dir);	//in|out
	Bit(GPIOx->CR1,pin,a);		//in:float|PU	out:OD|PP
	Bit(GPIOx->CR2,pin,b);		//in:!EXTI|EXTI out:slew|fast10MHz
	Bit(GPIOx->ODR,pin,VAL);	//0 | 1
}

void usartInit(void) {
	Bit(CLK->PCKENR1,CLK_PCKENR1_USART1,1);	//after global reset, CLK is reset. Turn on clock to periph
	Bit(USART1->CR1,USART_CR1_M,0);	//M bit: 0 = 8 bit words
	USART1->CR3 &= ~(0x30); //USART1->CR3 [5:4] = 00(default) -> 1 stop bit
	//sysclk = 16MHz HSI. 16000000/19200 baud = 833d = 0341h	(RM0031 pg532)
	//BRR1 = 0x34, BRR2 = 0x01
	USART1->BRR2 = 0x03;
	USART1->BRR1 = 0x68;	//updates when written to
	Bit(USART1->CR2,USART_CR2_TEN,1);	//TEN bit: 1 = transmit enabled
	//write to DR, TXE will auto clear.	(DR empty flag)
	//at end, wait for TC to set (transmittion complete flag)
}


void main(void) {
	CLK->CKDIVR = CLK_DIV1;				// 0x00: div by 1 prescaler = 16MHz
	
	PinInit(GPIOB,GPIO_Pin_0,GPIO_OUTPUT,GPIO_PP,GPIO_Slow,0);	//PB0 = LED: out, PP, slow, low
	PinInit(GPIOC,GPIO_Pin_5,GPIO_OUTPUT,GPIO_PP,GPIO_Slow,0);	//PC5 = TXout, PP, slow=clean, low

	usartInit();						//start uart for TX
	ia = 0;								//initialize the line counter

	while (1) {
		for (ib = 0; ib < 20; ib++) {			// hopefully ~1 sec
			for (i16 = 0; i16 < 50000; i16++){}	//delay 1,000,000 times
		}										//wild guess 16 cycles / loop @ 16MHz

		ChgBit(GPIOB->ODR,0);		//toggle LED @ PB0
		_itoa(ia,buf,10);			//convert u8 into string, base 10
		puts(buf);					//line number
		puts(": Hello World!\n");
		
		if ( ia < 255 ) { 			//max 255 for u8
			ia++; 					
		}
		else { ia = 0; }			//reset
	}											
}




