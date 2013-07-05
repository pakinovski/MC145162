#ifndef _HARDWARE_H
#define _HARDWARE_H

#include "stdint.h"

void LED_Init(void);
int SPI_Send(uint8_t data);
void STB_Enable(void);
void STB_Disable(void);
void STB_Trans(void);
void OE_Enable(void);
void OE_Disable(void);
void ChangeLine(uint8_t row);


#endif

