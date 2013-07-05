/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: MCB1700 Low level serial functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.H>                              /* LPC17xx definitions    */
#include "Serial.h"

#include "lpc17xx.h"                              /* LPC17xx definitions    */

#define FOSC                        12000000                            /*  振荡器频率                  */

#define FCCLK                      (FOSC  * 8)                          /*  主时钟频率<=100Mhz          */
                                                                        /*  FOSC的整数倍                */
#define FCCO                       (FCCLK * 3)                          /*  PLL频率(275Mhz~550Mhz)      */
                                                                        /*  与FCCLK相同，或是其的偶数倍 */
#define FPCLK                      (FCCLK / 4)                          /*  外设时钟频率,FCCLK的1/2、1/4*/
                                                                        /*  或与FCCLK相同               */

#define UART0_BPS     115200                                             /* 串口0通信波特率             */
#define UART2_BPS     115200                                             /* 串口2通信波特率             */
#define UART_BPS      115200 
/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_init (int uart) {
  LPC_UART_TypeDef *pUart;
  uint16_t usFdiv;
   
  if (uart == 0) {                               /* UART0 */
    LPC_PINCON->PINSEL0 |= (1 << 4);             /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);             /* Pin P0.3 used as RXD0 (Com0) */

	LPC_SC->PCONP = LPC_SC->PCONP|(1<<3);	      /*打开UART0电源控制位	           */

    pUart = (LPC_UART_TypeDef *)LPC_UART0;

  } else {		                                   /* UART2 */
   	LPC_PINCON->PINSEL0 |= (1 << 20);            /* Pin P0.10 used as TXD2 (Com2) */
    LPC_PINCON->PINSEL0 |= (1 << 22);            /* Pin P0.11 used as RXD2 (Com2) */

	LPC_SC->PCONP = LPC_SC->PCONP|(1<<24);	      /*打开UART2电源控制位	           */

    pUart = (LPC_UART_TypeDef *)LPC_UART2;
  }


  pUart->LCR  = 0x83;                       /* 允许设置波特率                */
  usFdiv = (FPCLK / 16) / UART_BPS;            /* 设置波特率                    */
  pUart->DLM  = usFdiv / 256;
  pUart->DLL  = usFdiv % 256; 
  pUart->LCR  = 0x03;                       /* 锁定波特率                    */
  pUart->FCR  = 0x06;

 // pUart->LCR    = 0x83;                          /* 8 bits, no Parity, 1 Stop bit  */
//  pUart->DLL    = 6;                             /* 115200 Baud Rate @ 18.0 MHZ PCLK */
//  pUart->FDR    = 0x85;                          /* FR = 1,627  DIVADDVAL = 5, MULVAL = 8 */
//  pUart->DLL    = 9;                             /* 115200 Baud Rate @ 25.0 MHZ PCLK */
 // pUart->FDR    = 0x21;                          /* FR 1,507, DIVADDVAL = 1, MULVAL = 2 */
  //pUart->DLM    = 0;                             /* High divisor latch = 0         */
  //pUart->LCR    = 0x03;                          /* DLAB = 0                       */

}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (int uart, int c) {
  LPC_UART_TypeDef *pUart;

  pUart = (uart == 0) ? (LPC_UART_TypeDef *)LPC_UART0 : (LPC_UART_TypeDef *)LPC_UART2;
  while (!(pUart->LSR & 0x20));
  return (pUart->THR = c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar (int uart) {
  LPC_UART_TypeDef *pUart;

  pUart = (uart == 0) ? (LPC_UART_TypeDef *)LPC_UART0 : (LPC_UART_TypeDef *)LPC_UART2;
  while (!(pUart->LSR & 0x01));
  return (pUart->RBR);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (non blocking read)
 *----------------------------------------------------------------------------*/
int SER_getChar_nb (int uart) {
  LPC_UART_TypeDef *pUart;

  pUart = (uart == 0) ? (LPC_UART_TypeDef *)LPC_UART0 : (LPC_UART_TypeDef *)LPC_UART2;
  if (pUart->LSR & 0x01)
    return (pUart->RBR);
  else
    return 0;
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_putString (int uart, unsigned char *s) {

  while (*s != 0) {
   SER_putChar(uart, *s++);
  }
}
