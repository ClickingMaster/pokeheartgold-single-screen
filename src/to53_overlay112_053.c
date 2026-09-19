#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


int ov112_021EA9A0(void *entries) {
    int i = 0;
    u8 *entry = entries;
    while (i < 100) {
        if (entry[0] == 0) return i;
        i++;
        entry += 16;
    }
    return -1;
}

void ov112_021EA9BC(void *p, const s16 *entry) {
    if (entry[3] < 0) {
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E568), 1);
    } else {
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E564), 1);
    }
    ManagedSprite_SetAnimateFlag(*(void **)((u8 *)p + 0x1E568), 1);
    ManagedSprite_SetAnimateFlag(*(void **)((u8 *)p + 0x1E564), 1);
    ManagedSprite_SetAnim(*(void **)((u8 *)p + 0x1E568), 10);
    ManagedSprite_SetAnim(*(void **)((u8 *)p + 0x1E564), 24);
}

void ov112_021EAA10(void *p) {
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E568), 0);
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E564), 0);
}

void ov112_021EAA34(void *p, int x, int y) {
    if (y < 0) {
        ManagedSprite_SetPositionXYWithSubscreenOffset(*(void **)((u8 *)p + 0x1E568), (s16)x, (s16)(y + 208), 1 << 20);
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E568), 1);
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E564), 0);
    } else {
        ManagedSprite_SetPositionXY(*(void **)((u8 *)p + 0x1E564), (s16)x, (s16)y);
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E564), 1);
        ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E568), 0);
    }
}
