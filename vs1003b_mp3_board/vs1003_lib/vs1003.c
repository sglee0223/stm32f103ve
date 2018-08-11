/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               VS1003.c
** Descriptions:            The VS1003 application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2011-2-27
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "vs1003.h"
#include "spec_rew.h"
#include "beep.h"

/* Private variables ---------------------------------------------------------*/

/* VS1003 */ 
/* 0 , henh.1 , hfreq.2 , lenh.3 , lfreq 5 , */
uint8_t vs1003ram[5] = { 0 , 0 , 0 , 0 , 250 };

#if 0
/*******************************************************************************
*******************************************************************************/
static void delay_ms(uint32_t ms)
{
    uint32_t len;
    for (;ms > 0; ms --)
        for (len = 0; len < 100; len++ );
}
#endif

/*******************************************************************************
*******************************************************************************/ 
static void VS1003_SPI_SetSpeed( uint8_t SpeedSet)
{
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
	
  if( SpeedSet == SPI_SPEED_LOW )
  {
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  }
  else 
  {
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  }	

  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
}

/*******************************************************************************
*******************************************************************************/ 
static uint8_t VS1003_SPI_ReadByte(void)
{	
	HAL_StatusTypeDef res;
	uint8_t rxData[1] = {0, };

	res = HAL_SPI_Receive(&hspi2, rxData, sizeof(rxData), HAL_MAX_DELAY);
	if (res != HAL_OK)
		printf("HAL_SPI_Receive Error\r\n");

  return rxData[0];
}

/*******************************************************************************
*******************************************************************************/ 
static uint8_t VS1003_SPI_WriteByte( uint8_t TxData )
{	
	HAL_StatusTypeDef res;
	uint8_t cmd[1] = {0, };

	cmd[0] = TxData;
	res = HAL_SPI_Transmit(&hspi2, cmd, sizeof(cmd), HAL_MAX_DELAY);
	if (res != HAL_OK)
		printf("HAL_SPI_Transmit Error\r\n");

  return 0;
}

/*******************************************************************************
*******************************************************************************/ 
void VS1003_Init(void)
{		  
  MP3_Reset(0);
  HAL_Delay(100);
  MP3_Reset(1);

  MP3_DCS(1); 
  MP3_CCS(1); 
}

/*******************************************************************************
*******************************************************************************/ 
void VS1003_WriteReg( uint8_t reg, uint16_t value )
{  
	while(  MP3_DREQ ==0 );           /* */

	VS1003_SPI_SetSpeed( SPI_SPEED_LOW );	 
	MP3_DCS(1); 
	MP3_CCS(0); 
	VS1003_SPI_WriteByte(VS_WRITE_COMMAND); /*  VS1003 */
	VS1003_SPI_WriteByte(reg);             
	VS1003_SPI_WriteByte(value>>8);        
	VS1003_SPI_WriteByte(value);	          
	MP3_CCS(1); 
	VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
} 

/*******************************************************************************
*******************************************************************************/ 
uint16_t VS1003_ReadReg( uint8_t reg)
{ 
	uint16_t value;

	while(  MP3_DREQ ==0 );
	
	VS1003_SPI_SetSpeed( SPI_SPEED_LOW );
	
	MP3_DCS(1);     
	MP3_CCS(0); 
	
	VS1003_SPI_WriteByte(VS_READ_COMMAND);/* VS1003 */
	VS1003_SPI_WriteByte( reg );   
	value = VS1003_SPI_ReadByte();		
	value = value << 8;
	value |= VS1003_SPI_ReadByte(); 
	
	MP3_CCS(1);
	
	VS1003_SPI_SetSpeed( SPI_SPEED_HIGH );
	
	return value; 
} 

/*******************************************************************************
*******************************************************************************/                       
void VS1003_ResetDecodeTime(void)
{
   VS1003_WriteReg(SPI_DECODE_TIME, 0x0000);
   VS1003_WriteReg(SPI_DECODE_TIME, 0x0000); /* */
}

/*******************************************************************************
*******************************************************************************/    
uint16_t VS1003_GetDecodeTime(void)
{ 
   return VS1003_ReadReg(SPI_DECODE_TIME);   
} 

/*******************************************************************************
*******************************************************************************/
void VS1003_SoftReset(void)
{
	uint8_t retry; 	
				 
	while(  MP3_DREQ ==0 );

	VS1003_WriteReg(SPI_MODE, (SM_SDINEW | SM_RESET));  /* */
	HAL_Delay(2);                        /* 1.35ms */
	
	while(  MP3_DREQ ==0 );

	retry = 0;
	/*
		Name Bits Description
		MULT 15:13 Clock multiplier
		ADD 12:11 Allowed multiplier addition
		FREQ 10: 0 Clock frequency
	*/
	/*
		0x9800 = 1001 1000 0000 0000
		100 -> 0x8000 XTALI x 3.0
		11  -> 0x1800 1.5 x 
		0~~~
	*/
	while( VS1003_ReadReg(SPI_CLOCKF) != 0X9800 )  
	{
		VS1003_WriteReg(SPI_CLOCKF, 0X9800);
		if( retry++ > 100 )
		{ 
			printf("SPI_CLOCKF Set Error\r\n");
			break; 
		}
	}
	 
	retry = 0;
	/* 0xBB80 = 48000Hz samplerate, 0x1 = stereo type */
	while( VS1003_ReadReg(SPI_AUDATA) != 0XBB81 ) 
	{
		VS1003_WriteReg(SPI_AUDATA, 0XBB81);
		if( retry++ > 100 )
		{ 
			printf("SPI_AUDATA Set Error\r\n");
			break; 
		}   
	}

	VS1003_WriteReg(SPI_VOL, 0x0000);			 
	VS1003_ResetDecodeTime();
} 

/*******************************************************************************
*******************************************************************************/
void VS1003_SineTest(void)
{	
	VS1003_WriteReg(0x0b,0X2020);	 
	VS1003_WriteReg(SPI_MODE,0x0820);	    
	while(  MP3_DREQ ==0 );

	/* vs1003 : 0x53 0xef 0x6e n 0x00 0x00 0x00 0x00 */
	MP3_CCS(1);
	MP3_DCS(0);
	VS1003_SPI_WriteByte(0x53);
	VS1003_SPI_WriteByte(0xef);
	VS1003_SPI_WriteByte(0x6e);
	VS1003_SPI_WriteByte(0x24);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);

	while(  MP3_DREQ ==0 );  /* DREQ*/
	MP3_CCS(0);
	MP3_DCS(1); 

	HAL_Delay(500);
	
	/* */
	MP3_CCS(1);
	MP3_DCS(0);
	VS1003_SPI_WriteByte(0x45);
	VS1003_SPI_WriteByte(0x78);
	VS1003_SPI_WriteByte(0x69);
	VS1003_SPI_WriteByte(0x74);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);

	while(  MP3_DREQ ==0 );  /* DREQ*/
	MP3_CCS(0);
	MP3_DCS(1); 

	HAL_Delay(500);	 

	/*  0x44 */
	MP3_CCS(1);
	MP3_DCS(0);     
	VS1003_SPI_WriteByte(0x53);
	VS1003_SPI_WriteByte(0xef);
	VS1003_SPI_WriteByte(0x6e);
	VS1003_SPI_WriteByte(0x44);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	while(  MP3_DREQ ==0 );  /* DREQ*/
	MP3_CCS(0);
	MP3_DCS(1); 

	HAL_Delay(500);
	
	/*  */
	MP3_CCS(1);
	MP3_DCS(0);      
	VS1003_SPI_WriteByte(0x45);
	VS1003_SPI_WriteByte(0x78);
	VS1003_SPI_WriteByte(0x69);
	VS1003_SPI_WriteByte(0x74);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	VS1003_SPI_WriteByte(0x00);
	while(  MP3_DREQ ==0 );  /* DREQ*/
	MP3_CCS(0);
	MP3_DCS(1); 

	HAL_Delay(500); 
}	 

/*******************************************************************************
*******************************************************************************/																			 
void VS1003_RamTest(void)
{
   volatile uint16_t value;
     
   VS1003_Init();
	
   VS1003_WriteReg(SPI_MODE,0x0820);
   while(  MP3_DREQ ==0 );
	
   MP3_DCS(0);
   VS1003_SPI_WriteByte(0x4d);
   VS1003_SPI_WriteByte(0xea);
   VS1003_SPI_WriteByte(0x6d);
   VS1003_SPI_WriteByte(0x54);
   VS1003_SPI_WriteByte(0x00);
   VS1003_SPI_WriteByte(0x00);
   VS1003_SPI_WriteByte(0x00);
   VS1003_SPI_WriteByte(0x00);
   HAL_Delay(50);  
   MP3_DCS(1);
   value = VS1003_ReadReg(SPI_HDAT0); /* 0x807F */
}     
		 				
/*******************************************************************************
*******************************************************************************/   
void VS1003_SetVol(void)
{
   uint8_t i;
   uint16_t bass=0;
   uint16_t volt=0;
   uint8_t  vset=0;
		 
   vset = 255 - vs1003ram[4];
   volt = vset;
   volt <<= 8;
   volt += vset;
	
   /* 0,henh.1,hfreq.2,lenh.3,lfreq */      
   for( i = 0; i < 4; i++ )
   {
       bass <<= 4;
       bass += vs1003ram[i]; 
   }     
   VS1003_WriteReg(SPI_BASS, bass);  
   VS1003_WriteReg(SPI_VOL, volt); 
}    

/*******************************************************************************
*******************************************************************************/ 
void VS1003_Record_Init(void)
{
  uint8_t retry; 	

  /* */
  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_BASS) != 0x0000 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x0000);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* Set sample rate divider=12 */ 
  while( VS1003_ReadReg(SPI_AICTRL0) != 0x0012 )   
  {
	  VS1003_WriteReg(SPI_AICTRL0,0x0012);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* AutoGain OFF, reclevel 0x1000 */ 
  while( VS1003_ReadReg(SPI_AICTRL1) != 0x1000 )   
  {
	  VS1003_WriteReg(SPI_AICTRL1,0x1000);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* RECORD,NEWMODE,RESET */ 
  while( VS1003_ReadReg(SPI_MODE) != 0x1804 )   
  {
	  VS1003_WriteReg(SPI_MODE,0x1804);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF,0x9800);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	
}

/*******************************************************************************
*******************************************************************************/ 
void VS1003_LineIn_Init(void)
{
  uint8_t retry; 	

  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF, 0x9800);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_BASS) != 0x0000 )   
  {
	  VS1003_WriteReg(SPI_BASS, 0x0000);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* Set sample rate divider=12 */ 
  while( VS1003_ReadReg(SPI_AICTRL0) != 0x0012 )   
  {
	  VS1003_WriteReg(SPI_AICTRL0, 0x0012);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* AutoGain OFF, reclevel 0x1000 */ 
  while( VS1003_ReadReg(SPI_AICTRL1) != 0x1000 )   
  {
	  VS1003_WriteReg(SPI_AICTRL1, 0x1000);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  /* RECORD,NEWMODE,RESET */ 
  while( VS1003_ReadReg(SPI_MODE) != 0x5804 )   
  {
	  VS1003_WriteReg(SPI_MODE, 0x5804);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	

  while( VS1003_ReadReg(SPI_CLOCKF) != 0x9800 )   
  {
	  VS1003_WriteReg(SPI_CLOCKF, 0x9800);   
	  HAL_Delay(2);                        /* 1.35ms */
	  if( retry++ > 100 )
	  { 
	      break; 
	  }
  }	
}

/*******************************************************************************
*******************************************************************************/ 
void VS1003_PlayBeep(void)
{
	uint32_t i;   
										
	MP3_CCS(1);
	MP3_DCS(0);   

	for (i=0; i<sizeof(szBeepMP3); i++)
		VS1003_SPI_WriteByte(szBeepMP3[i]);     
	
	MP3_CCS(0);
	MP3_DCS(1);
	
	//for(i=0; i<30;i++)   
	//	VS1003_SPI_WriteByte(0x00);	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

