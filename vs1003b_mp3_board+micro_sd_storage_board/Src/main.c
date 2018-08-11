
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>

#include "vs1003.h"
#include "player.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
extern char SDPath[4]; /* SD card logical drive path */

#define GPIO_TEST 0
#define LINE_OUT_TEST 0
#define LINE_IN_TEST 0
#define MIC_IN_TEST 1

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

FRESULT mountSDIO(void)
{
	return f_mount(&SDFatFs, (TCHAR const*)SDPath, 0);
}

FRESULT unMountSDIO(void)
{
	return f_mount(NULL, "", 0);
}

uint8_t linkFATFS(void)
{
	return FATFS_LinkDriver(&SD_Driver, SDPath);
}

uint8_t unLinkFATFS(void)
{
	return FATFS_UnLinkDriver(SDPath);
}

void FatFsTest(void)
{
  FRESULT res;                                          /* FatFs function common result code */
  uint32_t byteswritten, bytesread;                     /* File write/read counts */
	uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
  uint8_t rtext[100];                     /* File read buffers */
	char filename[] = "STM32cube.txt";
	
	printf("\r\n ****** FatFs Example ******\r\n\r\n");

  /*##-1- Link the micro SD disk I/O driver ##################################*/
  if(1)//(linkFATFS() == 0)
  {
    /*##-2- Register the file system object to the FatFs module ##############*/
		//printf(" f_mount \r\n");
    if(0)//((res = mountSDIO()) != FR_OK)
    {
      /* FatFs Initialization Error */
			printf(" mount error : %d \r\n",res);
      Error_Handler();
    }
    else
    {
      /*##-3- Create a FAT file system (format) on the logical drive #########*/
      /* WARNING: Formatting the uSD card will delete all content on the device */
			printf(" f_mkfs skip\r\n");
      if(0)//((res = f_mkfs((TCHAR const*)SDPath, 0, 0)) != FR_OK)
      {
        /* FatFs Format Error */
				printf(" mkfs error : %d \r\n",res);
        Error_Handler();
      }
      else
      {       
        /*##-4- Create and Open a new text file object with write access #####*/
				printf(" f_open \r\n");
        if((res = f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
        {
          /* 'STM32.TXT' file Open for write Error */
					printf(" open error : %d \r\n",res);
          Error_Handler();
        }
        else
        {
          /*##-5- Write data to the text file ################################*/
					printf(" f_write \r\n");
          if ((res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten)) != FR_OK)
					{
						printf(" write error : %d \r\n",res);
						Error_Handler();
					}

          /*##-6- Close the open text file #################################*/
					printf(" f_close \r\n");
          if ((res = f_close(&MyFile)) != FR_OK )
          {
						printf(" close error : %d \r\n",res);
            Error_Handler();
          }
          
          if(byteswritten == 0)
          {
            /* 'STM32.TXT' file Write or EOF Error */
						printf(" byteswritten = 0 \r\n");
            Error_Handler();
          }
          else
          {      
            /*##-7- Open the text file object with read access ###############*/
						printf(" f_open \r\n");
            if((res = f_open(&MyFile, filename, FA_READ)) != FR_OK)
            {
              /* 'STM32.TXT' file Open for read Error */
							printf(" open error : %d \r\n", res);
              Error_Handler();
            }
            else
            {
              /*##-8- Read data from the text file ###########################*/
							printf(" f_read \r\n");
              res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
              
              if((bytesread == 0) || (res != FR_OK))
              {
                /* 'STM32.TXT' file Read or EOF Error */
								printf(" read error : %d \r\n", res);
                Error_Handler();
              }
              else
              {
                /*##-9- Close the open text file #############################*/
								printf(" f_close \r\n");
								if ((res = f_close(&MyFile)) != FR_OK )
								{
									printf(" close error : %d \r\n",res);
									Error_Handler();
								}
                
                /*##-10- Compare read data with the expected data ############*/
                if((bytesread != byteswritten))
                {                
                  /* Read data is different from the expected data */
									printf(" read data wrong : ? \r\n");
                  Error_Handler();
                }
                else
                {
                  /* Success of the demo: no error occurrence */
                  printf(" Good!!! \r\n");
									HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
									HAL_Delay(500);
									HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
                }
              }
            }
          }
        }
      }
    }
  }
  
	//printf(" unmount \r\n");
	if(0)//((res = unMountSDIO()) != FR_OK )
	{
		printf(" unmount error : %d \r\n",res);
		Error_Handler();
	}
	
  /*##-11- Unlink the RAM disk I/O driver ####################################*/
  //unLinkFATFS();
}

uint32_t gFileTotalCount = 0;
uint32_t gFileTotalSize = 0;
typedef struct tagFileLinkedList
{
	char fullFileName[128];	// dir + file 
	char fileName[128];
	uint32_t fileSize;
	uint8_t isAudio;
} FileLinkedList;

FileLinkedList gFileList[128] = {0, };

void sort_files()
{
	int i, j;
	//FileLinkedList tmp = {0,};
	char tmpFullFileName[128];	// dir + file 
	char tmpFileName[128];
	uint32_t tmpFileSize;
	uint8_t tmpIsAudio;
	
	for(i=0; i<gFileTotalCount-1; i++)
  {
		for (j=i+1; j<gFileTotalCount; j++)
		{	
			if (strcmp(gFileList[i].fileName, gFileList[j].fileName) > 0)
			{
				//memcpy((void *)&tmp, (const void*)&gFileList[i], sizeof(FileLinkedList));
				//memcpy((void *)&gFileList[i], (const void*)&gFileList[j], sizeof(FileLinkedList));
				//memcpy((void *)&gFileList[j], (const void*)&tmp, sizeof(FileLinkedList));

				strcpy((char *)&tmpFullFileName, (const char*)&gFileList[i].fullFileName);
				strcpy((char *)&gFileList[i].fullFileName, (const char*)&gFileList[j].fullFileName);
				strcpy((char *)&gFileList[j].fullFileName, (const char*)&tmpFullFileName);
				
				strcpy((char *)&tmpFileName, (const char*)&gFileList[i].fileName);
				strcpy((char *)&gFileList[i].fileName, (const char*)&gFileList[j].fileName);
				strcpy((char *)&gFileList[j].fileName, (const char*)&tmpFileName);
				
				tmpFileSize = gFileList[i].fileSize;
				gFileList[i].fileSize = gFileList[j].fileSize;				
				gFileList[j].fileSize = tmpFileSize;
				
				tmpIsAudio = gFileList[i].isAudio;
				gFileList[i].isAudio = gFileList[j].isAudio;
				gFileList[j].isAudio = tmpIsAudio; 
			}
		}
  }
	
	printf("\r\n Start Check Array\r\n");
	for(i=0; i<gFileTotalCount; i++)
	{
		printf(" [%d] file: %s[%d byte][%d]\r\n", i, gFileList[i].fileName, gFileList[i].fileSize, gFileList[i].isAudio);
	}
	printf(" totalSize: %d\r\n", gFileTotalSize);
	printf(" End Check Array\r\n\r\n");	
}

FRESULT print_files (char* path)
{
	FRESULT res;
	FILINFO fno;
	DIR dir;
	int i;
	char *fn;
#if _USE_LFN
	static char lfn[_MAX_LFN+1];
	fno.lfname = lfn;
	fno.lfsize = sizeof(lfn);
#endif

	res = f_opendir(&dir, path);
	if (res == FR_OK) {
		
		for (;;) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) 
				break;
#if _USE_LFN
			fn = *fno.lfname ? fno.lfname : fno.fname;
#else
			fn = fno.fname;
#endif
			if (fno.fattrib & AM_DIR) 
			{
				char dirname[_MAX_LFN+1];
				
				if(fn[0]!='.')
				{
					i = strlen(path);
					
					//printf(" dir:  [%s/%s][attr:0x%x]\r\n", path, fn, fno.fattrib);
					
					sprintf(dirname, "%s/%s", path, fn);
					res = print_files(dirname);
					if (res != FR_OK) 
						break;
							
					path[i] = 0;		
				}
			} 
			else 
			{
				sprintf(gFileList[gFileTotalCount].fullFileName, "%s/%s", path, fn); 
				strcpy(gFileList[gFileTotalCount].fileName, fn);
				gFileList[gFileTotalCount].fileSize = fno.fsize;
				gFileTotalSize += fno.fsize;

				char seps[] = ".";
				char *tr;
				tr = strtok((char *)fn, (const char *)seps);
				//printf("  %s ", tr);
				tr = strtok(NULL, (const char *)seps);
				//printf("%s ", tr);
				if (strcmp(tr, "mp3") == 0)
				{
					gFileList[gFileTotalCount].isAudio = 1;
				}
				else
				{
					gFileList[gFileTotalCount].isAudio = 0;
				}
				
				/*printf(" file: [%s/%s] [%ld byte] [attr: 0x%x][audio: %d]\r\n", 
					gFileList[gFileTotalCount].fullFileName, 
					gFileList[gFileTotalCount].fileName, 
					gFileList[gFileTotalCount].fileSize, 
					fno.fattrib, 
					gFileList[gFileTotalCount].isAudio);*/
				
				gFileTotalCount += 1;
			}
		}
		
		f_closedir(&dir);
	}
	else
	{
		printf(" path(%s) not found\r\n", path);
	}
	return res;
}


uint8_t fileData[10240];
uint32_t fileSize;

void getFileDataFromSDCard(char *filename)
{
	FRESULT res;
  uint32_t bytesread;
	//char filename[] = "migyo_rainsound_mono_8kbps.mp3";
	
	//printf(" f_mount \r\n");
	if(0)//((res = mountSDIO()) != FR_OK)
	{
		printf(" mount error : %d \r\n",res);
		Error_Handler();
	}
	else
	{  
		printf(" f_open \r\n");
		if((res = f_open(&MyFile, filename, FA_READ)) != FR_OK)
		{
			printf(" open error : %d \r\n",res);
			Error_Handler();
		}
		else
		{
			fileSize = f_size(&MyFile);
			printf(" filesize: %d \r\n", fileSize);
			
			printf(" f_read \r\n");
			res = f_read(&MyFile, fileData, sizeof(fileData), (UINT*)&bytesread);

			printf(" f_close \r\n");
			if ((res = f_close(&MyFile)) != FR_OK )
			{
				printf(" close error : %d \r\n",res);
				Error_Handler();
			}
			printf(" Done \r\n");
		}
	}

	//printf(" unmount \r\n");
	if(0)//((res = unMountSDIO()) != FR_OK )
	{
		printf(" unmount error : %d \r\n",res);
		Error_Handler();
	}
	
  //FATFS_UnLinkDriver(SDPath);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	LED1_CTRL(GPIO_HIGH);
	
	FRESULT res;	
	printf(" f_mount \r\n");
	if((res = mountSDIO()) != FR_OK)
	{
		/* FatFs Initialization Error */
		printf(" mount error : %d \r\n",res);
		Error_Handler();
	}
	
#if LINE_OUT_TEST	
	//FatFsTest();
	//getFileListFromSDCard();
	
	char buff[128];
  strcpy(buff, "/");
	printf("\r\n Start print_files \r\n");
	print_files(buff);
	printf(" End print_files \r\n\r\n");
	
	sort_files();

	//printf(" unmount \r\n");
	//if ((res = unMountSDIO()) != FR_OK )
	//{
	//	printf(" unmount error : %d \r\n",res);
	//	Error_Handler();
	//}
#endif
	
	printf("\r\nProgram Start\r\n");
	
#if MIC_IN_TEST	
	playrec_test();
	while(1) 
	{ 
		LED1_CTRL(GPIO_TOGGLE);
		
		HAL_Delay(1000);
	}
#endif

  VS1003_Init();
  VS1003_SoftReset();

  /* Check VS10xx type */
  printf("Chip is VS %d\r\n", ((VS1003_ReadReg(SPI_STATUS) >> 4) & 0xF));

#if GPIO_TEST
	VS1003_WriteReg(SPI_WRAMADDR, 0xc017);
	VS1003_WriteReg(SPI_WRAM, 0x000f);
#endif
#if LINE_IN_TEST	
	VS1003_LineIn_Init();
#endif	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	int32_t idx = 0;
  while (1)
  {
		LED1_CTRL(GPIO_TOGGLE);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
#if LINE_OUT_TEST
		//VS1003_SineTest();
		//VS1003_PlayBeep();

		//printf("idx: %d, gFileTotalCount: %d\r\n", idx, gFileTotalCount);
		if (idx >= gFileTotalCount - 1)
			idx = 0;
		else
			idx += 1;
		
		if (gFileList[idx].isAudio)
		{		
			printf("Playing: %s\r\n", gFileList[idx].fileName);
			
			getFileDataFromSDCard(gFileList[idx].fileName);
			
			VS1003_PlayFromSDCard();
			HAL_Delay(1000);			
		}

#endif
		
#if GPIO_TEST	
		VS1003_WriteReg(SPI_WRAMADDR, 0xc019);
		VS1003_WriteReg(SPI_WRAM, 0x000f);
		HAL_Delay(500);

		VS1003_WriteReg(SPI_WRAMADDR, 0xc019);
		VS1003_WriteReg(SPI_WRAM, 0x0000);	
		HAL_Delay(500);
#endif		
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
void HAL_SD_ErrorCallback(SD_HandleTypeDef *hsd)
{
	printf("HAL_SD_ErrorCallback ErrorCode : 0x%x\r\n", hsd->ErrorCode);
	
  while(1)
  {
		  LED2_CTRL(GPIO_TOGGLE);
			HAL_Delay(500);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	printf("_Error_Handler : file[%s], line[%d]\r\n", file, line);
  while(1)
  {
		  LED2_CTRL(GPIO_TOGGLE);
			HAL_Delay(500);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
