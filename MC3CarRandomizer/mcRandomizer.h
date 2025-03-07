#ifndef MCRANDOMIZER_H
#define MCRANDOMIZER_H

#include "gprintf.h"
#if (defined __DEBUG) || (defined __ENABLELOG)
#define LOG(fmt, ...) gprintf("mcRandomizer: " fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

// Definitions

#define blank_STRHASH 0x04D51CE7
#define MCRANDOMIZER_STRHASH 0x5C7C228A

#define MC_SAVEDATA_PROFILENAME_OFFSET 0x2FC
#define MC_ONCREATE_PROFILENAME_OFFSET 0x1B
#define MC_SAVEDATA_CHATPRESET_OFFSET 0x377

#define MC3_REMIX_USA_CRC 0x60A42FF5

typedef struct _mcVehicle
{
	const char* name;
	char data[0x18];
}mcVehicle;

#ifndef __INTELLISENSE__
__attribute__((packed)) 
#endif
typedef struct _mcCarCfg
{
	char unk1[0x32];
	char plate[8];
	char unk2[0xA5];
	char vehName[20];
	char unk3[0x11];
}mcCarCfg;

typedef struct _mcProfile
{
	char unk[0x120];
	mcCarCfg* carCfg;
}mcProfile;


// Functions

void mcRandomizer_Init();
//void mcRandomizer_PrintAllVehicles();
//void mcRandomizer_PrintCurrentCar();





#endif