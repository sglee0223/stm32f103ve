#ifndef __LCD_BD663474_H__
#define __LCD_BD663474_H__

#include "stm32f1xx_hal.h"

/*
typedef enum
{		                           
  DC_CMD  = 0,	//command
	DC_DATA = 1		//data
} DCType;

//if IO for LCD is to be changed, just modify the constants below
#define LCD_X_SIZE		240	//LCD width
#define LCD_Y_SIZE		320	//LCD height
*/

#define  MAX_X  (240)
#define  MAX_Y  (320)  

//color define
#define     CYAN		 	 	 0x07FF
#define     PURPLE		 	 0xF81F
#define     RED					 0XF800
#define     GREEN        0X07E0
#define     BLUE         0X001F
#define     WHITE        0XFFFF
#define     BLACK        0X0000
#define     YELLOW       0XFFE0
#define     ORANGE       0XFC08
#define     GRAY  	     0X8430
#define     LGRAY        0XC618
#define     DARKGRAY     0X8410
#define     PORPO        0X801F
#define     PINK         0XF81F
#define     GRAYBLUE     0X5458
#define     LGRAYBLUE    0XA651
#define     DARKBLUE     0X01CF
#define 		LIGHTBLUE    0X7D7C

#define LCD_RST_H()		HAL_GPIO_WritePin(LCD_nRESET_GPIO_Port, LCD_nRESET_Pin, GPIO_PIN_SET)
#define LCD_RST_L()		HAL_GPIO_WritePin(LCD_nRESET_GPIO_Port, LCD_nRESET_Pin, GPIO_PIN_RESET)

#define LCD_RS_H()		HAL_GPIO_WritePin(LCD_nRS_GPIO_Port, LCD_nRS_Pin, GPIO_PIN_SET)
#define LCD_RS_L()		HAL_GPIO_WritePin(LCD_nRS_GPIO_Port, LCD_nRS_Pin, GPIO_PIN_RESET)

#define LCD_CS_H()		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_SET)
#define LCD_CS_L()		HAL_GPIO_WritePin(LCD_nCS_GPIO_Port, LCD_nCS_Pin, GPIO_PIN_RESET)

unsigned char SPI1_communication(unsigned char send_char);

//=============================================================================
//							LCD Basic Functions
//=============================================================================
void LCD_WRITE_REG(unsigned int index);
void LCD_WRITE_COMMAND(unsigned int index, unsigned int data);
void LCD_WRITE_DATA(unsigned int data);
void lcd_init(void);

//=============================================================================
//							LCD Application Functions
//=============================================================================
void lcd_clear_screen(unsigned int color_background);

void lcd_clear_area(unsigned int color_front, 
                    unsigned char x, 
                    unsigned int y, 
                    unsigned int width, 
                    unsigned height);
void lcd_clear_Rect(unsigned int color_front, 
                    unsigned int x0, 
                    unsigned int y0, 
                    unsigned int x1, 
                    unsigned int y1);
void lcd_set_cursor(unsigned int x, unsigned int y);

//unsigned char LCD_GetPoint(unsigned char x, unsigned int y);

void lcd_display_char(unsigned char ch_asc, 
                      unsigned int color_front, 
                      unsigned int color_background, 
                      unsigned char postion_x, 
                      unsigned char postion_y);

void lcd_display_string(unsigned char *str, 
                        unsigned int color_front, 
                        unsigned int color_background, 
                        unsigned char x, 
                        unsigned char y);

void lcd_display_GB2312(unsigned char gb, 
                        unsigned int color_front, 
                        unsigned int color_background, 
                        unsigned char postion_x, 
                        unsigned char postion_y);

void lcd_display_image(const unsigned char *img, 
                       unsigned char x, 
                       unsigned int y, 
                       unsigned int width, 
                       unsigned height);

void lcd_draw_dot(unsigned int x,
                  unsigned int y,
                  unsigned int color_front);

void lcd_draw_bigdot(unsigned int   color_front,
                     unsigned int    x,
                     unsigned int    y );

unsigned char lcd_draw_line(
                        unsigned int x0,
                        unsigned int y0,
                        unsigned int x1,
                        unsigned int y1,
												unsigned int color);
void lcd_display_number(unsigned int x,
                        unsigned int y,
                        unsigned long num,
                        unsigned char num_len);

void lcd_display_test(void);

unsigned char SPI1_Communication(unsigned char send_char);												
void LCD_Clear(uint16_t Color);	
//void LCD_SetBacklight(uint8_t intensity);
uint16_t LCD_GetPoint(uint16_t Xpos,uint16_t Ypos);
void LCD_SetPoint(uint16_t Xpos,uint16_t Ypos,uint16_t point);
void PutChar(uint16_t Xpos,uint16_t Ypos,uint8_t c,uint16_t charColor,uint16_t bkColor);
void LCD_DrawLine( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void PutChinese(uint16_t Xpos,uint16_t Ypos,uint8_t *str,uint16_t Color,uint16_t bkColor); 
void GUI_Text(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);
void GUI_Chinese(uint16_t Xpos, uint16_t Ypos, uint8_t *str,uint16_t Color, uint16_t bkColor);	
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);
void LCD_Drawcircle(int x0, int y0, int r, int color);
void DrawGrids(void);												
#endif
