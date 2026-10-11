#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_02230960(void *owner, void *data) {
    if (!BattleAnimUtil_UpdateLinearMotionSprite((u8 *)data + 8, PTR(data, 0x2C))) {
        Sprite_DeleteAndFreeResources(PTR(data, 0x2C));
        Heap_Free(data);
        BattleAnimSystem_DestroyTask(PTR(data, 0), owner);
    } else {
        SpriteSystem_DrawSprites(PTR(data, 4));
    }
}
