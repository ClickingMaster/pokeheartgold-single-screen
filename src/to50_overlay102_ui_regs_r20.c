#include "to50_overlay102_ui_regs_r20_internal.h"
#include <nitro/gx/gx.h>
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define U16(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define S16(p,o) (*(s16 *)((u8 *)(p)+(o)))

void ov102_021EA644(u8 *work) {
    void *bg = ov102_021EA268(PTR(work, 0));
    if (GX_GetVCount() < 0x30) {
        BgSetPosTextAndCommit(bg, 0, 3, S16(work, 0x1E0));
    } else {
        BgSetPosTextAndCommit(bg, 0, 3, 0);
    }
}

void ov102_021EA678(void *unused, BOOL enable) {
#pragma unused(unused)
    volatile u16 *reg48 = (volatile u16 *)0x04000048;
    volatile u16 *reg4A = (volatile u16 *)0x0400004A;
    *reg4A = (*reg4A & ~0x3F) | 0x1F;
    if (enable) {
        *reg48 = (*reg48 & ~0x3F) | 0x1A;
        *reg48 = (*reg48 & ~0x3F00) | 0x1B00;
        *(volatile u16 *)0x04000040 = 0xFF;
        *(volatile u16 *)0x04000044 = 0x30;
        *(volatile u16 *)0x04000042 = 0xFF;
        *(volatile u16 *)0x04000046 = 0xA0C0;
        *(volatile u32 *)0x04000000 = (*(volatile u32 *)0x04000000 & 0xFFFF1FFF) | 0x6000;
    } else {
        *reg48 = (*reg48 & ~0x3F) | 0x1F;
        *reg48 = (*reg48 & ~0x3F00) | 0x1F00;
        *(volatile u16 *)0x04000040 = 0;
        *(volatile u16 *)0x04000044 = 0;
        *(volatile u16 *)0x04000042 = 0;
        *(volatile u16 *)0x04000046 = 0;
        *(volatile u32 *)0x04000000 &= 0xFFFF1FFF;
    }
}
