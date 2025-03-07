//
// Minjector - minimal MIPS injector library
// by xan1242 / Tenjoin
//
// Portions taken from injector at ThirteenAG/WidescreenFixesPack
//

#include <stdint.h>
#include "minjector.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
//#define MAX(a,b) (((a)>(b))?(a):(b))

uintptr_t _minj_base_addr = 0;
size_t _minj_base_size = 0;

uintptr_t _minj_mod_addr = 0;
size_t _minj_mod_size = 0;

#ifndef MINJECTOR_MINIMAL
void* (*minj_memset)(void* ptr, int value, size_t num) = (void* (*)(void*, int, size_t))(0);
void* (*minj_memcpy)(void* dst, const void* src, size_t num) = (void* (*)(void*, const void*, size_t))(0);
#endif

void minj_SetModBaseAddress(uintptr_t addr, size_t size)
{
    _minj_mod_addr = addr;
    _minj_mod_size = size;
}

void minj_SetBaseAddress(uintptr_t addr, size_t size)
{
    _minj_base_addr = addr;
    _minj_base_size = size;
}

uintptr_t minj_GetModBaseAddress()
{
    return _minj_mod_addr;
}

size_t minj_GetModBaseSize()
{
    return _minj_mod_size;
}

uintptr_t minj_GetBaseAddress()
{
    return _minj_base_addr;
}

size_t minj_GetBaseSize()
{
    return _minj_base_size;
}

void minj_SetGameBaseAddress(uintptr_t addr, size_t size)
{
    _minj_base_addr = addr;
    _minj_base_size = size;
}


uintptr_t minj_adjustAddress(uintptr_t addr)
{
#ifdef MINJECTOR_NO_ADJUST
    return addr;
#else
    if (_minj_mod_addr == 0)
        return addr;

    if (addr >= _minj_base_addr && addr < _minj_base_addr + _minj_base_size)
        return addr;
    else if (addr >= _minj_mod_addr && addr < _minj_mod_addr + _minj_mod_size)
        return addr;   

    uintptr_t m = MIN(_minj_base_addr, _minj_mod_addr);
    if (addr >= m)
        return addr;
    else
        return addr + _minj_base_addr;
#endif
}

#ifndef MINJECTOR_MINIMAL
void minj_SetMemsetAddress(uintptr_t addr)
{
    minj_memset = (void* (*)(void*, int, size_t))(minj_adjustAddress(addr));
}

void minj_SetMemcpyAddress(uintptr_t addr)
{
    minj_memcpy = (void* (*)(void*, const void*, size_t))(minj_adjustAddress(addr));
}


uintptr_t minj_GetGP()
{
    uintptr_t gp;
    asm(
        "move %0, $gp\n"
        : "=r"(gp)
    );
    return gp;
}


void minj_WriteMemoryRaw(uintptr_t addr, void* value, size_t size)
{
    minj_memcpy((void*)minj_adjustAddress(addr), value, size);
}

void minj_ReadMemoryRaw(uintptr_t addr, void* ret, size_t size)
{
    minj_memcpy(ret, (void*)minj_adjustAddress(addr), size);
}

void minj_MemoryFill(uintptr_t addr, uint8_t value, size_t size)
{
    minj_memset((void*)minj_adjustAddress(addr), value, size);
}
#endif
void minj_WriteMemory8(uintptr_t addr, uint8_t value)
{
    *(uint8_t*)(minj_adjustAddress(addr)) = value;
}

void minj_WriteMemory16(uintptr_t addr, uint16_t value)
{
    *(uint16_t*)(minj_adjustAddress(addr)) = value;
}

void minj_WriteMemory32(uintptr_t addr, uint32_t value)
{
    *(uint32_t*)(minj_adjustAddress(addr)) = value;
}
#ifndef MINJECTOR_MINIMAL
void minj_WriteMemory64(uintptr_t addr, uint64_t value)
{
    *(uint64_t*)(minj_adjustAddress(addr)) = value;
}

void minj_WriteMemoryFloat(uintptr_t addr, float value)
{
    *(float*)(minj_adjustAddress(addr)) = value;
}

void minj_WriteMemoryDouble(uintptr_t addr, double value)
{
    *(double*)(minj_adjustAddress(addr)) = value;
}

uint8_t minj_ReadMemory8(uintptr_t addr)
{
    return *(uint8_t*)(minj_adjustAddress(addr));
}

uint16_t minj_ReadMemory16(uintptr_t addr)
{
    return *(uint16_t*)(minj_adjustAddress(addr));
}

uint32_t minj_ReadMemory32(uintptr_t addr)
{
    return *(uint32_t*)(minj_adjustAddress(addr));
}

uint64_t minj_ReadMemory64(uintptr_t addr)
{
    return *(uint64_t*)(minj_adjustAddress(addr));
}

float minj_ReadMemoryFloat(uintptr_t addr)
{
    return *(float*)(minj_adjustAddress(addr));
}

double minj_ReadMemoryDouble(uintptr_t addr)
{
    return *(double*)(minj_adjustAddress(addr));
}
#endif
void minj_MakeNOP(uintptr_t at)
{
#ifndef MINJECTOR_MINIMAL
    minj_MemoryFill(minj_adjustAddress(at), 0x00, 4);
#else
    minj_WriteMemory32(at, 0);
#endif
}

void minj_MakeNOPCount(uintptr_t at, uint32_t count)
{
#ifndef MINJECTOR_MINIMAL
    minj_MemoryFill(minj_adjustAddress(at), 0x00, sizeof(uint32_t) * count);
#else
    for (int i = 0; i < count; i++)
    {
        minj_WriteMemory32(at + (sizeof(uint32_t) * i), 0);
    }
#endif
}

void minj_MakeJMP(uintptr_t at, uintptr_t dest)
{
    minj_WriteMemory32(at, (0x08000000 | ((minj_adjustAddress(dest) & 0x0FFFFFFC) >> 2)));
}

void minj_MakeJMPwNOP(uintptr_t at, uintptr_t dest)
{
    minj_MakeJMP(at, dest);
    minj_MakeNOP(at + 4);
}

void minj_MakeCALL(uintptr_t at, uintptr_t dest)
{
    minj_WriteMemory32(minj_adjustAddress(at), (0x0C000000 | (((minj_adjustAddress(dest)) >> 2) & 0x03FFFFFF)));
}

uintptr_t minj_GetBranchDestination(uintptr_t at)
{
    return (*(uint32_t*)at & 0x03FFFFFF) << 2;
}

//
// Discovers the value of the given register LUI & ADDIU pair found within first 10 instructions
// start = address of first lui
// outptrLUI = out value of a LUI ptr
// outptrADDIU = out value of a ADDIU ptr
//
uintptr_t minj_DiscoverPtr(uintptr_t start, uintptr_t* outptrLUI, uintptr_t* outptrADDIU, MIPSRegister register_num)
{
    uintptr_t ptrLUI = 0;
    uintptr_t ptrADD = start;

    uint32_t in_insLUI = 0x3C000000 | (register_num << 16);
    uint32_t in_insADDIU = 0x24000000 | (register_num << 16) | (register_num << 21);

    // find the first LUI after ADDIU
    for (int i = 0; i < 10; i++)
    {
        uint32_t ins = *(uint32_t*)(ptrADD + (4 * i)) & 0xFFFF0000;
        if (ins == in_insLUI)
        {
            ptrLUI = ptrADD + (4 * i);
            break;
        }
    }

    if (ptrLUI == 0)
        return 0;


    ptrADD = 0;

    // find the first ADDIU after LUI
    for (int i = 0; i < 10; i++)
    {
        uint32_t ins = *(uint32_t*)(ptrLUI + (4 * i)) & 0xFFFF0000;
        if (ins == in_insADDIU)
        {
            ptrADD = ptrLUI + (4 * i);
            break;
        }
    }

    if (ptrADD == 0)
        return 0;

    uint32_t insLUI = *(uint32_t*)ptrLUI;
    uint32_t insADD = *(uint32_t*)ptrADD;

    // construct the ptr from the instructions
    uint32_t part1 = (insLUI & 0xFFFF);
    uint32_t part2 = (insADD & 0xFFFF);

    if (part2 > 0x7FFF)
        part1 -= 1;
    part1 <<= 16;

    uintptr_t retVal = part1 | part2;

    if (outptrLUI)
        *outptrLUI = ptrLUI;
    if (outptrADDIU)
        *outptrADDIU = ptrADD;

    return retVal;
}

void minj_CalcNewRegValue(uint32_t val, uint32_t* outLUI, uint32_t* outADDIU, MIPSRegister register_num)
{
    uint32_t part2 = val & 0xFFFF;
    uint32_t part1 = (val >> 16) & 0xFFFF;

    if (part2 > 0x7FFF)
        part1 += 1;

    *outLUI = (0x3C000000 | (register_num << 16) | part1);
    *outADDIU = (0x24000000 | (register_num << 16) | (register_num << 21) | part2);
}

//
// Writes a new value for a given LUI & ADDIU pair found within 10 instructions
// at = address of first lui
//
uintptr_t minj_WriteLUI_ADDIU(uintptr_t at, uint32_t value, MIPSRegister reg)
{
    uintptr_t ptrLUI = 0;
    uintptr_t ptrADDIU = 0;

    uintptr_t oldvalue = minj_DiscoverPtr(minj_adjustAddress(at), &ptrLUI, &ptrADDIU, reg);

    uint32_t newLUI = 0;
    uint32_t newADDIU = 0;

    minj_CalcNewRegValue(value, &newLUI, &newADDIU, reg);

    *(uint32_t*)(ptrLUI) = newLUI;
    *(uint32_t*)(ptrADDIU) = newADDIU;

    return oldvalue;
}
