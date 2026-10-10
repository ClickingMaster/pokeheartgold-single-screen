#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern int PMi_BlinkCounter[2];
void PM_SetLEDPattern(int pattern);

void PM_SetLEDPattern(int pattern) {
    if (pattern <= 15) {
        PMi_BlinkCounter[1] = pattern;
        PMi_BlinkCounter[0] = 0;
    }
}
