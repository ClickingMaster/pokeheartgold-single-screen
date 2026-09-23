#include "field_overlay_effects_internal.h"
void ov01_021EFEC8(void *, s32, s32, s32, s32);
void ov01_021F0050(void *, void *, s32, s32, s32, s32, s32);
void ov01_021F0050(void *root, void *arg, s32 value, s32 duration, s32 start, s32 end, s32 middle) {
    u8 *p = arg;
    u8 *r = root;
    if (*(void **)(p + 0x24) != NULL) {
        GF_AssertFail();
    }
    *(s32 *)(r + 0x18) = 0;
    *(void **)(p + 0x20) = *(void **)((u8 *)*(void **)((u8 *)*(void **)(r + 0x10) + 4) + 0x1C);
    *(s32 *)(p + 0x18) = value;
    *(s32 *)(p + 0x1C) = 0;
    *(void **)(p + 0x2C) = r + 0x18;
    ov01_021EFEC8(p, start, end, middle, duration);
    if (start >= 0) {
        *(volatile u16 *)0x04000040 = 0xFF - start;
        *(volatile u16 *)0x04000044 = 0xC0;
        *(volatile u16 *)0x04000042 = (u8)(start + 1);
        *(volatile u16 *)0x04000046 = 0xC0;
    } else {
        *(volatile u16 *)0x04000040 = (u8)(start + 0xFF);
        *(volatile u16 *)0x04000044 = 0xC0;
        *(volatile u16 *)0x04000042 = (u8)(1 - start);
        *(volatile u16 *)0x04000046 = 0xC0;
    }
    SysTask_CreateOnVWaitQueue((void (*)(SysTask *, Interp *))ov01_021F00DC, p, 0x400);
}
