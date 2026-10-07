#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern int PMi_BlinkCounter[2];
void PM_SetLEDPattern(int pattern);
// @fn PM_SetLEDPattern
void PM_SetLEDPattern(int pattern){if(pattern<=15){PMi_BlinkCounter[1]=pattern;PMi_BlinkCounter[0]=0;}}
