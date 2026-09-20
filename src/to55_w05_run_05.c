#include "to55_w05_private.h"
#define W(p, o)  (*(u32 *)((u8 *)(p) + (o)))
#define SW(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define H(p, o)  (*(s16 *)((u8 *)(p) + (o)))
#define P(p, o)  (*(void **)((u8 *)(p) + (o)))

void ov07_02230058(void *owner, void *p) {
    int i;
    u8 *q;
    if (ov07_0222FFD0(p)) {
        i = 0;
        q = p;
        do {
            Sprite_DeleteAndFreeResources(P(q, 0x18));
            i++;
            q += 4;
        } while (i < 15);
        Heap_Free(p);
        ov07_0221C448(P(p, 0), owner);
    } else {
        SpriteSystem_DrawSprites(P(p, 8));
    }
}
