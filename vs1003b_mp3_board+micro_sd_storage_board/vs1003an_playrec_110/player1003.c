/*

  VLSI Solution generic microcontroller example player / recorder for
  VS1003.

  v1.10 2016-05-09 HH  Added chip type recognition, modified quick sanity check
  v1.00 2012-11-28 HH  First release

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "player.h"


#include "spi.h"
#include "fatfs.h"
#include "sdio.h"

#include "vs1003.h"

#define FILE_BUFFER_SIZE 512
#define SDI_MAX_TRANSFER_SIZE 32
#define SDI_END_FILL_BYTES       2050
#define REC_BUFFER_SIZE 512


/* How many transferred bytes between collecting data.
   A value between 1-8 KiB is typically a good value.
   If REPORT_ON_SCREEN is defined, a report is given on screen each time
   data is collected. */
#define REPORT_INTERVAL 4096
#define REPORT_INTERVAL_MIDI 512
#if 1
#define REPORT_ON_SCREEN
#endif

/* Define PLAYER_USER_INTERFACE if you want to have a user interface in your
   player. */
#if 0
#define PLAYER_USER_INTERFACE
#endif

/* Define RECORDER_USER_INTERFACE if you want to have a user interface in your
   player. */
#if 0
#define RECORDER_USER_INTERFACE
#endif


#define min(a,b) (((a)<(b))?(a):(b))



enum AudioFormat {
  afUnknown,
  afRiff,
  afMp3,
  afMidi,
} audioFormat = afUnknown;

const char *afName[] = {
  "unknown",
  "RIFF",
  "MP3",
  "MIDI",
};






enum PlayerStates {
  psPlayback = 0,
  psUserRequestedCancel,
  psCancelSentToVS10xx,
  psStopped
} playerState;





void SetSpeedSPI( uint8_t SpeedSet)
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

void WriteSpi(u_int8 data)
{
	HAL_StatusTypeDef res;
	uint8_t cmd[1] = {0, };

	cmd[0] = data;
	res = HAL_SPI_Transmit(&hspi2, cmd, sizeof(cmd), HAL_MAX_DELAY);
	if (res != HAL_OK)
		printf("HAL_SPI_Transmit Error\r\n");
}

uint8_t ReadSpi(void)
{	
	HAL_StatusTypeDef res;
	uint8_t rxData[1] = {0, };

	res = HAL_SPI_Receive(&hspi2, rxData, sizeof(rxData), HAL_MAX_DELAY);
	if (res != HAL_OK)
		printf("HAL_SPI_Receive Error\r\n");

  return rxData[0];
}

void WriteSci(u_int8 addr, u_int16 data)
{
	while(  MP3_DREQ ==0 );           /* */

	SetSpeedSPI( SPI_SPEED_LOW );	 
	MP3_DCS(1); 
	MP3_CCS(0); 
	WriteSpi(VS_WRITE_COMMAND); /*  VS1003 */
	WriteSpi(addr);             
	WriteSpi(data>>8);        
	WriteSpi(data);	          
	MP3_CCS(1); 
	SetSpeedSPI( SPI_SPEED_HIGH );
}

u_int16 ReadSci(u_int8 addr)
{
	uint16_t value;

	while(  MP3_DREQ ==0 );
	
	SetSpeedSPI( SPI_SPEED_LOW );
	
	MP3_DCS(1);     
	MP3_CCS(0); 
	
	WriteSpi(VS_READ_COMMAND);/* VS1003 */
	WriteSpi( addr );   
	value = ReadSpi();		
	value = value << 8;
	value |= ReadSpi(); 
	
	MP3_CCS(1);
	
	SetSpeedSPI( SPI_SPEED_HIGH );
	
	return value; 
}

void WriteSdi(const u_int8 *data, u_int8 bytes)
{
	HAL_StatusTypeDef res;
	uint8_t cmd[1] = {0, };
	uint8_t i;
	
	for(i=0; i<bytes; i++)
	{
		WriteSpi(data[i]);		
	}
}


/*

  This function plays back an audio file.

  It also contains a simple user interface, which requires the following
  funtions that you must provide:
  void SaveUIState(void);
  - saves the user interface state and sets the system up
  - may in many cases be implemented as an empty function
  void RestoreUIState(void);
  - Restores user interface state before exit
  - may in many cases be implemented as an empty function
  int GetUICommand(void);
  - Returns -1 for no operation
  - Returns -2 for cancel playback command
  - Returns any other for user input. For supported commands, see code.

*/
//void VS1003PlayFile(FILE *readFp) 
void VS1003PlayFile(FIL *readFp) 	
{
  static u_int8 playBuf[FILE_BUFFER_SIZE];
  u_int32 bytesInBuffer;        // How many bytes in buffer left
  u_int32 pos=0;                // File position
  long nextReportPos=0; // File pointer where to next collect/report
  int i;
#ifdef PLAYER_USER_INTERFACE
  int volLevel = ReadSci(SCI_VOL) & 0xFF; // Assume both channels at same level
  int c;
#endif /* PLAYER_USER_INTERFACE */

#ifdef PLAYER_USER_INTERFACE
  SaveUIState();
#endif /* PLAYER_USER_INTERFACE */

  playerState = psPlayback;             // Set state to normal playback

  WriteSci(SCI_DECODE_TIME, 0);         // Reset DECODE_TIME


  /* Main playback loop */

  //while ((bytesInBuffer = fread(playBuf, 1, FILE_BUFFER_SIZE, readFp)) > 0 &&
	while ((f_read(readFp, playBuf, FILE_BUFFER_SIZE, (UINT*)&bytesInBuffer) == FR_OK) &&
         playerState != psStopped) {
    u_int8 *bufP = playBuf;

    while (bytesInBuffer && playerState != psStopped) {
      {
        int t = min(SDI_MAX_TRANSFER_SIZE, bytesInBuffer);

        // This is the heart of the algorithm: on the following line
        // actual audio data gets sent to VS10xx.
        WriteSdi(bufP, t);

        bufP += t;
        bytesInBuffer -= t;
        pos += t;
      }

      /* If the user has requested cancel, set VS10xx SM_OUTOFWAV bit */
      if (playerState == psUserRequestedCancel) {
        if (audioFormat == afMp3 || audioFormat == afUnknown) {
          playerState = psStopped;
        } else {
          unsigned short oldMode;
          playerState = psCancelSentToVS10xx;
          printf("\nSetting SM_OUTOFWAV at file offset %ld\r\n", pos);
          oldMode = ReadSci(SCI_MODE);
          WriteSci(SCI_MODE, oldMode | SM_OUTOFWAV);
        }
      }

      /* If VS10xx SM_OUTOFWAV bit has been set, see if it has gone
         through. If it is, it is time to stop playback. */
      if (playerState == psCancelSentToVS10xx) {
        unsigned short mode = ReadSci(SCI_MODE);
        if (!(mode & SM_OUTOFWAV)) {
          printf("SM_OUTOFWAV has cleared at file offset %ld\n", pos);
          playerState = psStopped;
        }
      }


      /* If playback is going on as normal, see if we need to collect and
         possibly report */
      if (playerState == psPlayback && pos >= nextReportPos) {
#ifdef REPORT_ON_SCREEN
        u_int16 sampleRate;
        u_int16 h1 = ReadSci(SCI_HDAT1);
#endif

        nextReportPos += (audioFormat == afMidi || audioFormat == afUnknown) ?
          REPORT_INTERVAL_MIDI : REPORT_INTERVAL;

#ifdef REPORT_ON_SCREEN
        if (h1 == 0x7665) {
          audioFormat = afRiff;
        } else if (h1 == 0x4d54) {
          audioFormat = afMidi;
        } else if ((h1 & 0xffe6) == 0xffe2) {
          audioFormat = afMp3;
        } else {
          audioFormat = afUnknown;
        }

        sampleRate = ReadSci(SCI_AUDATA);

        printf("\r\n%ldKiB "
               "%1ds %dHz %s %s"
               " %04x   \r\n",
               pos/1024,
               ReadSci(SCI_DECODE_TIME),
               sampleRate & 0xFFFE, (sampleRate & 1) ? "stereo" : "mono",
               afName[audioFormat], h1
               );
          
        //fflush(stdout);
#endif /* REPORT_ON_SCREEN */
      }
    } /* if (playerState == psPlayback && pos >= nextReportPos) */
  


    /* User interface. This can of course be completely removed and
       basic playback would still work. */

#ifdef PLAYER_USER_INTERFACE
    /* GetUICommand should return -1 for no command and -2 for CTRL-C */
    c = GetUICommand();
    switch (c) {

      /* Volume adjustment */
    case '-':
      if (volLevel < 255) {
        volLevel++;
        WriteSci(SCI_VOL, volLevel*0x101);
      }
      break;
    case '+':
      if (volLevel) {
        volLevel--;
        WriteSci(SCI_VOL, volLevel*0x101);
      }
      break;

      /* Show some interesting registers */
    case '_':
      printf("\nvol %1.1fdB, MODE %04x, ST %04x, "
             "HDAT1 %04x HDAT0 %04x\n",
             -0.5*volLevel,
             ReadSci(SCI_MODE),
             ReadSci(SCI_STATUS),
             ReadSci(SCI_HDAT1),
             ReadSci(SCI_HDAT0));
      break;

      /* Ask player nicely to stop playing the song. */
    case 'q':
      if (playerState == psPlayback)
        playerState = psUserRequestedCancel;
      break;

      /* Forceful and ugly exit. For debug uses only. */
    case 'Q':
      RestoreUIState();
      printf("\n");
      exit(EXIT_SUCCESS);
      break;

      /* Toggle differential mode */
    case 'd':
      {
        u_int16 t = ReadSci(SCI_MODE) ^ SM_DIFF;
        printf("\nDifferential mode %s\n", (t & SM_DIFF) ? "on" : "off");
        WriteSci(SCI_MODE, t);
      }
      break;

      /* Show help */
    case '?':
      printf("\nInteractive VS1003 file player keys:\n"
             "- +\tVolume down / up\n"
             "_\tShow current settings\n"
             "q Q\tQuit current song / program\n"
             "d\tToggle Differential\n"
             );
      break;

      /* Unknown commands or no command at all */
    default:
      if (c < -1) {
        printf("Ctrl-C, aborting\n");
        fflush(stdout);
        RestoreUIState();
        exit(EXIT_FAILURE);
      }
      if (c >= 0) {
        printf("\nUnknown char '%c' (%d)\n", isprint(c) ? c : '.', c);
      }
      break;
    } /* switch (c) */
#endif /* PLAYER_USER_INTERFACE */
  } /* while ((bytesInBuffer = fread(...)) > 0 && playerState != psStopped) */


  
#ifdef PLAYER_USER_INTERFACE
  RestoreUIState();
#endif /* PLAYER_USER_INTERFACE */

  printf("\r\nSending %d footer %d's... \r\n", SDI_END_FILL_BYTES, 0);
  //fflush(stdout);

  /* Earlier we collected endFillByte. Now, just in case the file was
     broken, or if a cancel playback command has been given, write
     lots of endFillBytes. */
  memset(playBuf, 0, sizeof(playBuf));
  for (i=0; i<SDI_END_FILL_BYTES; i+=SDI_MAX_TRANSFER_SIZE) {
    WriteSdi(playBuf, SDI_MAX_TRANSFER_SIZE);
  }

  /* If SM_OUTOFWAV is on at this point, there is some weirdness going
     on. Reset the IC just in case. */
  if (ReadSci(SCI_MODE) & SM_OUTOFWAV) {
    VSTestInitSoftware();
  }

  /* That's it. Now we've played the file as we should, and left VS10xx
     in a stable state. It is now safe to call this function again for
     the next song, and again, and again... */
  printf("ok\r\n");
}









u_int8 adpcmHeader[60] = {
  'R', 'I', 'F', 'F',
  0xFF, 0xFF, 0xFF, 0xFF,
  'W', 'A', 'V', 'E',
  'f', 'm', 't', ' ',
  0x14, 0, 0, 0,          /* 20 */
  0x11, 0,                /* IMA ADPCM */
  0x1, 0,                 /* chan */
  0x0, 0x0, 0x0, 0x0,     /* sampleRate */
  0x0, 0x0, 0x0, 0x0,     /* byteRate */
  0, 1,                   /* blockAlign */
  4, 0,                   /* bitsPerSample */
  2, 0,                   /* byteExtraData */
  0xf9, 0x1,              /* samplesPerBlock = 505 */
  'f', 'a', 'c', 't',     /* subChunk2Id */
  0x4, 0, 0, 0,           /* subChunk2Size */
  0xFF, 0xFF, 0xFF, 0xFF, /* numOfSamples */
  'd', 'a', 't', 'a',
  0xFF, 0xFF, 0xFF, 0xFF
};

void Set32(u_int8 *d, u_int32 n) {
  int i;
  for (i=0; i<4; i++) {
    *d++ = (u_int8)n;
    n >>= 8;
  }
}

void Set16(u_int8 *d, u_int16 n) {
  int i;
  for (i=0; i<2; i++) {
    *d++ = (u_int8)n;
    n >>= 8;
  }
}


/*
  This function records an audio file in Ogg, MP3, or WAV formats.
  If recording in WAV format, it updates the RIFF length headers
  after recording has finished.
*/
//void VS1003RecordFile(FILE *writeFp) 
void VS1003RecordFile(FIL *writeFp) 	
{
	FRESULT res;
	uint32_t byteswritten;
	
  static u_int8 recBuf[REC_BUFFER_SIZE];
  u_int32 nextReportPos=0;      // File pointer where to next collect/report
  u_int32 fileSize = 0;
  int volLevel = ReadSci(SCI_VOL) & 0xFF;
  int c;
  u_int32 adpcmBlocks = 0;
  u_int16 sampleRate;
  u_int16 divReg;

  playerState = psPlayback;

  printf("VS1003RecordFile\r\n");

  /* Initialize recording */

	// From Mic
  /* Set clock to a known, high value. */
  WriteSci(SCI_CLOCKF,
           HZ_TO_SC_FREQ(12288000) | SC_MULT_03_40X | SC_ADD_03_00X);


  /* Voice quality ADPCM recording at 8 kHz.
     This will result in a 32.44 kbit/s bitstream. */
  sampleRate = 8000;
  /* Calculate closest possible value for divider register */
  divReg = (int)((4.0*12288000)/256/sampleRate+0.5);
  /* Calculate back what sample rate we actually got */
  sampleRate = 4*12288000/(256*divReg);

  WriteSci(SCI_RECRATE,   divReg);
  WriteSci(SCI_RECGAIN,        0); /* 1024 = gain 1, 0 = AGC */
	
  /* Values according to VS1003b Datasheet Chapter "Adding a RIFF Header" */
  Set32(adpcmHeader+24, sampleRate);
  Set32(adpcmHeader+28, (u_int32)sampleRate*256/505);
  //fwrite(adpcmHeader, sizeof(adpcmHeader), 1, writeFp);
	if ((res = f_write(writeFp, adpcmHeader, sizeof(adpcmHeader), (void *)&byteswritten)) != FR_OK)
	{
		printf("f_write fail: %d\r\n", res);
	}	
  fileSize = sizeof(adpcmHeader);

  /* Start the encoder */
  WriteSci(SCI_MODE, ReadSci(SCI_MODE) | SM_ADPCM | SM_RESET);

#ifdef RECORDER_USER_INTERFACE
  SaveUIState();
#endif /* RECORDER_USER_INTERFACE */

  while (playerState != psStopped) {
    int n;

		if (fileSize >= 100 * 1024)
		{
			playerState = psStopped;
			break;
		}
#ifdef RECORDER_USER_INTERFACE
    {
      c = GetUICommand();
      
      switch(c) {
      case 'q':
        printf("\nSwitching encoder off...\n");
        playerState = psStopped;
        break;
      case '-':
        if (volLevel < 255) {
          volLevel++;
          WriteSci(SCI_VOL, volLevel*0x101);
        }
        break;
      case '+':
        if (volLevel) {
          volLevel--;
          WriteSci(SCI_VOL, volLevel*0x101);
        }
        break;
      case '_':
        printf("\nvol %4.1f\n", -0.5*volLevel);
        break;
      case '?':
        printf("\nInteractive VS1003 file recorder keys:\n"
               "- +\tVolume down / up\n"
               "_\tShow current settings\n"
               "q\tQuit recording\n"
               );
        break;
      default:
        if (c < -1) {
          printf("Ctrl-C, aborting\n");
          fflush(stdout);
          RestoreUIState();
          exit(EXIT_FAILURE);
        }
        if (c >= 0) {
          printf("\nUnknown char '%c' (%d)\n", isprint(c) ? c : '.', c);
        }
        break;  
      }
      
    }
#endif /* RECORDER_USER_INTERFACE */


    /* See if there is some data available */
    if ((n = ReadSci(SCI_RECWORDS)) > REC_BUFFER_SIZE) {
      int i;
      u_int8 *rbp = recBuf;

      /* Always writes one or two IMA ADPCM block(s) at a time */
      n = REC_BUFFER_SIZE/2;
      adpcmBlocks += 2;

      for (i=0; i<n; i++) {
        u_int16 w = ReadSci(SCI_RECDATA);
        *rbp++ = (u_int8)(w >> 8);
        *rbp++ = (u_int8)(w & 0xFF);
      }
      //fwrite(recBuf, 1, 2*n, writeFp);
			if ((res = f_write(writeFp, recBuf, 2*n, (void *)&byteswritten)) != FR_OK)
			{
				printf("f_write fail: %d\r\n", res);
			}
	
      fileSize += 2*n;
    }

    if (fileSize - nextReportPos >= REPORT_INTERVAL) {
      nextReportPos += REPORT_INTERVAL;
      printf("\r\n%ldKiB ", fileSize/1024);
      printf("%uHz mono RIFF \r\n", sampleRate);
      //fflush(stdout);
    }
  } /* while (playerState != psStopped) */

  
  /* Update file sizes according to VS1003b Datasheet Chapter
     "Adding a RIFF Header" */
  //fseek(writeFp, 0, SEEK_SET);
	f_lseek(writeFp, 0);
  Set32(adpcmHeader+4, fileSize-8);
  Set32(adpcmHeader+48, adpcmBlocks*505);
  Set32(adpcmHeader+56, fileSize-60);
  //fwrite(adpcmHeader, sizeof(adpcmHeader), 1, writeFp);
	if ((res = f_write(writeFp, adpcmHeader, sizeof(adpcmHeader), (void *)&byteswritten)) != FR_OK)
	{
		printf("f_write fail: %d\r\n", res);
	}


#ifdef RECORDER_USER_INTERFACE
  RestoreUIState();
#endif /* RECORDER_USER_INTERFACE */

  /* Finally, reset the VS10xx software, including realoading the
     patches package, to make sure everything is set up properly. */
  VSTestInitSoftware();

  printf("ok\r\n");
}





/*

  Hardware Initialization for VS1003.

  
*/
int VSTestInitHardware(void) {
  /* Write here your microcontroller code which puts VS10xx in hardware
     reset anc back (set xRESET to 0 for at least a few clock cycles,
     then to 1). */
  MP3_Reset(0);
  HAL_Delay(10);
  MP3_Reset(1);	
	
  return 0;
}



/* Note: code SS_VER=2 is used for both VS1002 and VS1011e */
const u_int16 chipNumber[16] = {
  1001, 1011, 1011, 1003, 1053, 1033, 1063, 1103,
  0, 0, 0, 0, 0, 0, 0, 0
};

/*

  Software Initialization for VS1003.

  Note that you need to check whether SM_SDISHARE should be set in
  your application or not.
  
*/
int VSTestInitSoftware(void) {
  u_int16 ssVer;

  /* Start initialization with a dummy read, which makes sure our
     microcontoller chips selects and everything are where they
     are supposed to be and that VS10xx's SCI bus is in a known state. */
  ReadSci(SCI_MODE);

  /* First real operation is a software reset. After the software
     reset we know what the status of the IC is. You need, depending
     on your application, either set or not set SM_SDISHARE. See the
     Datasheet for details. */
  WriteSci(SCI_MODE, SM_SDINEW|SM_SDISHARE|SM_TESTS|SM_RESET);

  /* A quick sanity check: write to two registers, then test if we
     get the same results. Note that if you use a too high SPI
     speed, the MSB is the most likely to fail when read again. */
  WriteSci(SCI_AICTRL1, 0xABAD);
  WriteSci(SCI_AICTRL2, 0x7E57);
  if (ReadSci(SCI_AICTRL1) != 0xABAD || ReadSci(SCI_AICTRL2) != 0x7E57) 
	{
    printf("There is something wrong with VS10xx SCI registers\r\n");
    return 1;
  }
  WriteSci(SCI_AICTRL1, 0);
  WriteSci(SCI_AICTRL2, 0);

  /* Check VS10xx type */
  ssVer = ((ReadSci(SCI_STATUS) >> 4) & 15);
  if (chipNumber[ssVer]) 
	{
    printf("Chip is VS%d\r\n", chipNumber[ssVer]);
    if (chipNumber[ssVer] != 1003) {
      printf("Incorrect chip\n");
      return 1;
    }
  } 
	else 
	{
    printf("Unknown VS10xx SCI_MODE field SS_VER = %d\r\n", ssVer);
    return 1;
  }

  /* Set the clock. Until this point we need to run SPI slow so that
     we do not exceed the maximum speeds mentioned in
     Chapter SPI Timing Diagram in the Datasheet. */
  WriteSci(SCI_CLOCKF,
           HZ_TO_SC_FREQ(12288000) | SC_MULT_03_30X | SC_ADD_03_10X);


  /* Now when we have upped the VS10xx clock speed, the microcontroller
     SPI bus can run faster. Do that before you start playing or
     recording files. */

  /* Set volume level at -6 dB of maximum */
  WriteSci(SCI_VOL, 0x0c0c);

  /* We're ready to go. */
  return 0;
}





/*
  Main function that activates either playback or recording.
*/
int VSTestHandleFile(const char *fileName, int record) 
{
	FRESULT res;
	FIL MyFile;
	
  if (!record) 
	{
#if 0		
    FILE *fp = fopen(fileName, "rb");
    printf("Play file %s\r\n", fileName);
    if (fp) {
      VS1003PlayFile(fp);
    } else {
      printf("Failed opening %s for reading\r\n", fileName);
      return -1;
    }
#endif		

		printf("Play file %s\r\n", fileName);
		if((res = f_open(&MyFile, fileName, FA_READ)) != FR_OK)
		{
			printf(" open error : %d \r\n",res);
			return -1;
		}
		else
		{
			VS1003PlayFile(&MyFile);
		}
  } 
	else 
	{
#if 0		
    FILE *fp = fopen(fileName, "wb");
    printf("Record file %s\n", fileName);
    if (fp) {
      VS1003RecordFile(fp);
    } else {
      printf("Failed opening %s for writing\r\n", fileName);
      return -1;
    }
#endif
		printf("Record file %s\r\n", fileName);
		if((res = f_open(&MyFile, fileName, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
		{
			printf(" open error : %d \r\n",res);
			return -1;
		}
		else
		{
			VS1003RecordFile(&MyFile);
		}		
  }

	printf(" f_close \r\n");
	if ((res = f_close(&MyFile)) != FR_OK )
	{
		printf(" close error : %d \r\n",res);
		return -1;
	}
	
  return 0;
}



void playrec_test(void) 
{
	if (VSTestInitHardware() || VSTestInitSoftware()) {
		printf("Failed initializing VS10xx, exiting\r\n");
		return;
	}
	
	/* Recording example */
	VSTestHandleFile("MyRecordFile.ogg", 1);

	/* Playback example. You can call these functions many times in a row
	because they leave VS1003 in a stable state. */
	VSTestHandleFile("MyRecordFile.ogg", 0);	
}

