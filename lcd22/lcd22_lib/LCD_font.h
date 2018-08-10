#ifndef __LCD_FONT_H__
#define __LCD_FONT_H__

extern const unsigned int color[];
extern const unsigned char ascii[];
extern const unsigned char GB2312[][32];

//#define  ASCII_8X16_MS_Gothic
#define  ASCII_8X16_System

void GetASCIICode(unsigned char* pBuffer,unsigned char ASCII);

#endif

