#include "gpio.h"

#include "ttp229.h"
#include "dwt_stm32_delay.h"

uint8_t _key8, _keys8, _key16;
uint16_t _keys16;

uint8_t ReadKey8(void)
{
	WaitForTouch();
	Key8();
	return _key8;
}
uint8_t GetKey8(void)
{
	if (IsTouch()) Key8();
	return _key8;
}
uint8_t ReadKeys8(void)
{
	WaitForTouch();
	Keys8();
	return _keys8;
}
uint8_t GetKeys8(void)
{
	if (IsTouch()) Keys8();
	return _keys8;
}
uint8_t ReadKey16(void)
{
	WaitForTouch();
	Key16();
	return _key16;
}
uint8_t GetKey16(void)
{
	if (IsTouch()) Key16();
	return _key16;
}
uint16_t ReadKeys16(void)
{
	WaitForTouch();
	Keys16();
	return _keys16;
}
uint16_t GetKeys16(void)
{
	if (IsTouch()) Keys16();
	return _keys16;
}

void Key8(void)
{
	_key8 = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (GetBit()) _key8 = i + 1;
	HAL_Delay(2); // Tout
}

void Keys8(void)
{
	_keys8 = 0;
	for (uint8_t i = 0; i < 8; i++)
		if (GetBit()) _keys8 |= 1 << i;
	HAL_Delay(2); // Tout
}

void Key16(void)
{
	_key16 = 0;
	for (uint8_t i = 0; i < 16; i++)
		if (GetBit()) _key16 = i + 1;
	HAL_Delay(2); // Tout
}

void Keys16(void)
{
	_keys16 = 0;
	for (uint8_t i = 0; i < 16; i++)
		if (GetBit()) _keys16 |= 1 << i;
	HAL_Delay(2); // Tout
}

bool GetBit(void)
{
	SetSCL(GPIO_PIN_RESET);
	DWT_Delay_us(2); // 500KHz
	bool retVal = !GetSDO();
	SetSCL(GPIO_PIN_SET);
	DWT_Delay_us(2); // 500KHz
	return retVal;
}

bool IsTouch(void)
{
	uint16_t timeout = 5000; // 50ms timeout
	while (GetSDO()) // DV LOW
	{
		if (--timeout == 0) return false;
		DWT_Delay_us(10);
	}
	while (!GetSDO()) // DV HIGH
	{
		if (--timeout == 0) return false;
		DWT_Delay_us(10);
	}
	DWT_Delay_us(10); // Tw
	return true;
}

void WaitForTouch(void)
{
	while (GetSDO()); // DV LOW
	while (!GetSDO()); // DV HIGH
	DWT_Delay_us(10); // Tw
}
