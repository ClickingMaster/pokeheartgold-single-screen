#pragma force_active on
#include "global.h"
#include "sprite_system.h"
typedef struct Work {
    void *owner;
    u8 unk04[4];
    int paletteOffset;
    int b;
    ManagedSprite *sprite;
    u8 unk14[0x28];
} Work;
void *ov07_022324D8(void *owner, u32 size);
void ov07_0221C410(void *owner, void *func, void *data);
void ov07_02230FE8(void *task, Work *work);
void ov07_02230F98(void *owner, int a, int b, ManagedSprite *sprite);

// @fn ov07_02230F98
void ov07_02230F98(void *owner, int a, int b, ManagedSprite *sprite) {
    Work *work = ov07_022324D8(owner, sizeof(Work));
    work->owner = owner;
    work->b = b;
    work->sprite = sprite;
    ManagedSprite_SetDrawFlag(work->sprite, FALSE);
    ManagedSprite_SetOamMode(work->sprite, GX_OAM_MODE_XLU);
    ManagedSprite_SetDrawPriority(work->sprite, 100);
    ManagedSprite_SetPriority(work->sprite, 1);
    work->paletteOffset = ManagedSprite_GetPaletteOverrideOffset(work->sprite);
    ov07_0221C410(work->owner, ov07_02230FE8, work);
}
