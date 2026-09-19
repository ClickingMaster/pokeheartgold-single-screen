#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


void ov112_021EA64C(void *p) {
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E534), 0);
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E538), 0);
}

void ov112_021EA670(void *p, int index) {
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E530 + index * 4), 1);
}

void ov112_021EA688(void *p, int index) {
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E530 + index * 4), 0);
}

void ov112_021EA6A0(void *p, int index, int anim) {
    ManagedSprite_SetAnim(*(void **)((u8 *)p + 0x1E530 + index * 4), anim);
}

void ov112_021EA6B8(void *p, int index, int x, int y) {
    ManagedSprite_SetPositionXYWithSubscreenOffset(*(void **)((u8 *)p + 0x1E530 + index * 4), (s16)x, (s16)y, 1 << 20);
}
