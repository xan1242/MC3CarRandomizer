//#include <stdio.h>
#include "includes/pcsx2f_api.h"
#include "includes/minjector.h"
#include "includes/bRandom.h"
#include "gprintf.h"
#include "mcRandomizer.h"

int CompatibleCRCList[] = { MC3_REMIX_USA_CRC };


void early_init()
{
    // get the game's printf function address for debugging purposes
    gprintf_init();
}

//void TestFuncHook()
//{
//    LOG("Entered audio options...\n");
//    //mcRandomizer_PrintAllVehicles();
//    mcRandomizer_PrintCurrentCar();
//
//    //unsigned int randTest = bRandom(10);
//    //LOG("rand test: %d\n", randTest);
//}

void init()
{
    early_init();

    //minj_MakeCALL(0x384440, (uintptr_t)&TestFuncHook);

    LOG("hello from plugin!\n");
    //LOG("sizeof mcCarCfg: 0x%X\n", sizeof(mcCarCfg));

    mcRandomizer_Init();
}

int main()
{
    return 0;
}