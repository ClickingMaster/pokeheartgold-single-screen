#include "field_hidden_object_cover.h"

#include "map_object.h"
#include "field/field_effect_manager.h"
#include "player_avatar.h"
#include "map_object_movement_utilities.h"
#include "map_object_follow.h"
#include "field_effects.h"
#include "render_model.h"
#include "render_animation.h"

struct HiddenObjectCover {
    FieldEffectManager *manager;
    UnkOv01_021FFECC_sub models[4];
    NNSG3dRenderObj renderObjs[4];
};

typedef struct {
    int unk0;
    FieldEffectManager *manager;
    HiddenObjectCover *unk8;
    LocalMapObject *mapObject;
    NNSG3dRenderObj *renderObj;
} HiddenObjectCoverEffectArgs;

typedef struct {
    int unk0;
    BOOL unk4;
    u32 localMapObjectId;
    u32 mapId;
    HiddenObjectCoverEffectArgs args;
} HiddenObjectCoverEffect;

static void HiddenObjectCover_LoadModels(HiddenObjectCover *a0);
static void HiddenObjectCover_FreeModels(HiddenObjectCover *a0);
static BOOL HiddenObjectCoverEffect_Init(FieldEffect *effect, HiddenObjectCoverEffect *a1);
static BOOL HiddenObjectCoverEffect_Destroy(FieldEffect *effect, HiddenObjectCoverEffect *a1);
static void HiddenObjectCoverEffect_Update(FieldEffect *effect, HiddenObjectCoverEffect *a1);
static void HiddenObjectCoverEffect_Draw(FieldEffect *effect, HiddenObjectCoverEffect *a1);

static const int sHiddenObjectCoverModelFiles[4] = { 0x64, 0x65, 0x66, 0x67 };
static const FieldEffectBasicTemplate sHiddenObjectCoverEffectTemplate = {
    sizeof(HiddenObjectCoverEffect),
    (FieldEffectInitFunc)HiddenObjectCoverEffect_Init,
    (FieldEffectFunc)HiddenObjectCoverEffect_Destroy,
    (FieldEffectFunc)HiddenObjectCoverEffect_Update,
    (FieldEffectFunc)HiddenObjectCoverEffect_Draw,
};

HiddenObjectCover *HiddenObjectCover_New(FieldEffectManager *manager) {
    HiddenObjectCover *ptr = FieldEffectManager_HeapAllocAndFill(manager, sizeof(HiddenObjectCover), FALSE, 0);
    ptr->manager = manager;
    HiddenObjectCover_LoadModels(ptr);
    return ptr;
}

void HiddenObjectCover_Free(HiddenObjectCover *a0) {
    HiddenObjectCover_FreeModels(a0);
    FieldEffectManager_HeapFree(a0);
}

static void HiddenObjectCover_LoadModels(HiddenObjectCover *a0) {
    for (int i = 0; i < 4; i++) {
        FieldEffectManager_LoadRenderModel(a0->manager, &a0->models[i], 0, sHiddenObjectCoverModelFiles[i], FALSE);
        RenderObj_InitFromModel(&a0->renderObjs[i], &a0->models[i]);
    }
}

static void HiddenObjectCover_FreeModels(HiddenObjectCover *a0) {
    for (int i = 0; i < 4; i++) {
        RenderModel_Free(&a0->models[i]);
    }
}

static NNSG3dRenderObj *HiddenObjectCover_GetRenderObj(HiddenObjectCover *a0, int idx) {
    return &a0->renderObjs[idx];
}

void HiddenObjectCover_Spawn(LocalMapObject *mapObject, int a1) {
    VecFx32 temp;
    HiddenObjectCoverEffectArgs temp2;

    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    FieldSystem *fieldSystem = FieldEffectManager_GetFieldSystem(manager);

    int x = MapObject_GetXCoord(mapObject);
    int z = MapObject_GetZCoord(mapObject);

    SetPositionToTileCenter(x, z, &temp);
    FieldSystem_SnapToTerrainHeight(fieldSystem, &temp);

    temp2.unk0 = a1;
    temp2.manager = manager;

    HiddenObjectCover *val1 = FieldEffectManager_GetRendererData(manager, 0xe);
    temp2.unk8 = val1;
    temp2.mapObject = mapObject;
    temp2.renderObj = HiddenObjectCover_GetRenderObj(val1, a1);

    int val3 = MapObject_GetPriorityPlusValue(mapObject, 2);
    FieldEffectManager_CreateEffect(manager, &sHiddenObjectCoverEffectTemplate, &temp, 0, &temp2, val3);
}

static BOOL HiddenObjectCoverEffect_Init(FieldEffect *effect, HiddenObjectCoverEffect *a1) {
    HiddenObjectCoverEffectArgs *args = (HiddenObjectCoverEffectArgs *)FieldEffect_GetUserData(effect);
    LocalMapObject *mapObject = args->mapObject;

    a1->args = *args;
    a1->localMapObjectId = MapObject_GetID(mapObject);
    a1->mapId = MapObject_GetMapID(mapObject);
    return TRUE;
}

static BOOL HiddenObjectCoverEffect_Destroy(FieldEffect *effect, HiddenObjectCoverEffect *a1) {
    LocalMapObject *mapObject = a1->args.mapObject;

    BOOL val = MapObject_CheckIDAndMap(mapObject, a1->localMapObjectId, a1->mapId);
    if (val == TRUE) {
        return HiddenObjectMovement_SetCoverEffect(mapObject, 0);
    }
    return val;
}

static void HiddenObjectCoverEffect_Update(FieldEffect *effect, HiddenObjectCoverEffect *a1) {
}

static void HiddenObjectCoverEffect_Draw(FieldEffect *effect, HiddenObjectCoverEffect *a1) {
    VecFx32 translation;

    if (a1->unk4 != TRUE) {
        FieldEffect_GetPosition(effect, &translation);
        RenderObj_DrawAt((int)a1->args.renderObj, &translation);
    }
}
