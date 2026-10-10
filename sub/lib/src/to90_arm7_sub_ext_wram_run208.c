#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a)      (*(volatile u16 *)(a))
#define WL        (*(u8 **)0x0380fff4)
#define R16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define R32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o)   (*(u8 **)((u8 *)(p) + (o)))

void CAM_SetRSSI(u32 index, u32 value);

void CAM_SetRSSI(u32 index, u32 value) {
    u8 *entry = P(WL, 0x31c) + index * 28;
    R16(entry, 0xa) = (u16)value;
}
