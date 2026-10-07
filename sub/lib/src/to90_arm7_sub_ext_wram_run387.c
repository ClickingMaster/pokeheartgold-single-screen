#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void WaitLoop_ClrAid(void);void WClearKSID(u32 owner);
// @fn WClearKSID
void WClearKSID(u32 owner){if(!(HW(0x04808094)&0x8000))WaitLoop_ClrAid();}
