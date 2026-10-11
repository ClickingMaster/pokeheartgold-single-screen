#include "field_follow_effect_internal.h"

void ov01_022037E8(void *task, void *v) {
    u8 pos[12];
    u8 *p = v;
    if (!sub_0205F0F8(*(void **)(p + 0x30), *(u32 *)(p + 4), *(u32 *)(p + 8), *(u32 *)(p + 0xc))) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    FieldEffect_GetPosition(task, pos);
    Field3dObject_Draw(*(u8 **)(p + 0x2c) + 0x3c);
}

BOOL ov01_02203820(void *task, void *v) {
    u8 *p = v;
    Vec3 pos;
    Copy36 *src = FieldEffect_GetUserData(task);
    *(u32 *)p = 0;
    *(Copy36 *)(p + 0x18) = *src;
    *(u32 *)(p + 4) = MapObject_GetSpriteID(*(void **)(p + 0x30));
    *(u32 *)(p + 8) = MapObject_GetID(*(void **)(p + 0x30));
    *(u32 *)(p + 0xc) = MapObject_GetMapID(*(void **)(p + 0x30));
    pos = *(Vec3 *)(p + 0x18);
    FieldEffect_SetPosition(task, &pos);
    Field3dObject_SetPos(*(u8 **)(p + 0x2c) + 0x3c, &pos);
    Field3dObject_SetActiveFlag(*(u8 **)(p + 0x2c) + 0x3c, FALSE);
    return TRUE;
}
