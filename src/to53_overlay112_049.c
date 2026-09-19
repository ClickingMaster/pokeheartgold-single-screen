#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


void ov112_021EA570(void *p) {
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E530), 0);
}

void ov112_021EA584(void *p) {
    int anim = ManagedSprite_GetActiveAnim(*(void **)((u8 *)p + 0x1E530));
    ManagedSprite_SetAnim(*(void **)((u8 *)p + 0x1E530), anim + 3);
}
