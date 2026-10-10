#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef union {
    u16 raw;

    struct {
        u16 beacon : 1, broadcast : 1, preamble : 1, antenna : 1, diversity : 1, invert : 1, indicate : 1, nullkey : 1, rest : 8;
    } b;
} F35425;

typedef struct {
    u8 before[0x33a];
    F35425 flags;
    u8 after[0x500 - 0x33c];
} W35425;

#define G       (*(W35425 *volatile *)0x0380fff4)
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

u32 WSetActiveZoneTime(u16 x, u32 write);

u32 WSetActiveZoneTime(u16 x, u32 write) {
    u8 *p;
    W35425 *w;
    if (x < 10) {
        return 5;
    }
    H(G, 0x33c) = x;
    if (write) {
        R(0x04808134) = x;
    }
    w = G;
    if (H(w, 0x4a4)) {
        p = (u8 *)D(w, 0x4ac) + 0x24 + H(w, 0x3da);
        if (H(w, 0x352) == 1) {
            WL_WriteByte(p + 6, x & 255);
            WL_WriteByte(p + 7, (x >> 8) & 255);
        } else {
            WL_WriteByte(p + 6, 255);
            WL_WriteByte(p + 7, 255);
        }
    }
    return 0;
}
