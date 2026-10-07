#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define WL (*(u8 **)0x0380fff4)
#define R16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define R32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define P(p,o) (*(u8 **)((u8 *)(p)+(o)))

u32 AdjustRingPointer(u32 pointer);
// @fn AdjustRingPointer
u32 AdjustRingPointer(u32 pointer){if(pointer>=0x04805f60)pointer-=R16(WL,0x3de);return pointer;}
