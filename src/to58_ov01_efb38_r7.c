#include "field_overlay_effects_internal.h"
void ov01_021EFEC8(void *, s32, s32, s32, s32);
void ov01_021F02C4(void *, void *, s32, s32, s32);
void ov01_021F02C4(void *root, void *arg, s32 duration, s32 value, s32 extra) {
    u8 *p = arg;
    u8 *r = root;
    if (*(void **)(p + 0x3C) != NULL) {
        GF_AssertFail();
    }
    *(s32 *)(r + 0x18) = 0;
    *(void **)(p + 0x38) = *(void **)((u8 *)*(void **)((u8 *)*(void **)(r + 0x10) + 4) + 0x1C);
    *(s32 *)(p + 0x30) = 0x60;
    *(s32 *)(p + 0x34) = 0;
    *(void **)(p + 0x44) = r + 0x18;
    ov01_021EFEC8(p, 0, 0xFF000, value, duration);
    ov01_021EFEC8(p + 0x18, 0, 6 << 16, extra, duration);
    *(volatile u16 *)0x04000040 = 0xFF;
    *(volatile u16 *)0x04000044 = 0xC0;
    *(volatile u16 *)0x04000042 = 0xFF;
    *(volatile u16 *)0x04000046 = 0xC0;
    SysTask_CreateOnVWaitQueue((void (*)(SysTask *, Interp *))ov01_021F0330, p, 0x400);
}
