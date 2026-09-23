#include "global.h"
extern void MI_CpuCopy8(const void *, void *, u32);
void ov13_02222BC0(const void *src, void *dst);
void ov13_02222BC0(const void *src, void *dst) {
    u8 *p = dst;
    MIi_CpuClear16(0, (u16 *)(p + 0xC), 0x20);
    *(u16 *)(p + 0xA) = *(u32 *)src;
    MI_CpuCopy8((const u8 *)src + 4, p + 0xC, *(u16 *)(p + 0xA));
}
