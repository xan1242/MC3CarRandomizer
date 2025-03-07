#ifndef BRANDOM_H
#define BRANDOM_H

unsigned int bRandom_CustomSeed(int range, unsigned int* seed);
unsigned int bRandom(int range);
void bRandom_SetSeed(unsigned int seed);
unsigned int bRandom_GetSeed();

#endif