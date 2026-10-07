#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern void PM_SetLEDPattern(int),PMi_SetLED(int);
void WVR_Shutdown(void);
// @fn WVR_Shutdown
void WVR_Shutdown(void){*(volatile u16*)0x04000304 &= ~2u;PM_SetLEDPattern(1);PMi_SetLED(1);}
