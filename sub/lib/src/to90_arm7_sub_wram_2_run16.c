#pragma force_active on
#include <nitro/types.h>

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int first, second;
} Cycles;

extern void MI_SetCartridgeRomCycle1st(u32);
extern void MI_SetCartridgeRomCycle2nd(u32);
void CTRDGi_ChangeLatestAccessCycle(Cycles *c);

void CTRDGi_ChangeLatestAccessCycle(Cycles *c) {
    c->first = (*(volatile u16 *)0x4000204 & 12) >> 2;
    c->second = (*(volatile u16 *)0x4000204 & 16) >> 4;
    MI_SetCartridgeRomCycle1st(3);
    MI_SetCartridgeRomCycle2nd(0);
}
