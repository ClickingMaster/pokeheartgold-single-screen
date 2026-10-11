#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"

#include "field_effects.h"
#include "map_object.h"

// The marker shown over a Union Room player for a moment when the touch screen chat log entry of
// something they said is touched (field effect renderer 21): a billboard that follows the player's map
// object until the effect is destroyed.

#define UNION_CHAT_MARKER_RECORD   4
#define UNION_CHAT_MARKER_RESOURCE 4
#define UNION_CHAT_MARKER_TEXTURE  3

// Renderer data, from FieldEffectManager_GetRendererData(manager, 21).
typedef struct UnionChatMarker {
    FieldEffectManager *manager;
} UnionChatMarker;

typedef struct UnionChatMarkerEffectArgs {
    FieldSystem *fieldSystem;
    FieldEffectManager *manager;
    UnionChatMarker *marker;
    LocalMapObject *mapObject;
} UnionChatMarkerEffectArgs;

typedef struct UnionChatMarkerEffect {
    u8 unk00[4];
    u32 objectId;
    u32 mapId;
    u8 unk0C[4];
    UnionChatMarkerEffectArgs args;
    void *billboard;
} UnionChatMarkerEffect;

UnionChatMarker *UnionChatMarker_New(FieldEffectManager *manager);
void UnionChatMarker_Free(UnionChatMarker *marker);
void UnionChatMarker_LoadGfx(UnionChatMarker *marker);
void UnionChatMarker_FreeGfx(UnionChatMarker *marker);
FieldEffect *UnionChatMarker_Spawn(LocalMapObject *mapObject);
BOOL UnionChatMarkerEffect_Init(FieldEffect *effect, UnionChatMarkerEffect *work);
void UnionChatMarkerEffect_Destroy(FieldEffect *effect, UnionChatMarkerEffect *work);
void UnionChatMarkerEffect_Update(FieldEffect *effect, UnionChatMarkerEffect *work);
void UnionChatMarkerEffect_Draw(FieldEffect *effect, UnionChatMarkerEffect *work);

static const FieldEffectBasicTemplate sUnionChatMarkerEffectTemplate = {
    sizeof(UnionChatMarkerEffect),
    (FieldEffectInitFunc)UnionChatMarkerEffect_Init,
    (FieldEffectFunc)UnionChatMarkerEffect_Destroy,
    (FieldEffectFunc)UnionChatMarkerEffect_Update,
    (FieldEffectFunc)UnionChatMarkerEffect_Draw,
};

static const u32 sUnionChatMarkerRecordParams[6] = { 0, 1, 1, 0, 0, 2 };

UnionChatMarker *UnionChatMarker_New(FieldEffectManager *manager) {
    UnionChatMarker *marker = FieldEffectManager_HeapAllocAndFill(manager, sizeof(UnionChatMarker), FALSE, 0);
    marker->manager = manager;
    UnionChatMarker_LoadGfx(marker);
    return marker;
}

void UnionChatMarker_Free(UnionChatMarker *marker) {
    UnionChatMarker_FreeGfx(marker);
    FieldEffectManager_HeapFree(marker);
}

void UnionChatMarker_LoadGfx(UnionChatMarker *marker) {
    FieldEffectManager_LoadBillboardResource(marker->manager, UNION_CHAT_MARKER_RESOURCE, 0x78);
    FieldEffectManager_LoadTexResource(marker->manager, UNION_CHAT_MARKER_TEXTURE, 0x13, TRUE);
    FieldEffectManager_AddRecord(marker->manager, UNION_CHAT_MARKER_RECORD, UNION_CHAT_MARKER_RESOURCE, 0, UNION_CHAT_MARKER_TEXTURE, 0, sUnionChatMarkerRecordParams);
}

void UnionChatMarker_FreeGfx(UnionChatMarker *marker) {
    FieldEffectManager_FreeBillboardResource(marker->manager, UNION_CHAT_MARKER_RESOURCE);
    FieldEffectManager_FreeTexResource(marker->manager, UNION_CHAT_MARKER_TEXTURE);
    FieldEffectManager_ReleaseRecord(marker->manager, UNION_CHAT_MARKER_RECORD);
}

FieldEffect *UnionChatMarker_Spawn(LocalMapObject *mapObject) {
    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    UnionChatMarkerEffectArgs args;

    args.fieldSystem = MapObject_GetFieldSystem(mapObject);
    args.manager = manager;
    args.marker = FieldEffectManager_GetRendererData(manager, 21);
    {
        VecFx32 position = { 0, 0, 0 };
        args.mapObject = mapObject;
        return FieldEffectManager_CreateEffect(manager, &sUnionChatMarkerEffectTemplate, &position, 0, &args, MapObject_GetPriorityPlusValue(mapObject, 2));
    }
}

BOOL UnionChatMarkerEffect_Init(FieldEffect *effect, UnionChatMarkerEffect *work) {
    VecFx32 position = { 0, 0, 0 };
    work->args = *(UnionChatMarkerEffectArgs *)FieldEffect_GetUserData(effect);
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->mapId = MapObject_GetMapID(work->args.mapObject);
    FieldEffect_SetPosition(effect, &position);
    work->billboard = FieldEffectManager_CreateBillboardFromRecord(work->args.manager, UNION_CHAT_MARKER_RECORD, &position);
    return TRUE;
}

void UnionChatMarkerEffect_Destroy(FieldEffect *effect, UnionChatMarkerEffect *work) {
    BillboardObject_Delete(work->billboard);
}

void UnionChatMarkerEffect_Update(FieldEffect *effect, UnionChatMarkerEffect *work) {
    if (!sub_0205F0A8(work->args.mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
    }
}

void UnionChatMarkerEffect_Draw(FieldEffect *effect, UnionChatMarkerEffect *work) {
    LocalMapObject *mapObject = work->args.mapObject;
    VecFx32 position;
    VecFx32 facing;

    if (!sub_0205F0A8(mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    position.x += facing.x;
    position.y += facing.y;
    position.z += facing.z;
    position.z += 12 * FX32_ONE;
    BillboardObject_SetPosition(work->billboard, &position);
}
