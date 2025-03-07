#include "bRandom.h"

unsigned int bRandSeed = 0x12345678;

#ifndef __INTELLISENSE__
asm
(
	".global bRandom_CustomSeed\n"
	"bRandom_CustomSeed:\n"
	".byte 0x03\n"
	".byte 0x00\n"
	".byte 0x80\n"
	".byte 0x14\n"
	".byte 0x2D\n"
	".byte 0x30\n"
	".byte 0xA0\n"
	".byte 0x00\n"
	".byte 0x08\n"
	".byte 0x00\n"
	".byte 0xE0\n"
	".byte 0x03\n"
	".byte 0x2D\n"
	".byte 0x10\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0xC3\n"
	".byte 0x8C\n"
	".byte 0x87\n"
	".byte 0x1D\n"
	".byte 0x05\n"
	".byte 0x3C\n"
	".byte 0x41\n"
	".byte 0x2B\n"
	".byte 0xA5\n"
	".byte 0x34\n"
	".byte 0x01\n"
	".byte 0x00\n"
	".byte 0x80\n"
	".byte 0x50\n"
	".byte 0xCD\n"
	".byte 0x01\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0x1B\n"
	".byte 0x00\n"
	".byte 0x64\n"
	".byte 0x00\n"
	".byte 0x26\n"
	".byte 0x18\n"
	".byte 0x65\n"
	".byte 0x00\n"
	".byte 0x42\n"
	".byte 0x21\n"
	".byte 0x03\n"
	".byte 0x00\n"
	".byte 0x26\n"
	".byte 0x20\n"
	".byte 0x64\n"
	".byte 0x00\n"
	".byte 0xC0\n"
	".byte 0x2E\n"
	".byte 0x04\n"
	".byte 0x00\n"
	".byte 0x26\n"
	".byte 0x18\n"
	".byte 0x65\n"
	".byte 0x00\n"
	".byte 0x26\n"
	".byte 0x20\n"
	".byte 0x83\n"
	".byte 0x00\n"
	".byte 0x10\n"
	".byte 0x10\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0x08\n"
	".byte 0x00\n"
	".byte 0xE0\n"
	".byte 0x03\n"
	".byte 0x00\n"
	".byte 0x00\n"
	".byte 0xC4\n"
	".byte 0xAC\n"

);
#endif

unsigned int bRandom(int range)
{
	return bRandom_CustomSeed(range, &bRandSeed);
}

void bRandom_SetSeed(unsigned int seed)
{
	bRandSeed = seed;
}

unsigned int bRandom_GetSeed()
{
	return bRandSeed;
}
