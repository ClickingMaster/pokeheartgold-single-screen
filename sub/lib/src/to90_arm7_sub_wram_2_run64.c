#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern void PM_SetLEDPattern(int);
extern void PMi_SetLED(int);
void WVR_Shutdown(void);

void WVR_Shutdown(void) {
    *(volatile u16 *)0x04000304 &= ~2u;
    PM_SetLEDPattern(1);
    PMi_SetLED(1);
}
