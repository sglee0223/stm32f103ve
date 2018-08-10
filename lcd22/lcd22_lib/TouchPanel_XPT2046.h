#ifndef __TOUCHPANEL_H__
#define __TOUCHPANEL_H__

#include "stm32f1xx_hal.h"
#include "LCD_BD663474.h"

#define TOUCH_nCS_H()    HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_SET)
#define TOUCH_nCS_L()    HAL_GPIO_WritePin(TOUCH_nCS_GPIO_Port, TOUCH_nCS_Pin, GPIO_PIN_RESET)

#define TP_INT_IN       HAL_GPIO_ReadPin(TOUCH_nIRQ_GPIO_Port,TOUCH_nIRQ_Pin)

typedef	struct POINT 
{
   uint16_t x;
   uint16_t y;
}Coordinate;

typedef struct Matrix 
{						
	long double An,  
							Bn,     
							Cn,   
							Dn,    
							En,    
							Fn,     
							Divider ;
} Matrix ;

extern Coordinate ScreenSample[3];
extern Coordinate DisplaySample[3];
extern Matrix matrix ;
extern Coordinate  display ;

#define	CHX 	0x90
#define	CHY 	0xd0

void Touch_Init(void);
void TP_GetAdXY(int *x,int *y) ;
Coordinate *Read_Ads7846(void);
void TouchPanel_Calibrate(uint16_t charColor, uint16_t bkColor );
void DrawCross(uint16_t Xpos,uint16_t Ypos,int color);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr);
FunctionalState getDisplayPoint(Coordinate * displayPtr,Coordinate * screenPtr,Matrix * matrixPtr );
void TouchPanel_Calibrate2(void);
void Touch_Debug(void);
#endif 
