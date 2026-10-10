#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
u16 GetRomValidLanguage(void);
extern u32 PMi_Initialized;
extern void PM_SelfBlinkProc(void);
void VBlankIntr(void);
#pragma opt_propagation off

u16 GetRomValidLanguage(void) {
    u16 result = 0;
    u8 code = *(volatile u8 *)0x027ffe1d;
    if (code == 0x80) {
        result |= 0x40;
        return result;
    }
    if (code == 0x40) {
        result |= 0x80;
    }
    return result;
}

#pragma opt_propagation on

void VBlankIntr(void) {
    if (PMi_Initialized) {
        PM_SelfBlinkProc();
    }
}
