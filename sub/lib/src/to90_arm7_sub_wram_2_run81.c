#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define H(a) (*(volatile u16 *)(a))
#define W(a) (*(volatile u32 *)(a))
extern void PMi_SetControl(u32);
extern void PMi_ResetControl(u32);
extern void OS_Terminate(void);
extern int PMi_LEDStatus;
void PMi_SetLED(int state);

void PMi_SetLED(int state) {
    switch (state) {
    case 1:
        PMi_ResetControl(16);
        break;
    case 3:
        PMi_SetControl(48);
        break;
    case 2:
        PMi_ResetControl(32);
        PMi_SetControl(16);
        break;
    default:
        OS_Terminate();
        break;
    }
    PMi_LEDStatus = state;
}
