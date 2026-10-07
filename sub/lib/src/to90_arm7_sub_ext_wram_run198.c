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
extern u32 CheckKeyTxEndMain(void *);
u32 CheckKeyTxEnd(void);
// @fn CheckKeyTxEnd
u32 CheckKeyTxEnd(void){u8 *slots=WL+0x42c;u32 result=CheckKeyTxEndMain(slots+0x50);return result|CheckKeyTxEndMain(slots+0x64);}
