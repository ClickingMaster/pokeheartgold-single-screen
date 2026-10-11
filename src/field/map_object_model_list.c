#include "global.h"

#include "field/map_object_model_list.h"

#include "map_object.h"
#include "render_animation.h"
#include "render_model.h"
#include "field_effects.h"

// Map objects whose sprite is drawn as a 3D model (gates and the like): one model per sprite id, loaded
// up front (MapObjectModelList_Init), drawn by an effect that follows the object.

#define MAP_OBJECT_MODEL_FREE 0xFFFF

void FieldEffect_SetPosition(void *effect, VecFx32 *position);

typedef struct MapObjectModel {
    u32 spriteId; // MAP_OBJECT_MODEL_FREE: free
    UnkOv01_021FFECC_sub model;
    NNSG3dRenderObj renderObj;
} MapObjectModel;

struct MapObjectModelList {
    FieldEffectManager *manager;
    u32 count;
    MapObjectModel *models;
};

typedef struct MapObjectModelEffectArgs {
    u32 spriteId;
    FieldEffectManager *manager;
    LocalMapObject *mapObject;
} MapObjectModelEffectArgs;

typedef struct MapObjectModelEffect {
    int unused;
    u32 objectId;
    u32 mapId;
    VecFx32 offset;
    NNSG3dRenderObj *renderObj;
    MapObjectModelEffectArgs args;
} MapObjectModelEffect;

typedef struct MapObjectModelFile {
    u32 spriteId;
    int fileId;
} MapObjectModelFile;

void MapObjectModel_Load(MapObjectModel *model, u32 spriteId, int fileId, FieldEffectManager *manager);
void MapObjectModel_Free(MapObjectModel *model);
void MapObjectModelList_FreeModels(MapObjectModelList *list);
MapObjectModel *MapObjectModelList_Find(MapObjectModelList *list, u32 spriteId);
void MapObjectModelList_AllocModels(FieldEffectManager *manager, u32 count);
void MapObjectModelList_AddModel(FieldEffectManager *manager, u32 spriteId, int fileId);
NNSG3dRenderObj *MapObjectModelList_GetRenderObj(FieldEffectManager *manager, u32 spriteId);
BOOL MapObjectModelEffect_Init(FieldEffect *effect, MapObjectModelEffect *work);
void MapObjectModelEffect_Destroy(FieldEffect *effect, MapObjectModelEffect *work);
void MapObjectModelEffect_Update(FieldEffect *effect, MapObjectModelEffect *work);
void MapObjectModelEffect_Draw(FieldEffect *effect, MapObjectModelEffect *work);

static const FieldEffectBasicTemplate sMapObjectModelEffectTemplate = {
    sizeof(MapObjectModelEffect),
    (FieldEffectInitFunc)MapObjectModelEffect_Init,
    (FieldEffectFunc)MapObjectModelEffect_Destroy,
    (FieldEffectFunc)MapObjectModelEffect_Update,
    (FieldEffectFunc)MapObjectModelEffect_Draw,
};

static const MapObjectModelFile sMapObjectModelFiles[12] = {
    { 0xB7,  0x54 },
    { 0x100, 0x6B },
    { 0x101, 0x6C },
    { 0xFD,  0x6D },
    { 0xFB,  0x6E },
    { 0xFC,  0x6F },
    { 0xFE,  0x70 },
    { 0xFF,  0x71 },
    { 0x120, 0x72 },
    { 0x123, 0x74 },
    { 0x124, 0x73 },
    { 0x121, 0x75 },
};

MapObjectModelList *MapObjectModelList_New(FieldEffectManager *manager) {
    MapObjectModelList *list = FieldEffectManager_HeapAllocAndFill(manager, sizeof(MapObjectModelList), FALSE, 0);
    list->manager = manager;
    return list;
}

void MapObjectModelList_Free(MapObjectModelList *list) {
    MapObjectModelList_FreeModels(list);
    if (list->models != NULL) {
        FieldEffectManager_HeapFree(list->models);
    }
    FieldEffectManager_HeapFree(list);
}

void MapObjectModel_Load(MapObjectModel *model, u32 spriteId, int fileId, FieldEffectManager *manager) {
    model->spriteId = spriteId;
    FieldEffectManager_LoadRenderModel(manager, &model->model, 0, fileId, FALSE);
    RenderObj_InitFromModel(&model->renderObj, &model->model);
}

void MapObjectModel_Free(MapObjectModel *model) {
    model->spriteId = MAP_OBJECT_MODEL_FREE;
    RenderModel_Free(&model->model);
}

void MapObjectModelList_FreeModels(MapObjectModelList *list) {
    u32 count = list->count;
    u32 i = 0;
    MapObjectModel *model = list->models;
    for (; i < count; i++) {
        if (model->spriteId != MAP_OBJECT_MODEL_FREE) {
            MapObjectModel_Free(model);
        }
        model++;
    }
}

MapObjectModel *MapObjectModelList_Find(MapObjectModelList *list, u32 spriteId) {
    u32 count = list->count;
    MapObjectModel *model = list->models;
    do {
        if (model->spriteId == spriteId) {
            return model;
        }
        model++;
    } while (--count != 0);
    return NULL;
}

void MapObjectModelList_AllocModels(FieldEffectManager *manager, u32 count) {
    MapObjectModelList *list = FieldEffectManager_GetRendererData(manager, 0x14);
    if (count == 0) {
        GF_AssertFail();
    }
    list->count = count;
    MapObjectModel *model = FieldEffectManager_HeapAllocAndFill(manager, count * sizeof(MapObjectModel), FALSE, 0);
    list->models = model;
    do {
        model->spriteId = MAP_OBJECT_MODEL_FREE;
        model++;
    } while (--count != 0);
}

void MapObjectModelList_AddModel(FieldEffectManager *manager, u32 spriteId, int fileId) {
    MapObjectModelList *list = FieldEffectManager_GetRendererData(manager, 0x14);
    if (MapObjectModelList_Find(list, spriteId) == NULL) {
        MapObjectModel *model = MapObjectModelList_Find(list, MAP_OBJECT_MODEL_FREE);
        if (model == NULL) {
            GF_AssertFail();
            return;
        }
        MapObjectModel_Load(model, spriteId, fileId, manager);
    }
}

NNSG3dRenderObj *MapObjectModelList_GetRenderObj(FieldEffectManager *manager, u32 spriteId) {
    MapObjectModelList *list = FieldEffectManager_GetRendererData(manager, 0x14);
    MapObjectModel *model = MapObjectModelList_Find(list, spriteId);
    if (model == NULL) {
        GF_AssertFail();
    }
    return &model->renderObj;
}

FieldEffect *MapObjectModel_CreateEffect(LocalMapObject *mapObject, const VecFx32 *offset) {
    MapObjectModelEffectArgs args;

    FieldEffectManager *manager = FieldEffectManager_GetFromMapObject(mapObject);
    args.spriteId = MapObject_GetSpriteID(mapObject);
    args.manager = manager;
    args.mapObject = mapObject;
    u32 priority = MapObject_GetPriorityPlusValue(mapObject, 2);
    return FieldEffectManager_CreateEffect(manager, &sMapObjectModelEffectTemplate, offset, 0, &args, priority);
}

BOOL MapObjectModelEffect_Init(FieldEffect *effect, MapObjectModelEffect *work) {
    MapObjectModelEffectArgs *args = (MapObjectModelEffectArgs *)FieldEffect_GetUserData(effect);
    work->args = *args;
    work->objectId = MapObject_GetID(work->args.mapObject);
    work->renderObj = MapObjectModelList_GetRenderObj(work->args.manager, work->args.spriteId);
    work->mapId = MapObject_CheckFlag25(work->args.mapObject) == TRUE ? sub_0205F544(work->args.mapObject) : MapObject_GetMapID(work->args.mapObject);
    FieldEffect_GetPosition(effect, &work->offset);
    return TRUE;
}

void MapObjectModelEffect_Destroy(FieldEffect *effect, MapObjectModelEffect *work) {
}

void MapObjectModelEffect_Update(FieldEffect *effect, MapObjectModelEffect *work) {
    VecFx32 position;
    VecFx32 facing;

    LocalMapObject *mapObject = work->args.mapObject;
    if (!sub_0205F0A8(mapObject, work->objectId, work->mapId)) {
        FieldEffectManager_DestroyEffect((int)effect);
        return;
    }
    MapObject_CopyPositionVector(mapObject, &position);
    MapObject_CopyFacingVector(mapObject, &facing);
    position.x += facing.x + work->offset.x;
    position.y += facing.y + work->offset.y;
    position.z += facing.z + work->offset.z;
    FieldEffect_SetPosition(effect, &position);
}

void MapObjectModelEffect_Draw(FieldEffect *effect, MapObjectModelEffect *work) {
    VecFx32 position;

    FieldEffect_GetPosition(effect, &position);
    RenderObj_DrawAt((int)work->renderObj, &position);
}

void MapObjectModelList_Init(FieldEffectManager *manager) {
    const MapObjectModelFile *file = sMapObjectModelFiles;
    u32 i = NELEMS(sMapObjectModelFiles);
    MapObjectModelList_AllocModels(manager, i);
    do {
        MapObjectModelList_AddModel(manager, file->spriteId, file->fileId);
        file++;
    } while (--i != 0);
}
