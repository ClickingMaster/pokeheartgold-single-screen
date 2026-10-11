#include "field_effect_follow_motion_internal.h"
void *ov01_021FEA0C(u32 arg) {
    u8 *p = FieldEffectManager_HeapAllocAndFill(arg, 0x7c, 0, 0);
    *(u32 *)(p + 0x10) = arg;
    return p;
}
void ov01_021FEA20(void *p) {
    ov01_021FEA7C(p);
    FieldEffectManager_HeapFree(p);
}
void ov01_021FEA30(void *p) {
    ++*(s32 *)p;
}
void ov01_021FEA38(void *p) {
    if (--*(s32 *)p < 0) {
        GF_AssertFail();
    }
}
void ov01_021FEA48(void *v) {
    u8 *p = v;
    if (*(u32 *)(p + 4) == 0) {
        FieldEffectManager_LoadRenderModel(*(void **)(p + 0x10), p + 0x14, 0, 0x57, 1);
        RenderObj_InitFromModel(p + 0x28, p + 0x14);
        *(u32 *)(p + 4) = 1;
    }
}
void ov01_021FEA7C(void *v) {
    u8 *p = v;
    if (*(u32 *)(p + 4) == 1) {
        *(u32 *)(p + 4) = 0;
        RenderModel_Free(p + 0x14);
    }
}
void ov01_021FEA90(void *p) {
    if (*(u32 *)p == 0) {
        ov01_021FEA48(p);
    }
}
void ov01_021FEAA0(void *p) {
    if (*(u32 *)p == 0) {
        ov01_021FEA7C(p);
    }
}
void ov01_021FEAB0(void *mapObj, s32 x, s32 z, u32 kind, u32 usePosition) {
    SpawnSmall a;
    Vec3 pos = { 0, 0, 0 };
    void *ctx = FieldEffectManager_GetFromMapObject(mapObj);
    s32 priority;
    a.kind = kind;
    a.ctx = ctx;
    a.fieldSystem = MapObject_GetFieldSystem(mapObj);
    a.member = FieldEffectManager_GetRendererData(ctx, 5);
    a.mapObj = mapObj;
    if (usePosition == 0) {
        pos.x = (x << 16) + 0x8000;
        pos.z = (z << 16) + 0x8000;
        sub_0206121C(a.fieldSystem, &pos);
    } else {
        MapObject_CopyPositionVector(mapObj, &pos);
    }
    priority = MapObject_GetPriorityPlusValue(mapObj, 2);
    FieldEffectManager_CreateEffect(ctx, ov01_022090FC, &pos, usePosition, &a, priority);
}
void ov01_021FEB30(void *task, void *v) {
    void *p = FieldEffect_GetState(task);
    *(void **)((u8 *)p + 0xc) = v;
}
BOOL ov01_021FEB3C(void *task, void *v) {
    u8 *p = v;
    Copy20 *src = FieldEffect_GetUserData(task);
    *(Copy20 *)(p + 0x10) = *src;
    *(u32 *)(p + 4) = src->v[0];
    *(void **)(p + 0xc) = FieldEffect_GetParam(task);
    ov01_021FEA90(*(void **)(p + 0x1c));
    ov01_021FEA30(*(void **)(p + 0x1c));
    return TRUE;
}
void ov01_021FEB78(void *task, void *v) {
    u8 *p = v;
    ov01_021FEA38(*(void **)(p + 0x1c));
    ov01_021FEAA0(*(void **)(p + 0x1c));
}
void ov01_021FEB8C(void *task, void *v) {
    u8 *p = v;
    Vec3 zero, pos;
    void *mapObj = *(void **)(p + 0x20);
    if (*(u32 *)(p + 0xc) != 0) {
        memset(&zero, 0, sizeof(zero));
        sub_0205F9A0(mapObj, &zero);
        MapObject_CopyPositionVector(mapObj, &pos);
        FieldEffect_SetPosition(task, &pos);
    }
}
void ov01_021FEBC0(void *task, EffectWork *work) {
    Rot9 rot;
    Vec3 pos;
    Vec3 scale;
    void *obj;
    int angle;
    if (work->state == 1) {
        return;
    }
    work->counter++;
    angle = 0;
    scale = ov01_022090F0;
    obj = work->ctx->raw + 0x28;
    switch (work->dir) {
    case 0:
        angle = 180;
        break;
    case 1:
        break;
    case 2:
        angle = 270;
        break;
    case 3:
        angle = 90;
        break;
    }
    Math_CalcRotationMatrixXYZDeg(&rot, 0, angle, 0);
    FieldEffect_GetPosition(task, &pos);
    RenderObj_Draw(obj, &pos, &scale, &rot);
}
