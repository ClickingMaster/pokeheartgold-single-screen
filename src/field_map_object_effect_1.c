#include "field_map_object_effect_internal.h"
typedef struct LocalFourWords {
    u32 v[4];
} LocalFourWords;
extern const u32 ov01_022091C0[], ov01_022091D4[];
LocalCtx *ov01_021FF464(void *owner) {
    LocalCtx *c = FieldEffectManager_HeapAllocAndFill(owner, 4, 0, 0);
    c->owner = owner;
    ov01_021FF490(c);
    return c;
}
void ov01_021FF480(LocalCtx *c) {
    ov01_021FF4D4(c);
    FieldEffectManager_HeapFree(c);
}
void ov01_021FF490(LocalCtx *c) {
    ov01_021F18D4(c->owner, 3, 0x77);
    ov01_021F1908(c->owner, 2, 0x90);
    FieldEffectManager_LoadTexResource(c->owner, 2, 0x12, 1);
    FieldEffectManager_AddRecord(c->owner, 3, 3, 2, 2, 0, ov01_022091D4);
}
void ov01_021FF4D4(LocalCtx *c) {
    ov01_021F18FC(c->owner, 3);
    ov01_021F1924(c->owner, 2);
    FieldEffectManager_FreeTexResource(c->owner, 2);
    FieldEffectManager_ReleaseRecord(c->owner, 3);
}
void ov01_021FF4FC(void *obj, void *arg) {
    struct {
        void *fieldSystem, *manager, *resource, *object;
    } local;
    void *manager = FieldEffectManager_GetFromMapObject(obj);
    local.fieldSystem = MapObject_GetFieldSystem(obj);
    local.manager = manager;
    local.resource = FieldEffectManager_GetRendererData(manager, 0xa);
    {
        VecFx32 pos = { 0, 0, 0 };
        local.object = obj;
        FieldEffectManager_CreateEffect(manager, ov01_022091C0, &pos, arg, &local, MapObject_GetPriorityPlusValue(obj, 2));
    }
}
BOOL ov01_021FF54C(void *task, LocalState *s) {
    VecFx32 pos = { 0, 0, 0 };
    *(LocalFourWords *)&s->data14 = *(LocalFourWords *)FieldEffect_GetUserData(task);
    s->sprite = MapObject_GetSpriteID(s->mapObject);
    s->id = MapObject_GetID(s->mapObject);
    s->mapId = MapObject_GetMapID(s->mapObject);
    FieldEffect_SetPosition(task, &pos);
    s->effect = FieldEffectManager_CreateBillboardFromRecord(s->resource, 3, &pos);
    if (MapObject_TestFlagsBits(s->mapObject, 2 << 8) == TRUE) {
        BillboardObject_SetVisible(s->effect, FALSE);
    }
    return TRUE;
}
void ov01_021FF5B8(void *task, LocalState *s) {
    BillboardObject_Delete(s->effect);
}
void ov01_021FF5C4(void *task, LocalState *s) {
    int mode;
    void *obj = s->mapObject;
    if (!sub_0205F0F8(obj, s->sprite, s->id, s->mapId)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    mode = FieldEffect_GetParam(task);
    if (mode == 1 && !MapObject_CheckFlag26(obj)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    if (MapObject_TestFlagsBits(obj, 2 << 8) == TRUE) {
        BillboardObject_SetVisible(s->effect, FALSE);
    } else {
        BillboardObject_SetVisible(s->effect, TRUE);
    }
    if (s->stage == 0) {
        BillboardObject_AdvanceFrame(s->effect, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(s->effect) / FX32_ONE >= 12) {
            if (mode == 0) {
                FieldEffectManager_DestroyEffect(task);
                return;
            }
            BillboardObject_SetFrame(s->effect, 0);
        }
    }
}
void ov01_021FF658(void *task, LocalState *s) {
    struct {
        VecFx32 facing, pos;
    } local;
    void *obj = s->mapObject;
    if (!sub_0205F0F8(obj, s->sprite, s->id, s->mapId)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    MapObject_CopyPositionVector(obj, &local.pos);
    MapObject_CopyFacingVector(obj, &local.facing);
    local.pos.x += local.facing.x;
    local.pos.z += local.facing.z;
    local.pos.z += 2 << 14;
    BillboardObject_SetPosition(s->effect, &local.pos);
}
