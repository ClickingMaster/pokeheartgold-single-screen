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
void WEnableTmpttPowerSave(void);
// @fn WEnableTmpttPowerSave
void WEnableTmpttPowerSave(void){H(BASE,0x3ea)=0;HW(0x04808038)|=2;}
