#include "field_object_motion_effect_internal.h"
typedef struct LocalNineWords {
    u32 v[9];
} LocalNineWords;
extern const u32 ov01_02209258[];
LocalCtx *ov01_021FFC0C(void *owner) {
    LocalCtx *ctx = FieldEffectManager_HeapAllocAndFill(owner, 4, 0, 0);
    ctx->owner = owner;
    ov01_021FFC38(ctx);
    return ctx;
}

void ov01_021FFC28(LocalCtx *ctx) {
    ov01_021FFC80(ctx);
    FieldEffectManager_HeapFree(ctx);
}

void ov01_021FFC38(LocalCtx *ctx) {
    FieldEffectManager_LoadBillboardResource(ctx->owner, 8, 0x7b);
    FieldEffectManager_LoadBillboardSequence(ctx->owner, 8, 0x93);
    FieldEffectManager_LoadTexResource(ctx->owner, 9, 0x16, 1);
    FieldEffectManager_AddRecord(ctx->owner, 0xa, 8, 8, 9, 0, ov01_02209258);
}

void ov01_021FFC80(LocalCtx *ctx) {
    FieldEffectManager_FreeBillboardResource(ctx->owner, 8);
    FieldEffectManager_FreeBillboardSequence(ctx->owner, 8);
    FieldEffectManager_FreeTexResource(ctx->owner, 9);
    FieldEffectManager_ReleaseRecord(ctx->owner, 0xa);
}

BOOL ov01_021FFCA8(void *task, LocalState *s) {
    VecFx32 pos;
    *(LocalNineWords *)&s->x = *(LocalNineWords *)FieldEffect_GetUserData(task);
    s->sprite = MapObject_GetSpriteID(s->mapObject);
    s->id = MapObject_GetID(s->mapObject);
    s->mapId = MapObject_GetMapID(s->mapObject);
    pos.x = s->x << 16;
    pos.z = s->z << 16;
    pos.y = MapObject_GetPositionVectorYCoord(s->mapObject);
    s->onFloor = sub_0206121C(s->fieldSystem, &pos);
    pos.x += 0x1e << 10;
    pos.y -= 2 << 14;
    pos.z += 0xd << 12;
    FieldEffect_SetPosition(task, &pos);
    s->effect = FieldEffectManager_CreateBillboardFromRecord(s->resource, 0xa, &pos);
    if (MapObject_TestFlagsBits(s->mapObject, 2 << 8) == TRUE) {
        BillboardObject_SetVisible(s->effect, FALSE);
    }
    if (FieldEffect_GetParam(task) == 0) {
        BillboardObject_SetFrame(s->effect, 2 << 12);
        s->stage = 2;
    }
    return TRUE;
}

void ov01_021FFD64(void *task, LocalState *s) {
    BillboardObject_Delete(s->effect);
}

void ov01_021FFD70(void *task, LocalState *s) {
    struct {
        VecFx32 target, current;
    } local;
    void *obj = s->mapObject;
    int x, z;
    if (!sub_0205F0F8(obj, s->sprite, s->id, s->mapId)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    if (MapObject_TestFlagsBits(obj, 2 << 8) == TRUE) {
        BillboardObject_SetVisible(s->effect, FALSE);
    } else {
        BillboardObject_SetVisible(s->effect, TRUE);
    }
    if (s->onFloor == 0) {
        FieldEffect_GetPosition(task, &local.current);
        local.target.x = s->x << 16;
        local.target.z = s->z << 16;
        local.target.y = local.current.y;
        s->onFloor = sub_0206121C(s->fieldSystem, &local.target);
        if (s->onFloor == 1) {
            local.current.y = local.target.y;
            FieldEffect_SetPosition(task, &local.current);
        }
    }
    switch (s->stage) {
    case 0:
        BillboardObject_AdvanceFrame(s->effect, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(s->effect) / FX32_ONE >= 2) {
            s->stage = 1;
        }
        break;
    case 1:
        BillboardObject_SetFrame(s->effect, 2 * FX32_ONE);
        BillboardObject_AdvanceFrame(s->effect, 0);
        s->stage = 2;
    case 2:
        if (!sub_0205F0F8(obj, s->sprite, s->id, s->mapId)) {
            FieldEffectManager_DestroyEffect(task);
            return;
        }
        x = MapObject_GetXCoord(obj);
        z = MapObject_GetZCoord(obj);
        if (s->x != x || s->z != z) {
            FieldEffectManager_DestroyEffect(task);
            return;
        }
        if (s->facing != -1 && s->facing != MapObject_GetFacingDirection(obj)) {
            FieldEffectManager_DestroyEffect(task);
        }
        break;
    }
}

void ov01_021FFE98(void *task, LocalState *s) {
    VecFx32 pos;
    if (!sub_0205F0A8(s->mapObject, s->id, s->mapId)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    FieldEffect_GetPosition(task, &pos);
    BillboardObject_SetPosition(s->effect, &pos);
}
