#ifndef __TTP229_H
#define __TTP229_H

#include "stm32f1xx_hal.h"

#include "stdbool.h"

uint8_t ReadKey8(void);
uint8_t GetKey8(void);
uint8_t ReadKeys8(void);
uint8_t GetKeys8(void);
uint8_t ReadKey16(void);
uint8_t GetKey16(void);
uint16_t ReadKeys16(void);
uint16_t GetKeys16(void);

void WaitForTouch(void);
void Key8(void);
void Keys8(void);
void Key16(void);
void Keys16(void);
bool IsTouch(void);
bool GetBit(void);

#endif
