#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern int PMi_BlinkCounter[2];
int PM_GetLEDPattern(void);

int PM_GetLEDPattern(void) {
    return PMi_BlinkCounter[1];
}
