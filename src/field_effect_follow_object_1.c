#include "field_effect_follow_object_internal.h"

Ov01FieldEffectCtx *ov01_021FEEEC(FieldEffectManager *manager) {
    Ov01FieldEffectCtx *ctx = (Ov01FieldEffectCtx *)FieldEffectManager_HeapAllocAndFill(manager, 4, 0, 0);
    ctx->manager = manager;
    ov01_021FEF18(ctx);
    return ctx;
}

void ov01_021FEF08(Ov01FieldEffectCtx *ctx) {
    ov01_021FEFF8(ctx);
    FieldEffectManager_HeapFree(ctx);
}

void ov01_021FEF18(Ov01FieldEffectCtx *ctx) {
    ov01_021F18D4(ctx->manager, 0, 0x7e);
    ov01_021F18D4(ctx->manager, 5, 0x7f);
    ov01_021F1908(ctx->manager, 0, 0x8c);
    ov01_021F1908(ctx->manager, 3, 0x8d);
    ov01_021F1908(ctx->manager, 4, 0x8e);
    ov01_021F1908(ctx->manager, 5, 0x8f);
    FieldEffectManager_LoadTexResource(ctx->manager, 0, 0x19, 1);
    FieldEffectManager_LoadTexResource(ctx->manager, 4, 0x10, 1);
    FieldEffectManager_LoadTexResource(ctx->manager, 5, 0, 1);
    FieldEffectManager_LoadTexResource(ctx->manager, 6, 1, 1);
    FieldEffectManager_AddRecord(ctx->manager, 0, 0, 0, 0, 0, &ov01_02209178);
    FieldEffectManager_AddRecord(ctx->manager, 5, 5, 3, 4, 0, &ov01_02209190);
    FieldEffectManager_AddRecord(ctx->manager, 6, 5, 4, 5, 0, &ov01_02209160);
    FieldEffectManager_AddRecord(ctx->manager, 7, 5, 5, 6, 0, &ov01_022091A8);
}

void ov01_021FEFF8(Ov01FieldEffectCtx *ctx) {
    ov01_021F18FC(ctx->manager, 0);
    ov01_021F18FC(ctx->manager, 5);
    ov01_021F1924(ctx->manager, 0);
    ov01_021F1924(ctx->manager, 3);
    ov01_021F1924(ctx->manager, 4);
    ov01_021F1924(ctx->manager, 5);
    FieldEffectManager_FreeTexResource(ctx->manager, 0);
    FieldEffectManager_FreeTexResource(ctx->manager, 4);
    FieldEffectManager_FreeTexResource(ctx->manager, 5);
    FieldEffectManager_FreeTexResource(ctx->manager, 6);
    FieldEffectManager_ReleaseRecord(ctx->manager, 0);
    FieldEffectManager_ReleaseRecord(ctx->manager, 5);
    FieldEffectManager_ReleaseRecord(ctx->manager, 6);
    FieldEffectManager_ReleaseRecord(ctx->manager, 7);
}

void ov01_021FF070(LocalMapObject *object, int a1) {
    VecFx32 pos;
    FieldEffectFollowArgs args;
    FieldSystem *fieldSys = FieldEffectManager_GetFromMapObject(object);

    args.x = MapObject_GetXCoord(object);
    args.y = MapObject_GetYCoord(object);
    args.z = MapObject_GetZCoord(object);
    args.dx = 0;
    args.dz = 0;
    args.dir = -1;
    args.fieldSys = fieldSys;
    args.taskman = FieldEffectManager_GetFieldSystem(fieldSys);
    args.unk14 = FieldEffectManager_GetRendererData(fieldSys, 8);
    args.object = object;
    MapObject_CopyPositionVector(object, &pos);
    FieldEffectManager_CreateEffect(fieldSys, &ov01_02209138, &pos, a1, (UnkOv01_021FFF5C *)&args, MapObject_GetPriorityPlusValue(object, 2));
}
