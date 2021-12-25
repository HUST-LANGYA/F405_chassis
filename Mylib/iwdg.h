#ifndef __IWDG_H
#define __IWDG_H

#include "main.h"
#define IWDG_Prescaler_4   0x00
#define IWDG_Prescaler_8   0x01                      
#define IWDG_Prescaler_16  0x02
#define IWDG_Prescaler_32  0x03
#define IWDG_Prescaler_64  0x04
#define IWDG_Prescaler_128 0x05
void IWDG_Config(uint8_t prv ,uint16_t rlv);
void IWDG_Feed(void);

#endif
