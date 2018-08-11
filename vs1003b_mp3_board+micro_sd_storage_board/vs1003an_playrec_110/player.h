/*

  VLSI Solution generic microcontroller example player / recorder definitions.
  v1.00.

  See VS10xx AppNote: Playback and Recording for details.

  v1.00 2012-11-23 HH  First release

*/
#ifndef PLAYER_RECORDER_H
#define PLAYER_RECORDER_H

#include "vs10xx_uc.h"

int VSTestInitHardware(void);
int VSTestInitSoftware(void);
int VSTestHandleFile(const char *fileName, int record);

void WriteSci(u_int8 addr, u_int16 data);
u_int16 ReadSci(u_int8 addr);
void WriteSdi(const u_int8 *data, u_int8 bytes);
void SaveUIState(void);
void RestoreUIState(void);
int GetUICommand(void);

void playrec_test(void); 
	
#endif
