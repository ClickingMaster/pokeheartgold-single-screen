#include "to53_overlay112_w11_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))


void ov112_021EA4F4(void *p) {
    ManagedSprite_SetAnimateFlag(*(void **)((u8 *)p + 0x1E530), 1);
    ManagedSprite_SetAnim(*(void **)((u8 *)p + 0x1E530), 8);
    ManagedSprite_SetDrawFlag(*(void **)((u8 *)p + 0x1E530), 0);
}
