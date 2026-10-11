#include "field_effect_variant_task_internal.h"

void *ov01_022006A8(u32 context) {
    void *manager = FieldEffectManager_HeapAllocAndFill(context, 4, 0, 0);
    *(u32 *)manager = context;
    ov01_022006D4(manager);
    return manager;
}

void ov01_022006C4(void *manager) {
    ov01_02200710(manager);
    FieldEffectManager_HeapFree(manager);
}

void ov01_022006D4(void *manager) {
    ov01_021F18D4(*(void **)manager, 4, 0x78);
    FieldEffectManager_LoadTexResource(*(void **)manager, 3, 0x13, 1);
    FieldEffectManager_AddRecord(*(void **)manager, 4, 4, 0, 3, 0, ov01_0220931C);
}

void ov01_02200710(void *manager) {
    ov01_021F18FC(*(void **)manager, 4);
    FieldEffectManager_FreeTexResource(*(void **)manager, 3);
    FieldEffectManager_ReleaseRecord(*(void **)manager, 4);
}

void ov01_02200730(void *mapObject) {
    void *context = FieldEffectManager_GetFromMapObject(mapObject);
    Ov01SpawnArgs args;

    args.fieldSystem = MapObject_GetFieldSystem(mapObject);
    args.context = context;
    args.renderer = FieldEffectManager_GetRendererData(context, 0x15);
    {
        Ov01Vec3 position = { 0, 0, 0 };
        args.mapObject = mapObject;
        FieldEffectManager_CreateEffect(context, ov01_02209308, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
    }
}

BOOL ov01_02200780(void *task, Ov01ObjectState *state) {
    Ov01Vec3 position = { 0, 0, 0 };
    state->setup = *(Ov01Setup *)sub_02068D98(task);
    state->objectId = MapObject_GetID((void *)state->setup.words[3]);
    state->mapId = MapObject_GetMapID((void *)state->setup.words[3]);
    sub_02068DA8(task, &position);
    state->effect = FieldEffectManager_CreateBillboardFromRecord((void *)state->setup.words[1], 4, &position);
    return TRUE;
}

void ov01_022007D0(void *unused, Ov01ObjectState *state) {
    BillboardObject_Delete(state->effect);
}

void ov01_022007DC(void *task, Ov01ObjectState *state) {
    if (!sub_0205F0A8((void *)state->setup.words[3], state->objectId, state->mapId)) {
        FieldEffectManager_DestroyEffect(task);
    }
}

void ov01_022007F8(void *task, Ov01ObjectState *state) {
    void *mapObject = (void *)state->setup.words[3];
    Ov01Vec3 position;
    Ov01Vec3 facing;

    if (!sub_0205F0A8(mapObject, state->objectId, state->mapId)) {
        FieldEffectManager_DestroyEffect(task);
        return;
    }
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    position.x += facing.x;
    position.y += facing.y;
    position.z += facing.z;
    position.z += 3 << 14;
    BillboardObject_SetPosition(state->effect, &position);
}
