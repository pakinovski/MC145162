#include "LPC17xx.h"


#define STB 19
#define OE 17
#define ROWA 6
#define ROWB 5
#define ROWC 8
#define ROWD 7


/* GPIOs Definition */
/*
#define LPC_GPIO_BASE         (0x2009C000UL)												 
#define LPC_GPIO0_BASE        (LPC_GPIO_BASE + 0x00000)
#define LPC_GPIO1_BASE        (LPC_GPIO_BASE + 0x00020)
#define LPC_GPIO2_BASE        (LPC_GPIO_BASE + 0x00040)
#define LPC_GPIO3_BASE        (LPC_GPIO_BASE + 0x00060)
#define LPC_GPIO4_BASE        (LPC_GPIO_BASE + 0x00080)
	   
#define LPC_GPIO0             ((LPC_GPIO_TypeDef      *) LPC_GPIO0_BASE    )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef      *) LPC_GPIO1_BASE    )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef      *) LPC_GPIO2_BASE    )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef      *) LPC_GPIO3_BASE    )
#define LPC_GPIO4             ((LPC_GPIO_TypeDef      *) LPC_GPIO4_BASE    )
#define LPC_WDT               ((LPC_WDT_TypeDef       *) LPC_WDT_BASE      )
*/

/*------------------------------------------------------------------------------
  delays number of tick Systicks (happens every 1 ms)
 *------------------------------------------------------------------------------*/
/*extern volatile uint32_t msTicks;  
__INLINE static void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}			 */


void GPIO_Init() {
	LPC_SC->PCONP |= 0x00008000; // turn on GPIO
//	LPC_GPIO0->FIODIR = 0x000b8000;
	LPC_GPIO0->FIODIR = 0x000b82f0;
//	LPC_GPIO1->FIODIR = 0x00000000;
   	LPC_GPIO2->FIODIR = 0x000001ff;
}

void SPI_Init1(){
	
	LPC_SC->PCONP |= 0x00000100; // turn on SPI
	LPC_SC->PCLKSEL0 |= 0x00030000;  // select clock for SPI 1/8 CCLK
	LPC_PINCON->PINSEL0 |= 0xc0000000; // Pin function select SCK
	LPC_PINCON->PINSEL1 |= 0x00000030; // Pin function select MOSI

	LPC_SPI->SPCR |= 0x00000020;	// SPI Control Register
	LPC_SPI->SPCCR |=  0x00000010;	// SPI Clock Counter Register
}

void LED_Init()
{
	GPIO_Init();
	SPI_Init1();
}

uint32_t cl_bit(uint32_t status)
{
	uint32_t temp;

	if(status & 0x00000080)
	{
		temp = LPC_SPI->SPDR;
	}
	else
	{
		if(status & 0x00000010)
		{
			LPC_SPI->SPCR = LPC_SPI->SPCR;
		}
	
		if(status & 0x00000040)
		{
			temp = LPC_SPI->SPDR;
		}
	}

	return temp;
}

int32_t SPI_Send(uint8_t data){
	
	uint32_t status = 0;
	int32_t rtn = 0;
	LPC_SPI->SPDR =  data;

	while(1)
	{
		status = (LPC_SPI->SPSR & 0x000000f8);

		if(status)
		{
			if(status & 0x00000080)			//Check SPIF
			{	
				rtn = 0;
			}
			else
			{
			
				rtn = -1;
			}

			cl_bit(status);
			break;
		}
	}

	return rtn;				   	
}

void STB_Enable() {
	LPC_GPIO0->FIOPIN |= 1<<STB;
}

void STB_Disable() {
	LPC_GPIO0->FIOPIN &= ~(1<<STB);
}

void STB_Trans() {
	STB_Enable();
	STB_Disable();
}

void OE_Enable() {
   	LPC_GPIO0->FIOPIN |= 1<<OE;
}

void OE_Disable() {
  	LPC_GPIO0->FIOPIN &= ~(1<<OE);
}

void RowA(uint8_t data) {
	if(data == 0)
		LPC_GPIO2->FIOPIN &= ~(1<<ROWA);
	else
   		LPC_GPIO2->FIOPIN |= 1<<ROWA;
}

void RowB(uint8_t data) {
	if(data == 0)
		LPC_GPIO2->FIOPIN &= ~(1<<ROWB);
	else
   		LPC_GPIO2->FIOPIN |= 1<<ROWB;
}

void RowC(uint8_t data) {
	if(data == 0)
		LPC_GPIO2->FIOPIN &= ~(1<<ROWC);
	else
   		LPC_GPIO2->FIOPIN |= 1<<ROWC;
}

void RowD(uint8_t data) {
	if(data == 0)
		LPC_GPIO2->FIOPIN &= ~(1<<ROWD);
	else
   		LPC_GPIO2->FIOPIN |= 1<<ROWD;
}

void ChangeLine(uint8_t row){  //输出行线状态ABCD （A低,D高)

    RowA(row & 0x01);
    RowB((row & 0x02) >> 1);
   	RowC((row & 0x04) >> 2);
   	RowD((row & 0x08) >> 3);
}


