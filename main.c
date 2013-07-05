#include "lpc17xx_spi.h"
#include "lpc17xx_ssp.h"
#include "system_LPC17xx.h"
#include "serial.h"
#include "stdio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc_types.h"
#include "hardware.h"

#define P00 1<<0
#define P10 1<<10
#define P27 1<<27
#define P28 1<<28
#define P15 1<<15
#define P16 1<<16
#define P17 1<<17
#define P18 1<<18
#define P0DAT *(volatile unsigned int*)0x2009c014

void delay(int num)
{
	int i;
	for(i=0; i<num; i++);
}
void gpio_init()
{
	LPC_SC->PCONP |= 0x00008000; // turn on GPIO
//	LPC_GPIO0->FIODIR = 0x000b8000;																						 
	LPC_GPIO0->FIODIR |= P15 | P16 | P17 | P18 ;
//	LPC_GPIO1->FIODIR = 0x00000000;
//   	LPC_GPIO2->FIODIR = 0x000001ff;
}
			   
void enable_down()		  // p17
{
	LPC_GPIO0->FIOPIN &=  ~(P17);


}
void enable_up()	   //p17
{
	LPC_GPIO0->FIOPIN |= (P17) ;

}

void clk_up()
{

	LPC_GPIO0->FIOPIN |= (P18);
	delay(80);

}
void clk_down()
{
 	LPC_GPIO0->FIOPIN &= ~(P18);
	delay(60);
}
void p15_send_bit0()
{
	LPC_GPIO0->FIOPIN &=  ~(P15)   ;
	delay(60);
}
void p15_send_bit1()
{
	LPC_GPIO0->FIOPIN |= P15 ;
	delay(60);
}
void p16_send_bit0()
{
	LPC_GPIO0->FIOPIN &=  ~(P16)   ;
	delay(60);
}
void p16_send_bit1()
{
	LPC_GPIO0->FIOPIN |= P16 ;
	delay(60);
}
void send_8bit(unsigned int num)
{
	int i;
	int j;
	int bit;
 	enable_up();
	clk_down();
	for(i=0,j=7; i<8; i++,j--)
	{
		bit = (num >> j) & 0x1;
		if(bit)
		{
			p15_send_bit1();
		}
		else
		{
		 	p15_send_bit0();
		}
		clk_up();
		clk_down();
	}
	delay(10);
	enable_down();
}
void send_32bit(unsigned int num)
{
	int i;
	int j;
	int bit;
	enable_up();
	for(i=0,j=31; i<32; i++,j--)
	{
		bit = (num >> j) & 0x1;
		if(bit)
			p15_send_bit1();
		else
			p15_send_bit0();
			clk_up();
			clk_down();
	}
	delay(10);
	enable_down();
}

void send_counters(unsigned int txcounter, unsigned int rxcounter)
{
	int i;
	int j;
	int bit1;
	int bit2;
	enable_down();
	for(i=0,j=15; i<16; i++,j--)
	{
		bit1 = (txcounter >> j) & 0x1;
		bit2 = (rxcounter >> j) & 0x1;
		if(bit1)
			 p16_send_bit1();
		else
			 p16_send_bit0();
		if(bit2)
			 p15_send_bit1();
		else
			p15_send_bit0();
			clk_up();
			clk_down();
    }
	enable_up();
	enable_down();

}

int main()
{

	unsigned int iSelected;
	unsigned int icount;
	unsigned int irefe;
	unsigned int txcounter;
	unsigned int rxcounter;

	SystemInit(); /* System initialize */
	SER_init(2); /* Using UART2 as Debug OutPut*/
    gpio_init();
	while(1)
	{
		printf("*************************************\n" );
		printf("1. Control Register\n");
		printf("2. Auxiliary/Reference Frequency Counters\n");
		printf("3. Transmit and Receive Counters(4-pins)\n") ;
		printf("ÇëÊäÈëÐÞ¸Ä¼Ä´æÆ÷ÐòºÅ£º" );
		scanf("%d", &iSelected);
		printf("%d\n", iSelected);
		if(iSelected == 1)
		{	
			iSelected =0;
			icount = 0x80;
			printf("TestBit :");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<5;
			printf("Aux Data Select¡¡£º");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<4;
			printf("REFout 3/4¡¡£º");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<3;
			printf("TxPD Enable¡¡£º");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<2;
			printf("RxPD Enable: ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<1;
			printf("Ref PD Enable: ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			icount |= iSelected<<0;
			printf(" SendNum : 0x%x\n", icount);
			send_8bit(icount);
		}
		else if (iSelected == 2)
		{
			iSelected = 0;
			irefe = 0;
			printf("AUX REF ENABLE :");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<30;
			printf("TX-0 SELECT : ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<29;
			printf("RX-0 SELECT : ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<28;
			printf("12-BITS  REF FREQ DATA :¡¡");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<16;
			printf(" fR1 S1 : ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<15;
			printf("fR1 S2 : ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<14;
			printf("14-BITS AUX REF FREQ DATA : ");
			scanf("%d", &iSelected);
			printf("%d\n", iSelected);
			irefe |= iSelected<<0;
			printf("SenderNum : 0x%08x \n", irefe);
			send_32bit(irefe);
		}
		else  if(iSelected == 3)
		{
			printf("16¨CBIT Tx COUNTER DIVIDE RATIO : ");
			scanf("%d", &txcounter);
			printf("%d\n", txcounter);
			printf("16¨CBIT Rx COUNTER DIVIDE     RATIO : ");
			scanf("%d", &rxcounter);
			printf("%d\n", rxcounter);
			send_counters(txcounter, rxcounter);
	 	}	

		else
		{
			printf("ÇëÖØÐÂÊäÈë\n");
			continue;
		}
	}
}

