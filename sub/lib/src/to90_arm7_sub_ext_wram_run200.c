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
extern void WlIntr(void),__VENEER_OS_SetIrqFunction(u32,void(*)(void));extern u32 __VENEER_OS_EnableIrqMask(u32);
void InitializeIntr(void);
// @fn InitializeIntr
void InitializeIntr(void){__VENEER_OS_SetIrqFunction(0x1000000,WlIntr);__VENEER_OS_EnableIrqMask(0x1000000);}
