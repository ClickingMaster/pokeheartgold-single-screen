#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define G       (*(u8 **)0x0380fff4)
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define P(p, o) (*(void **)((u8 *)(p) + (o)))
#define V(a)    (*(volatile u16 *)(a))
extern void __VENEER_MIi_CpuClear16(u32, void *, u32);
extern void CAM_SetPowerMgtMode(u32, u16);
extern void CAM_SetStaState(u32, u32);
extern void WSetMacAdrs1(u16 *, const u16 *);
/* Selected station storage is sixteen28-byte records; index names a valid record. */
void CAM_InitElement(u32 index, const u8 *addr);

void CAM_InitElement(u32 index, const u8 *addr) {
    u8 *g = G, *cam = (u8 *)P(g, 0x31c) + index * 28;
    if (!H(cam, 0)) {
        H(g, 0x52c)
        ++;
    }
    __VENEER_MIi_CpuClear16(0, cam, 26);
    g = G;
    H(g, 0x534) &= ~(1U << index);
    CAM_SetPowerMgtMode((u16)index, 0);
    g = G;
    H(g, 0x530) |= (1U << index);
    WSetMacAdrs1((u16 *)(cam + 4), (const u16 *)addr);
    H(cam, 0x14) = 0xffff;
    H(cam, 0x10) = H(G, 0x3a6);
    H(cam, 0x18) = H(cam, 0x1a);
    CAM_SetStaState(index & 65535U, 0x20);
}
