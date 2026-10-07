#include "global.h"
void *ov07_022324D8(void *, int);
void ov07_02231FE4(void *, void *);

extern u32 ov07_0221C4A8(void *p, int i);
void ManagedSprite_OffsetPositionXY(void *, int, int);
void ov07_0221C410(void *, void *, void *);
void ov07_02226804(void);
void ov07_02226838(void *, int, int, void *);

void ov07_02226838(void *context, int ignored1, int ignored2, void *sprite) {
    u8 *work = ov07_022324D8(context, 0x20);
    ov07_02231FE4(context, work);
    *(void **)(work + 0x1C) = sprite;
    s16 x = ((s32)ov07_0221C4A8(context, 0));
    s16 y = ((s32)ov07_0221C4A8(context, 1));
    ManagedSprite_OffsetPositionXY(sprite, x, y);
    ov07_0221C410(*(void **)(work + 4), (void *)ov07_02226804, work);
}
