#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
struct State {
    void (*log)(int, char *, ...);
    u8 *work;
};
extern struct State ov13_0224F5C0;
extern char ov13_022472DC[], ov13_022472F4[], ov13_0224733C[];
extern void OS_Terminate(void);
extern u16 WM_GetDispersionBeaconPeriod(void);
void ov13_0223D6A8(u32);
int ov13_0223D744(void);
int ov13_0223E43C(u32 mode, u16 a, u16 b);
#define W ov13_0224F5C0.work
#define U32(o) (*(u32 *)(W + (o)))
#define U16(o) (*(u16 *)(W + (o)))

#pragma push
#pragma opt_propagation off
#pragma opt_dead_assignments off
// @fn ov13_0223E43C
int ov13_0223E43C(u32 mode, u16 a, u16 b) {
    (void)((U32(0x40) == 1) || (OS_Terminate(), 0));
    U32(0x12a4) = 0x180;
    U32(0x12a0) = 0xe0;
    if (ov13_0224F5C0.log) {
        ov13_0224F5C0.log(0x8000000, ov13_022472DC, U32(0x12a4));
    }
    if (ov13_0224F5C0.log) {
        ov13_0224F5C0.log(0x8000000, ov13_022472F4, U32(0x12a0));
    }
    U32(0x44) = mode;
    ov13_0223D6A8(3);
    U16(0xc) = a;
    U16(0x32) = b;
    U16(0x18) = WM_GetDispersionBeaconPeriod();
    U16(0x34) = 0xd0;
    U16(0x36) = 0x44;
    U16(0x10) = 2;
    U16(0x16) = 0;
    U16(0x12) = 0;
    U16(0xe) = 1;
    U16(0x14) = mode == 2 ? 1 : 0;
    if (mode == 0 || mode == 2 || mode == 4) {
        return ov13_0223D744();
    }
    if (ov13_0224F5C0.log) {
        ov13_0224F5C0.log(0x8000000, ov13_0224733C, mode);
    }
    return 0;
}
#pragma pop
