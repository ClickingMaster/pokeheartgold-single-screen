#include "overlay14_box_sprite_layouts_r51_internal.h"

#define WORK(p) PTR_AT((p), 0x34)
#define DEFINE_FUNC(name, xoff, yoff)                                                                            \
    void name(void *p) {                                                                                         \
        u32 count;                                                                                               \
        u32 i;                                                                                                   \
        const Ov14Pos *pos;                                                                                      \
        count = Party_GetCount(PTR_AT(p, 8));                                                                    \
        pos = ov14_021F80BC;                                                                                     \
        for (i = 0; i < 6; i++, pos++) {                                                                         \
            u8 slot = U8_AT(WORK(p), 0x40b2 + i);                                                                \
            ManagedSprite_SetPositionXY(((Ov14Work *)WORK(p))->sprites[slot], pos->x + (xoff), pos->y + (yoff)); \
            ov14_021F3190(WORK(p), i + 0x1e, 1);                                                                 \
            if (i < count) {                                                                                     \
                ov14_021F2A18(WORK(p), slot, 1);                                                                 \
            } else {                                                                                             \
                ov14_021F2A18(WORK(p), slot, 0);                                                                 \
            }                                                                                                    \
        }                                                                                                        \
    }

DEFINE_FUNC(ov14_021F2FDC, 0, 0)
DEFINE_FUNC(ov14_021F3044, 0x98, 0)
DEFINE_FUNC(ov14_021F30B0, 0, -0x90)
DEFINE_FUNC(ov14_021F311C, 0x98, -0x90)
