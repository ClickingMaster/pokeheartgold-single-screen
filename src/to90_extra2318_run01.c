#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define U16(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
extern u8 ov13_0224F5C0[];
extern int ov13_0223E3A0(int);
int ov13_0223E308(void);
#pragma force_active on
#pragma opt_rebuildconditionals off

int ov13_0223E308(void) {
    U32(PTR(ov13_0224F5C0, 4), 0x12a4) = 0;
    U32(PTR(ov13_0224F5C0, 4), 0x12a0) = 0;
    U32(PTR(ov13_0224F5C0, 4), 0x48) = 0;
    U16(PTR(ov13_0224F5C0, 4), 0x50) = 0;
    U16(PTR(ov13_0224F5C0, 4), 0x52) = 1;
    U32(PTR(ov13_0224F5C0, 4), 0x54) = 0;
    U32(PTR(ov13_0224F5C0, 4), 0) = 0;
    U16(PTR(ov13_0224F5C0, 4), 4) = 0;
    U32(PTR(ov13_0224F5C0, 4), 0x4c) = 0;
    return ov13_0223E3A0(0) == 0 ? 0 : 1;
}
