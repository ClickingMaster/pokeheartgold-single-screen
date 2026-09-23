#include "global.h"
extern void (*const ov93_02263114[])(void *, void *);
extern void MI_CpuFill8(void *, u8, u32);
u32 ov93_022627C0(void *p, u32 *e);
u32 ov93_022627C0(void *p, u32 *e) {
    if (ov93_02263114[*e] != 0) {
        ov93_02263114[*e](p, e);
        MI_CpuFill8(e, 0, 0x14);
        return 1;
    }
    return 0;
}
