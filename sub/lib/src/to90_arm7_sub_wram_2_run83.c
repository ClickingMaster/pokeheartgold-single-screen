#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern int PMi_BlinkCounter[2];
int PM_GetLEDPattern(void);
// @fn PM_GetLEDPattern
int PM_GetLEDPattern(void){return PMi_BlinkCounter[1];}
