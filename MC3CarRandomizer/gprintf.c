#include "gprintf.h"
#include "includes/minjector.h"

int(*gprintf)(const char*, ...) = (int(*)(const char*, ...))(0);

#if (defined __DEBUG) || (defined __ENABLELOG)
static uintptr_t loc_45BBC4 = 0x45BBC4;
#endif

void gprintf_init()
{
#if (defined __DEBUG) || (defined __ENABLELOG)
	uintptr_t addr = minj_GetBranchDestination(loc_45BBC4);
	if (!addr)
		return;

	gprintf = (int(*)(const char*, ...))(addr);
#endif
}
