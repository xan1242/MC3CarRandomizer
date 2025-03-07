#include <stdint.h>
#include <string.h>
#include "mcRandomizer.h"
#include "includes/stringhash.h"
#include "includes/bRandom.h"
//#include "includes/ps2timeutil.h"
#include "includes/minjector.h"

// functions
uintptr_t loc_4AE100 = 0x004AE100;
void(*SetCarCfg)(mcCarCfg* dest, mcCarCfg* src) = (void(*)(mcCarCfg*, mcCarCfg*))(0);

uintptr_t loc_1B0C20 = 0x1B0C20;
uintptr_t(*OnLoadCareerDataDone)(uintptr_t unk1, uintptr_t unk2, const char* label) = (uintptr_t(*)(uintptr_t, uintptr_t, const char*))(0);

//uintptr_t loc_1B0664 = 0x1B0664;
//uintptr_t(*OnSaveCareerDataDone)(uintptr_t unk1, uintptr_t unk2, const char* label) = (uintptr_t(*)(uintptr_t, uintptr_t, const char*))(0);

uintptr_t loc_1AE8A0 = 0x1AE8A0;
void(*OnSaveData_1)(uintptr_t unk1, uintptr_t unk2) = (void(*)(uintptr_t, uintptr_t))(0);

uintptr_t loc_1AF4F8 = 0x1AF4F8;
void(*OnSaveData_2)(uintptr_t unk1, uintptr_t unk2) = (void(*)(uintptr_t, uintptr_t))(0);

uintptr_t loc_1AF098 = 0x1AF098;
void(*OnCreateSavegame)(uintptr_t unk1, int unk2, const char* str, uintptr_t unk3, uintptr_t unk5, int unk6) = (void(*)(uintptr_t, int, const char*, uintptr_t, uintptr_t, int unk6))(0);

uintptr_t loc_3EE860 = 0x3EE860;
void(*OnRaceOver)(uintptr_t unk1, const char* str) = (void(*)(uintptr_t, const char*))(0);

uintptr_t loc_3EDAC8 = 0x3EDAC8;
void(*OnRaceFinished)(uintptr_t unk1) = (void(*)(uintptr_t))(0);

//uintptr_t loc_4AF878 = 0x4AF878;
//int(*GetVehicleID)(const char* vehName) = (int(*)(const char*))(0);

// vehicle list stuff
uintptr_t ppVehList = 0x006E0170;
uintptr_t pVehCount = 0x006E0174;

// user profile stuff
uintptr_t pProfile = 0x00619B14;

int CustomSeedWasSet = 0;
int CurrentVehicleIndex = -1;
int RaceFinishedReached = 0;

uint16_t mcRandomizer_GetVehicleCount()
{
	return *(uint16_t*)pVehCount;
}

mcVehicle* mcRandomizer_GetVehicleList()
{
	return *(mcVehicle**)ppVehList;
}

mcProfile* mcRandomizer_GetUserProfile()
{
	return *(mcProfile**)pProfile;
}

mcCarCfg* mcRandomizer_GetUserCarCfg()
{
	mcProfile* profile = mcRandomizer_GetUserProfile();
	if (!profile)
		return NULL;

	return profile->carCfg;
}

int mcRandomizer_IsVehicleIndexOutOfBounds(int id)
{
	int vehCount = mcRandomizer_GetVehicleCount();
	if ((CurrentVehicleIndex >= vehCount) || (CurrentVehicleIndex < -1))
	{
		return 1;
	}

	return 0;
}

void mcRandomizer_ApplyOverrideVehicle()
{
	mcVehicle* vehList = mcRandomizer_GetVehicleList();
	if (!vehList)
		return;

	mcCarCfg* userCfg = mcRandomizer_GetUserCarCfg();
	if (!userCfg)
		return;

	if (mcRandomizer_IsVehicleIndexOutOfBounds(CurrentVehicleIndex))
	{
		LOG("ApplyOverrideVehicle: ERROR: vehicle %d exceeds the boundaries! Resetting to 0...\n", CurrentVehicleIndex);
		CurrentVehicleIndex = 0;
	}

	const char* overrideVehicle = vehList[CurrentVehicleIndex].name;
	LOG("ApplyOverrideVehicle: Override with vehicle: %s\n", overrideVehicle);

	//strncpy(userCfg->vehName, name, 19);
	//userCfg->vehName[20] = '\0';
	// this actually might be safer because the game is prone to softlocking if an invalid vehicle name is passed...
	strcpy(userCfg->vehName, overrideVehicle);
}

//void mcRandomizer_PrintAllVehicles()
//{
//	int count = mcRandomizer_GetVehicleCount();
//	mcVehicle* vehicles = mcRandomizer_GetVehicleList();
//
//	for (int i = 0; i < count; i++)
//	{
//		mcVehicle* v = &vehicles[i];
//
//		LOG("Veh [%d] = %s\n", i, v->name);
//	}
//	LOG("Total vehicles: %d\n", count);
//}

//void mcRandomizer_PrintCurrentCar()
//{
//	mcProfile* profile = mcRandomizer_GetUserProfile();
//	mcCarCfg* cfg = profile->carCfg;
//
//	if (!cfg)
//	{
//		LOG("No CarCfg in profile!\n");
//		return;
//	}
//
//	LOG("Profile at 0x%X\n", profile);
//	LOG("CarCfg at 0x%X\n", cfg);
//	LOG("==== VEHICLE INFO ====\nVehicle: %s\nPlate: %s\n", cfg->vehName, cfg->plate);
//}

uintptr_t mcRandomizer_OnLoadCareerDataDone(uintptr_t unk1, uintptr_t unk2, const char* label)
{
	uintptr_t result = OnLoadCareerDataDone(unk1, unk2, label);

	char* profName = (char*)(unk1 + MC_SAVEDATA_PROFILENAME_OFFSET);
	LOG("OnLoadCareerDataDone: Profile: %s\n", profName);

	LOG("OnLoadCareerDataDone args: a0: 0x%X , a1: 0x%X, result: 0x%X, label: %s\n", unk1, unk2, result, label);
	
	unsigned int* chatTxt = (unsigned int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET);
	unsigned int* seedChk = (unsigned int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET + sizeof(unsigned int));
	int* vehicleID = (int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET + sizeof(unsigned int) + sizeof(int));

	if (*seedChk == MCRANDOMIZER_STRHASH)
	{
		unsigned int seed = *chatTxt;
		LOG("OnLoadCareerDataDone: Loaded seed: 0x%X\n", seed);
		bRandom_SetSeed(seed);
		CurrentVehicleIndex = *vehicleID;

		if (mcRandomizer_IsVehicleIndexOutOfBounds(CurrentVehicleIndex))
		{
			LOG("OnLoadCareerDataDone: ERROR: vehicle %d exceeds the boundaries! Resetting to 0...\n", CurrentVehicleIndex);
			CurrentVehicleIndex = 0;
		}

	}
	else
	{
		unsigned int seed = bStringHash(profName);
		LOG("OnLoadCareerDataDone: Failed to load seed, generated seed from profile name: 0x%X\n", seed);
		bRandom_SetSeed(seed);
	}

	CustomSeedWasSet |= 1;

	return result;
}

//uintptr_t mcRandomizer_OnSaveCareerDataDone(uintptr_t unk1, uintptr_t unk2, const char* label)
//{
//	uintptr_t result = OnSaveCareerDataDone(unk1, unk2, label);
//
//	char* profName = (char*)(unk1 + 0x2FC);
//
//	LOG("OnSaveCareerDataDone: %s\n", profName);
//	LOG("OnSaveCareerDataDone args: a0: 0x%X , a1: 0x%X, result: 0x%X, label: %s\n", unk1, unk2, result, label);
//
//
//	return result;
//}

void mcRandomizer_OnSaveData_PreHook(uintptr_t unk1, uintptr_t unk2)
{
	char* profName = (char*)(unk1 + MC_SAVEDATA_PROFILENAME_OFFSET);
	LOG("OnSaveData: Profile: %s\n", profName);

	unsigned int seed = 0;

	if (CustomSeedWasSet & 1)
		seed = bRandom_GetSeed();
	else
	{
		// #TODO this is a workaround for fresh profiles (THIS WILL NOT WORK IF THE USER DOESN'T SAVE)
		LOG("OnSaveData: Generating seed from profile name...\n");
		seed = bStringHash(profName);
		bRandom_SetSeed(seed);
		CustomSeedWasSet |= 1;
	}

	LOG("OnSaveData: Injecting current vehicle ID and seed into the chat text preset...\n");
	LOG("OnSaveData: Seed is: 0x%X\n", seed);
	LOG("OnSaveData: Vehicle is: %d\n", CurrentVehicleIndex);

	if (mcRandomizer_IsVehicleIndexOutOfBounds(CurrentVehicleIndex))
	{
		LOG("OnSaveData: ERROR: vehicle %d exceeds the boundaries! Resetting to 0...\n", CurrentVehicleIndex);
		CurrentVehicleIndex = 0;
	}

	unsigned int* chatTxt = (unsigned int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET);
	*chatTxt = seed;

	unsigned int* seedChk = (unsigned int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET + sizeof(unsigned int));
	*seedChk = MCRANDOMIZER_STRHASH;

	int* vehicleID = (int*)(unk1 + MC_SAVEDATA_CHATPRESET_OFFSET + sizeof(unsigned int) + sizeof(int));
	*vehicleID = CurrentVehicleIndex;

	CustomSeedWasSet |= 1;
	RaceFinishedReached = 0;
}

void mcRandomizer_OnSaveData_Hook_1(uintptr_t unk1, uintptr_t unk2)
{
	mcRandomizer_OnSaveData_PreHook(unk1, unk2);

	return OnSaveData_1(unk1, unk2);
}

void mcRandomizer_OnSaveData_Hook_2(uintptr_t unk1, uintptr_t unk2)
{
	mcRandomizer_OnSaveData_PreHook(unk1, unk2);

	return OnSaveData_2(unk1, unk2);
}

void mcRandomizer_OnCreateSavegame_Hook(uintptr_t unk1, int unk2, const char* str, uintptr_t unk3, uintptr_t unk4, int unk5)
{
	char* profName = (char*)(unk3 + MC_ONCREATE_PROFILENAME_OFFSET);
	LOG("OnCreateSavegame: Profile: %s\n", profName);

	LOG("OnCreateSavegame: Generating seed from profile name...\n");
	bRandom_SetSeed(bStringHash(profName));
	CustomSeedWasSet |= 1;
	CurrentVehicleIndex = -1;

	return OnCreateSavegame(unk1, unk2, str, unk3, unk4, unk5);
}

void mcRandomizer_OnRaceOver_Hook(uintptr_t unk1, const char* str)
{
	LOG("OnRaceOver: screen reached...\n");

	RaceFinishedReached = 0;

	return OnRaceOver(unk1, str);
}

void mcRandomizer_OnRaceFinished_Hook(uintptr_t unk1)
{
	if (RaceFinishedReached)
	{
		LOG("OnRaceFinished: not randomizing, already reached race finish. Waiting for the Race Over screen to appear or a save to happen...\n");
		return OnRaceFinished(unk1);
	}

	LOG("OnRaceFinished: randomizing and applying vehicle...\n");
	int vehCount = mcRandomizer_GetVehicleCount();

	CurrentVehicleIndex = bRandom(vehCount);

	mcCarCfg* userCfg = mcRandomizer_GetUserCarCfg();
	if (!userCfg)
		return;

	// override
	if (CurrentVehicleIndex >= 0)
	{
		mcRandomizer_ApplyOverrideVehicle();
	}

	RaceFinishedReached = 1;

	return OnRaceFinished(unk1);
}

void mcRandomizer_SetCarCfgHook(mcCarCfg* dest, mcCarCfg* src)
{
	SetCarCfg(dest, src);
	mcCarCfg* userCfg = mcRandomizer_GetUserCarCfg();
	if (dest != userCfg)
		return;
	LOG("SetCarCfgHook: Set user CarCfg with vehicle: %s\n", userCfg->vehName);

	int nameHash = bStringHash(userCfg->vehName);
	if (nameHash == blank_STRHASH)
		return;

	// override
	if (CurrentVehicleIndex >= 0)
	{
		mcRandomizer_ApplyOverrideVehicle();
	}
}

void mcRandomizer_Init()
{
	uintptr_t pSetCarCfg = minj_GetBranchDestination(loc_4AE100);
	SetCarCfg = (void(*)(mcCarCfg*, mcCarCfg*))(pSetCarCfg);
	minj_MakeCALL(loc_4AE100, (uintptr_t)&mcRandomizer_SetCarCfgHook);

	uintptr_t pOnLoadCareerDataDone = minj_GetBranchDestination(loc_1B0C20);
	OnLoadCareerDataDone = (uintptr_t(*)(uintptr_t, uintptr_t, const char*))(pOnLoadCareerDataDone);
	minj_MakeCALL(loc_1B0C20, (uintptr_t)&mcRandomizer_OnLoadCareerDataDone);

	//uintptr_t pOnSaveCareerDataDone = minj_GetBranchDestination(loc_1B0664);
	//OnSaveCareerDataDone = (uintptr_t(*)(uintptr_t, uintptr_t, const char*))(pOnSaveCareerDataDone);
	//minj_MakeCALL(loc_1B0664, (uintptr_t)&mcRandomizer_OnSaveCareerDataDone);

	uintptr_t pOnSaveData1 = minj_GetBranchDestination(loc_1AE8A0);
	OnSaveData_1 = (void(*)(uintptr_t, uintptr_t))(pOnSaveData1);
	minj_MakeCALL(loc_1AE8A0, (uintptr_t)&mcRandomizer_OnSaveData_Hook_1);

	uintptr_t pOnSaveData2 = minj_GetBranchDestination(loc_1AF4F8);
	OnSaveData_2 = (void(*)(uintptr_t, uintptr_t))(pOnSaveData2);
	minj_MakeCALL(loc_1AF4F8, (uintptr_t)&mcRandomizer_OnSaveData_Hook_2);

	uintptr_t pOnCreateSavegame = minj_GetBranchDestination(loc_1AF098);
	OnCreateSavegame = (void(*)(uintptr_t, int, const char*, uintptr_t, uintptr_t, int unk6))(pOnCreateSavegame);
	minj_MakeCALL(loc_1AF098, (uintptr_t)&mcRandomizer_OnCreateSavegame_Hook);

	uintptr_t pOnRaceOver = minj_GetBranchDestination(loc_3EE860);
	OnRaceOver = (void(*)(uintptr_t, const char*))(pOnRaceOver);
	minj_MakeCALL(loc_3EE860, (uintptr_t)&mcRandomizer_OnRaceOver_Hook);

	uintptr_t pOnRaceFinished = minj_GetBranchDestination(loc_3EDAC8);
	OnRaceFinished = (void(*)(uintptr_t))(pOnRaceFinished);
	minj_MakeCALL(loc_3EDAC8, (uintptr_t)&mcRandomizer_OnRaceFinished_Hook);

	//uintptr_t pGetVehicleID = minj_GetBranchDestination(loc_4AF878);
	//GetVehicleID = (int(*)(const char*))(pGetVehicleID);

	// #TODO check this, it causes a tlb miss...
	//sceCdCLOCK c;
	//if (sceCdReadClock(&c) & 1)
	//{
	//	time_t t = pack_sceCdCLOCK(&c);
	//	LOG("Init: Initializing the random seed with current time... %d\n", t);
	//	bRandom_SetSeed(t);
	//}
}
