//
// Minjector - minimal MIPS injector library
// by xan1242 / Tenjoin
//
// Portions taken from injector at ThirteenAG/WidescreenFixesPack
//

#include <stddef.h>
#include <stdint.h>

typedef enum _MIPSRegister {
    MIPSR_r0 = 0,
    MIPSR_r1,
    MIPSR_r2,
    MIPSR_r3,
    MIPSR_r4,
    MIPSR_r5,
    MIPSR_r6,
    MIPSR_r7,
    MIPSR_r8,
    MIPSR_r9,
    MIPSR_r10,
    MIPSR_r11,
    MIPSR_r12,
    MIPSR_r13,
    MIPSR_r14,
    MIPSR_r15,
    MIPSR_r16,
    MIPSR_r17,
    MIPSR_r18,
    MIPSR_r19,
    MIPSR_r20,
    MIPSR_r21,
    MIPSR_r22,
    MIPSR_r23,
    MIPSR_r24,
    MIPSR_r25,
    MIPSR_r26,
    MIPSR_r27,
    MIPSR_r28,
    MIPSR_r29,
    MIPSR_r30,
    MIPSR_r31,
    MIPSR_zero = MIPSR_r0,
    MIPSR_at = MIPSR_r1,
    MIPSR_v0 = MIPSR_r2,
    MIPSR_v1 = MIPSR_r3,
    MIPSR_a0 = MIPSR_r4,
    MIPSR_a1 = MIPSR_r5,
    MIPSR_a2 = MIPSR_r6,
    MIPSR_a3 = MIPSR_r7,
    MIPSR_t0 = MIPSR_r8,
    MIPSR_t1 = MIPSR_r9,
    MIPSR_t2 = MIPSR_r10,
    MIPSR_t3 = MIPSR_r11,
    MIPSR_t4 = MIPSR_r12,
    MIPSR_t5 = MIPSR_r13,
    MIPSR_t6 = MIPSR_r14,
    MIPSR_t7 = MIPSR_r15,
    MIPSR_s0 = MIPSR_r16,
    MIPSR_s1 = MIPSR_r17,
    MIPSR_s2 = MIPSR_r18,
    MIPSR_s3 = MIPSR_r19,
    MIPSR_s4 = MIPSR_r20,
    MIPSR_s5 = MIPSR_r21,
    MIPSR_s6 = MIPSR_r22,
    MIPSR_s7 = MIPSR_r23,
    MIPSR_t8 = MIPSR_r24,
    MIPSR_t9 = MIPSR_r25,
    MIPSR_k0 = MIPSR_r26,
    MIPSR_k1 = MIPSR_r27,
    MIPSR_gp = MIPSR_r28,
    MIPSR_sp = MIPSR_r29,
    MIPSR_fp = MIPSR_r30,
    MIPSR_ra = MIPSR_r31,
    MIPSR_InvalidGPRReg = -1,
} MIPSRegister;

void minj_SetModBaseAddress(uintptr_t addr, size_t size);
void minj_SetBaseAddress(uintptr_t addr, size_t size);
uintptr_t minj_GetModBaseAddress();
size_t minj_GetModBaseSize();
uintptr_t minj_GetBaseAddress();
size_t minj_GetBaseSize();
void minj_SetGameBaseAddress(uintptr_t addr, size_t size);
uintptr_t minj_adjustAddress(uintptr_t addr);

void minj_WriteMemory8(uintptr_t addr, uint8_t value);
void minj_WriteMemory16(uintptr_t addr, uint16_t value);
void minj_WriteMemory32(uintptr_t addr, uint32_t value);
void minj_MakeNOP(uintptr_t at);
void minj_MakeNOPCount(uintptr_t at, uint32_t count);
void minj_MakeJMP(uintptr_t at, uintptr_t dest);
void minj_MakeJMPwNOP(uintptr_t at, uintptr_t dest);
void minj_MakeCALL(uintptr_t at, uintptr_t dest);
uintptr_t minj_GetBranchDestination(uintptr_t at);
uintptr_t minj_DiscoverPtr(uintptr_t start, uintptr_t* outptrLUI, uintptr_t* outptrADDIU, MIPSRegister register_num);
uintptr_t minj_WriteLUI_ADDIU(uintptr_t at, uint32_t value, MIPSRegister reg);

#ifndef MINJECTOR_MINIMAL
void minj_SetMemsetAddress(uintptr_t addr);
void minj_SetMemcpyAddress(uintptr_t addr);
uintptr_t minj_GetGP();
void minj_WriteMemoryRaw(uintptr_t addr, void* value, size_t size);
void minj_ReadMemoryRaw(uintptr_t addr, void* ret, size_t size);
void minj_MemoryFill(uintptr_t addr, uint8_t value, size_t size);
void minj_WriteMemory64(uintptr_t addr, uint64_t value);
void minj_WriteMemoryFloat(uintptr_t addr, float value);
void minj_WriteMemoryDouble(uintptr_t addr, double value);
uint8_t minj_ReadMemory8(uintptr_t addr);
uint16_t minj_ReadMemory16(uintptr_t addr);
uint32_t minj_ReadMemory32(uintptr_t addr);
uint64_t minj_ReadMemory64(uintptr_t addr);
float minj_ReadMemoryFloat(uintptr_t addr);
double minj_ReadMemoryDouble(uintptr_t addr);
#endif
