#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union {
    u16 raw;

    struct {
        u16 beacon : 1, broadcast : 1, preamble : 1, antenna : 1, diversity : 1, invert : 1, indicate : 1, nullkey : 1, rest : 8;
    } b;
} F35426;

typedef struct {
    u8 before[0x33a];
    F35426 flags;
    u8 after[0x500 - 0x33c];
} W35426;

#define G       (*(W35426 *volatile *)0x0380fff4)
#define H(p, n) (*(u16 *)((u8 *)(p) + (n)))
#define D(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define R(n)    (*(volatile u16 *)(n))
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern void __VENEER_MIi_CpuCopy16(const void *, void *, u32);
extern void WSetMacAdrs1(void *, const void *);
extern void WSetPowerMgtMode(u32);
extern void WSetTxTimeStampOffset(void);
extern void WSetFrameLifeTime(u32);
extern void WL_WriteByte(void *, u32);
extern void BBP_Write(u32, u32);
extern u32 WL_ReadByte(const void *);

u32 WSetSsidMask(const u16 *p);

u32 WSetSsidMask(const u16 *p) {
    u16 *d = (u16 *)((u8 *)G + 0x384);
    u32 i = 0;
    do {
        *d++ = *p++;
        i++;
    } while (i < 16);
    return 0;
}
