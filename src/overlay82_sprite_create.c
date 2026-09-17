#include <string.h>

#include "overlay82_sprite_create_private.h"
Ov82Spr *ov82_0223FC48(void *work, u32 resource, u32 sequence, s32 x, u16 y, const u8 *animData) {
    Ov82Spr *result = Heap_Alloc(HEAP_ID_105, sizeof(Ov82Spr));
    VecFx32 position;
    memset(result, 0, sizeof(Ov82Spr));
    result->animData = animData;
    result->sprite = ov82_0223FB08(work, resource, sequence, 0, 0);
    position.x = x << FX32_SHIFT;
    position.y = y << FX32_SHIFT;
    Sprite_SetMatrix(result->sprite, &position);
    return result;
}
