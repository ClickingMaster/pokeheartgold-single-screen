#include "field_effect_path_motion_internal.h"

void *ov01_021FF6B0(void *owner) {
    void *work = FieldEffectManager_HeapAllocAndFill(owner, 4, 0, 0);
    *(void **)work = owner;
    ov01_021FF6DC(work);
    return work;
}

void ov01_021FF6CC(void *work) {
    ov01_021FF724(work);
    FieldEffectManager_HeapFree(work);
}

void ov01_021FF6DC(void *work) {
    ov01_021F18D4(*(void **)work, 6, 0x79);
    ov01_021F1908(*(void **)work, 6, 0x91);
    FieldEffectManager_LoadTexResource(*(void **)work, 7, 0x14, 1);
    FieldEffectManager_AddRecord(*(void **)work, 8, 6, 6, 7, 0, ov01_02209200);
}

void ov01_021FF724(void *work) {
    ov01_021F18FC(*(void **)work, 6);
    ov01_021F1924(*(void **)work, 6);
    FieldEffectManager_FreeTexResource(*(void **)work, 7);
    FieldEffectManager_ReleaseRecord(*(void **)work, 8);
}

void ov01_021FF74C(void *mapObject) {
    EffectContextPrivate context;
    VecFx32Private position;
    void *manager = FieldEffectManager_GetFromMapObject(mapObject);
    s32 x;
    s32 z;
    context.fieldSystem = MapObject_GetFieldSystem(mapObject);
    context.manager = manager;
    context.work = FieldEffectManager_GetRendererData(manager, 11);
    context.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    sub_020611C8(x, z, &position);
    FieldEffectManager_CreateEffect(manager, ov01_022091EC, &position, 0, &context, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL ov01_021FF7B0(void *object, void *data) {
    u8 *p = data;
    VecFx32Private position;
    *(FourWordsPrivate *)(p + 0x10) = *(FourWordsPrivate *)FieldEffect_GetUserData(object);
    *(u32 *)(p + 4) = MapObject_GetID(*(void **)(p + 0x1c));
    *(u32 *)(p + 8) = MapObject_GetMapID(*(void **)(p + 0x1c));
    FieldEffect_GetPosition(object, &position);
    *(void **)(p + 0x20) = FieldEffectManager_CreateBillboardFromRecord(*(void **)(p + 0x14), 8, &position);
    return TRUE;
}

void ov01_021FF7F4(void *unused, void *data) {
    BillboardObject_Delete(*(void **)((u8 *)data + 0x20));
}

void ov01_021FF800(void *task, void *data) {
    u8 *p = data;
    if (*(u32 *)p == 0) {
        s32 height;
        BillboardObject_AdvanceFrame(*(void **)(p + 0x20), 0x1000);
        height = BillboardObject_GetSequenceFrame(*(void **)(p + 0x20));
        if (height / 0x1000 >= 9) {
            FieldEffectManager_DestroyEffect(task);
        }
    }
}

void ov01_021FF830(void *object, void *data) {
    VecFx32Private position;
    FieldEffect_GetPosition(object, &position);
    position.z += 0x8000;
    BillboardObject_SetPosition(*(void **)((u8 *)data + 0x20), &position);
}
