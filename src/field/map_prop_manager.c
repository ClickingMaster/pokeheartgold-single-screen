#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"
#include "field/build_model_shapes.h"

#include "field_system.h"
#include "gf_3d_render.h"
#include "model_box_test.h"
#include "geometry_math.h"
#include "safari_zone_encounters.h"

typedef struct MapPropArcData {
    int buildModel;
    VecFx32 translation;
    VecFx32 rotation;
    VecFx32 scale;
    u8 padding[8];
} MapPropArcData; // size: 0x30

struct MapProp {
    int buildModel;
    BOOL active;
    BOOL culled;
    BOOL overrideRotation;
    Field3dRenderObj *render;
    VecFx32 translation;
    VecFx32 rotation;
    VecFx32 scale;
}; // size: 0x38

struct MapPropManager {
    MapProp mapProps[MAP_PROP_MAX];
    Field3dRenderObjManager *fsys_unkC0;
}; // size: 0x704

static void MapPropManager_Init(MapPropManager *mapPropManager, void *unkC0);
static void MapProp_Init(MapProp *mapProp);
static void DrawModelShapewise(const NNSG3dResMdl *mapPropManager, const VecFx32 *baseTrans, const MtxFx33 *prmBaseRot, const VecFx32 *baseScale, BuildModelShapes *shapes, int buildModel);

static void MapPropManager_Init(MapPropManager *mapPropManager, void *unkC0) {
    mapPropManager->fsys_unkC0 = unkC0;
    for (u8 i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        MapProp_Init(mapProp);
        mapProp->render = NULL;
    }
}

MapPropManager *MapPropManager_New(enum HeapID heapID, Field3dRenderObjManager *unkC0) {
    MapPropManager *ret = Heap_Alloc(heapID, sizeof(MapPropManager));
    MI_CpuClearFast(ret, sizeof(MapPropManager));
    MapPropManager_Init(ret, unkC0);
    return ret;
}

void MapPropManager_Free(MapPropManager *mapPropManager) {
    Heap_Free(mapPropManager);
}

static void MapProp_Init(MapProp *mapProp) {
    VecFx32 zero = { 0, 0, 0 };

    mapProp->buildModel = 0;
    mapProp->active = FALSE;
    mapProp->culled = FALSE;
    mapProp->overrideRotation = FALSE;
    mapProp->translation = zero;
    mapProp->rotation = zero;
    mapProp->scale = zero;
}

void MapPropManager_Reset(MapPropManager *mapPropManager) {
    for (u8 i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        MapProp_Init(mapProp);
        Field3dObjectList_ReleaseInstance(mapPropManager->fsys_unkC0, mapProp->render);
        mapProp->render = NULL;
    }
}

void MapPropManager_RemoveMapPropByIndex(int modelID, MapPropManager *mapPropManager) {
    GF_ASSERT(modelID < MAP_PROP_MAX);
    VecFx32 zero = { 0, 0, 0 };
    MapProp *mapProp = &mapPropManager->mapProps[modelID];

    mapProp->buildModel = 0;
    mapProp->active = FALSE;
    mapProp->culled = FALSE;
    mapProp->overrideRotation = FALSE;
    mapProp->translation = zero;
    mapProp->rotation = zero;
    mapProp->scale = zero;

    Field3dObjectList_ReleaseInstance(mapPropManager->fsys_unkC0, mapPropManager->mapProps[modelID].render);
    mapPropManager->mapProps[modelID].render = NULL;
}

// NARC_a_0_6_5
void MapPropManager_LoadFromNARC(NARC *narc, u32 size, MapPropManager *mapPropManager, int a3) {
    MapPropArcData *narcData = NULL;
    u32 num;

    if (size != 0) {
        narcData = Heap_AllocAtEnd(HEAP_ID_FIELD1, size);
        NARC_ReadFile(narc, size, narcData);
        num = size / sizeof(MapPropArcData);
    } else {
        num = 0;
    }

    for (int i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        if (i < num) {
            mapProp->buildModel = narcData[i].buildModel;
            mapProp->active = TRUE;
            mapProp->overrideRotation = FALSE;
            mapProp->translation = narcData[i].translation;
            mapProp->rotation = narcData[i].rotation;
            mapProp->scale = narcData[i].scale;
            if (!Field3dObjectList_HasModelFile(mapPropManager->fsys_unkC0, mapProp->buildModel)) {
                mapProp->buildModel = 0;
            }
            mapProp->render = Field3dObjectList_GetOrAddInstance(mapPropManager->fsys_unkC0, mapProp->buildModel);
        } else {
            VecFx32 zero = { 0, 0, 0 };
            mapProp->buildModel = 0;
            mapProp->active = FALSE;
            mapProp->overrideRotation = FALSE;
            mapProp->translation = zero;
            mapProp->rotation = zero;
            mapProp->scale = zero;
        }
    }
    if (narcData != NULL) {
        Heap_Free(narcData);
    }
}

void MapPropManager_LoadFromSafariZone(NARC *narc, MapPropManager *mapPropManager, int a2, const SAFARIZONE_AREA *safariArea, u16 *a4, BOOL gender) {
    u32 numSafariObjects = safariArea->active_object_count;
    if (numSafariObjects > SAFARI_ZONE_MAX_OBJECTS) {
        GF_ASSERT(FALSE);
        numSafariObjects = 0;
    }

    for (int i = 0; i < MAP_PROP_MAX; ++i) {
        VecFx32 rotation = { 0, 0, 0 };
        VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };
        MapProp *mapProp = &mapPropManager->mapProps[i];

        if (i >= numSafariObjects) {
            mapProp->buildModel = 0;
            mapProp->active = FALSE;
            mapProp->overrideRotation = FALSE;
            mapProp->translation = rotation;
            mapProp->rotation = rotation;
            mapProp->scale = scale;
        } else {
            SafariObjectConfig objectConfig;
            GetSafariObjectConfig(&objectConfig, safariArea->objects[i].id, gender);
            mapProp->buildModel = objectConfig.buildModel;
            s16 x = 16 * safariArea->objects[i].x + 8 * objectConfig.width;
            s16 y = safariArea->objects[i].y;
            s16 z = 16 * safariArea->objects[i].z + 8 * (2 - objectConfig.height);
            mapProp->translation.x = FX32_CONST(x - 0x100);
            mapProp->translation.y = FX32_CONST(y);
            mapProp->translation.z = FX32_CONST(z - 0x100);
            mapProp->rotation = rotation;
            mapProp->scale = scale;
            mapProp->active = TRUE;
            mapProp->overrideRotation = FALSE;
            for (int j = safariArea->objects[i].z; j > safariArea->objects[i].z - objectConfig.height; --j) {
                for (int k = safariArea->objects[i].x; k < safariArea->objects[i].x + objectConfig.width; ++k) {
                    a4[j * 32 + k] = 0x8023;
                }
            }
            if (!Field3dObjectList_HasModelFile(mapPropManager->fsys_unkC0, mapProp->buildModel)) {
                mapProp->buildModel = 0;
            }
            mapProp->render = Field3dObjectList_GetOrAddInstance(mapPropManager->fsys_unkC0, mapProp->buildModel);
        }
    }
}

void MapPropManager_DrawWithOffset(const VecFx32 *position, AreaDataManager *areaDataManager, BOOL applyAttributes, ModelAttributes *modelAttributes, MapPropManager *mapPropManager) {
    VecFx32 translation;
    MtxFx33 rotation = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

    for (int i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        if (!mapProp->active || mapProp->culled) {
            continue;
        }
        translation = mapProp->translation;
        translation.x += position->x;
        translation.z += position->z;
        if (!GF3d_ModelBoxTest(mapProp->render->model, &translation, &rotation, &mapProp->scale)) {
            continue;
        }
        BuildModelShapes *buildModelShapes = AreaDataManager_GetBuildModelShapes(areaDataManager);
        if (applyAttributes == TRUE) {
            ModelAttributes_ApplyToModel(modelAttributes, mapProp->render->model, 0xF33);
        }
        u16 shapeCount;
        BuildModelShapes_GetShapeCount(mapProp->buildModel, buildModelShapes, &shapeCount);
        if (shapeCount == 0) {
            GF3dRender_DrawModel(&mapProp->render->renderObj, &translation, &rotation, &mapProp->scale);
        } else {
            DrawModelShapewise(mapProp->render->model, &translation, &rotation, &mapProp->scale, buildModelShapes, mapProp->buildModel);
        }
    }
}

VecFx32 *MapProp_GetScale(MapProp *mapProp) {
    return &mapProp->scale;
}

VecFx32 *MapProp_GetRotation(MapProp *mapProp) {
    return &mapProp->rotation;
}

void MapProp_GetTranslation(VecFx32 *translation, MapProp *mapProp) {
    *translation = mapProp->translation;
}

void MapProp_SetTranslation(MapProp *mapProp, VecFx32 *translation) {
    mapProp->translation = *translation;
}

void MapProp_SetCulled(MapProp *mapProp, BOOL culled) {
    mapProp->culled = culled;
}

BOOL MapProp_IsActive(MapProp *mapProp) {
    return mapProp->active;
}

int MapProp_GetBuildModel(MapProp *mapProp) {
    return mapProp->buildModel;
}

NNSG3dRenderObj *MapProp_GetRenderSurface(MapProp *mapProp) {
    return &mapProp->render->renderObj;
}

NNSG3dResMdl *MapProp_GetResModel(MapProp *mapProp) {
    return mapProp->render->model;
}

MapProp *MapPropManager_GetMapPropByIndex(MapPropManager *mapPropManager, u8 index) {
    return &mapPropManager->mapProps[index];
}

MapProp *MapPropManager_FindMapPropByBuildModel(MapPropManager *mapPropManager, int buildModel) {
    for (int i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        if (mapProp->buildModel == buildModel) {
            return mapProp;
        }
    }

    return NULL;
}

MapProp *MapPropManager_GetMapPropByIndex_Checked_RequireActive(MapPropManager *mapPropManager, int index) {
    GF_ASSERT(index < MAP_PROP_MAX);
    MapProp *ret = &mapPropManager->mapProps[index];
    GF_ASSERT(ret->active);
    return ret;
}

static void DrawModelShapewise(const NNSG3dResMdl *model, const VecFx32 *baseTrans, const MtxFx33 *prmBaseRot, const VecFx32 *baseScale, BuildModelShapes *shapes, int buildModel) {
    NNS_G3dGlbSetBaseTrans(baseTrans);
    NNS_G3dGlbSetBaseRot(prmBaseRot);
    NNS_G3dGlbSetBaseScale(baseScale);
    NNS_G3dGlbFlush();

    u16 num;
    u16 start;
    BuildModelShapes_GetShapeRange(buildModel, shapes, &num, &start);
    u16(*shpDat)[2] = BuildModelShapes_GetShapes(start, shapes);
    u8 i;
    u8 matID = 0xFF;
    for (i = 0; i < num; ++i) {
        BOOL sendMat;
        if (matID != shpDat[i][0]) {
            matID = shpDat[i][0];
            sendMat = TRUE;
        } else {
            sendMat = FALSE;
        }
        NNS_G3dDraw1Mat1Shp(model, matID, shpDat[i][1], sendMat);
    }
}

u8 MapPropManager_LoadOne(MapPropManager *mapPropManager, int modelID, const VecFx32 *position, const VecFx32 *rotation, MapPropAnimationManager *mapPropAnimationManager) {
    VecFx32 scale = { FX32_ONE, FX32_ONE, FX32_ONE };

    for (u8 i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        if (mapProp->active) {
            continue;
        }
        mapProp->active = TRUE;
        mapProp->translation = *position;
        if (rotation != NULL) {
            mapProp->overrideRotation = TRUE;
            mapProp->rotation = *rotation;
        } else {
            mapProp->overrideRotation = FALSE;
        }
        mapProp->scale = scale;
        mapProp->buildModel = modelID;
        mapProp->render = Field3dObjectList_GetOrAddInstance(mapPropManager->fsys_unkC0, modelID);
        return i;
    }

    GF_ASSERT(FALSE);
    return MAP_PROP_MAX;
}

void MapPropManager_Draw(MapPropManager *mapPropManager, AreaDataManager *areaDataManager) {
    MtxFx33 baseRot = { FX32_ONE, 0, 0, 0, FX32_ONE, 0, 0, 0, FX32_ONE };

    for (u8 i = 0; i < MAP_PROP_MAX; ++i) {
        MapProp *mapProp = &mapPropManager->mapProps[i];
        if (!mapProp->active || mapProp->culled) {
            continue;
        }
        BuildModelShapes *buildModelShapes = AreaDataManager_GetBuildModelShapes(areaDataManager);
        u16 shapeCount;
        BuildModelShapes_GetShapeCount(mapProp->buildModel, buildModelShapes, &shapeCount);
        if (mapProp->overrideRotation) {
            Math_CalcRotationMatrixXYZ(&baseRot, &mapProp->rotation);
        }
        if (shapeCount == 0) {
            GF3dRender_DrawModel(&mapProp->render->renderObj, &mapProp->translation, &baseRot, &mapProp->scale);
        } else {
            DrawModelShapewise(mapProp->render->model, &mapProp->translation, &baseRot, &mapProp->scale, buildModelShapes, mapProp->buildModel);
        }
    }
}
