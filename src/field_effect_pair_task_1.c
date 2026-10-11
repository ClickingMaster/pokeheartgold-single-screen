#include "field_effect_pair_task_internal.h"

static const u32 ov01_02209294[];
static const u32 ov01_022092A8[];

LocalManager *ov01_02200040(void *owner) {
    LocalManager *manager = FieldEffectManager_HeapAllocAndFill(owner, sizeof(LocalManager), 0, 0);
    manager->owner = owner;
    ov01_0220006C(manager);
    return manager;
}

void ov01_0220005C(LocalManager *manager) {
    ov01_022000B4(manager);
    FieldEffectManager_HeapFree(manager);
}

void ov01_0220006C(LocalManager *manager) {
    ov01_021F18D4(manager->owner, 9, 0x7c);
    ov01_021F1908(manager->owner, 9, 0x94);
    FieldEffectManager_LoadTexResource(manager->owner, 10, 0x17, 1);
    FieldEffectManager_AddRecord(manager->owner, 11, 9, 9, 10, 0, ov01_022092A8);
}

void ov01_022000B4(LocalManager *manager) {
    ov01_021F18FC(manager->owner, 9);
    ov01_021F1924(manager->owner, 9);
    FieldEffectManager_FreeTexResource(manager->owner, 10);
    FieldEffectManager_ReleaseRecord(manager->owner, 11);
}

void ov01_022000DC(void *mapObject) {
    struct {
        void *fieldSystem;
        void *manager;
        void *resource;
        void *mapObject;
    } local;
    VecFx32 position;
    int x;
    int z;
    void *manager = FieldEffectManager_GetFromMapObject(mapObject);
    local.fieldSystem = MapObject_GetFieldSystem(mapObject);
    local.manager = manager;
    local.resource = FieldEffectManager_GetRendererData(manager, 15);
    local.mapObject = mapObject;
    MapObject_CopyPositionVector(mapObject, &position);
    x = MapObject_GetXCoord(mapObject);
    z = MapObject_GetZCoord(mapObject);
    sub_020611C8(x, z, &position);
    FieldEffectManager_CreateEffect(manager, ov01_02209294, &position, NULL, &local, MapObject_GetPriorityPlusValue(mapObject, 2));
}

BOOL ov01_02200140(void *task, LocalState *state) {
    VecFx32 position;
    *(LocalFourWords *)&state->args0 = *(LocalFourWords *)sub_02068D98(task);
    state->objectId = MapObject_GetID(state->mapObject);
    state->mapId = MapObject_GetMapID(state->mapObject);
    sub_02068DB8(task, &position);
    state->effect = FieldEffectManager_CreateBillboardFromRecord(state->resource, 11, &position);
    return TRUE;
}

void ov01_02200184(void *task, LocalState *state) {
    BillboardObject_Delete(state->effect);
}

void ov01_02200190(void *task, LocalState *state) {
    if (state->stage == 0) {
        BillboardObject_AdvanceFrame(state->effect, FX32_ONE);
        if (BillboardObject_GetSequenceFrame(state->effect) / FX32_ONE >= 7) {
            FieldEffectManager_DestroyEffect(task);
        }
    }
}

void ov01_022001C0(void *task, LocalState *state) {
    VecFx32 position;
    sub_02068DB8(task, &position);
    position.z += 8 * FX32_ONE;
    BillboardObject_SetPosition(state->effect, &position);
}

static const u32 ov01_02209294[] = {
    0x24,
    (u32)ov01_02200140,
    (u32)ov01_02200184,
    (u32)ov01_02200190,
    (u32)ov01_022001C0,
};

static const u32 ov01_022092A8[] = { 0, 7, 1, 0, 0, 2 };
