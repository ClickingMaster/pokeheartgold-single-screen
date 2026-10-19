#include "field/area_data.h"

#include <nnsys/g3d.h>
#include <nnsys/gfd.h>
#include <string.h>

#include "global.h"

#include "field/build_model_shapes.h"
#include "field/field_3d_model.h"
#include "field/map_prop_animation.h"
#include "field/map_prop_render.h"

#include "assert.h"
#include "filesystem.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task.h"
#include "sys_task_api.h"

#define AREA_DATA_MAX_MAP_PROPS 550

// The header of an area, member <areaDataBank> of NARC_a_0_4_2.
typedef struct AreaDataHeader {
    u16 mapPropNarcMemberId; // map prop id list (NARC_a_0_4_3) and map prop textures (NARC_a_0_7_0)
    u16 mapTexNarcMemberId;  // map textures (NARC_a_0_4_4)
    u16 mapPropAnimFileId;   // 0xFFFF = none
    u8 buildModelKind;       // 0 = room, otherwise field
    u8 lightKind;
} AreaDataHeader;

// Scratch state between AreaDataManager_Alloc and AreaDataManager_Load.
typedef struct AreaDataLoadContext {
    u32 areaDataBank;
    u32 unk_04;
    NARC *mapPropModelNarc;
    u16 mapPropCount;
    u16 unk_0E;
} AreaDataLoadContext;

struct AreaDataManager {
    u32 unk_00;
    NNSG3dResFileHeader *mapPropModels[AREA_DATA_MAX_MAP_PROPS];
    void *mapTexFile;
    void *mapPropTexFile;
    NNSG3dResTex *mapTex;
    NNSG3dResTex *mapPropTex;
    BuildModelShapes *buildModelShapes;
    AreaDataHeader header;
    AreaDataLoadContext *loadCtx;
    u16 *mapPropIdList; // count, then the ids
    NARC *mapPropAnimListNarc;
};

typedef struct FieldAnmRes {
    u32 unk_00;
    u16 numFrames;
    u16 unk_06;
} FieldAnmRes;

int sprintf(char *buf, const char *fmt, ...);

NNSG3dResFileHeader **AreaDataManager_GetMapPropModel(u32 mapPropId, AreaDataManager *areaDataManager);
BOOL AreaDataManager_IsLit(AreaDataManager *areaDataManager);
void Field3dObject_SetXRotation(Field3dObject *object, u16 angle, int axis);

static char sBuildModelShapesRoomPath[44] = "fielddata/build_model/bm_room_matshp.dat";

static char sBuildModelShapesFieldPath[44] = "fielddata/build_model/bm_field_matshp.dat";

static const u16 sAreaEdgeColorTableLightKindN[8] = { 0x1084, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084 };

static const u16 sAreaEdgeColorTableLightKind0[8] = { 0, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084, 0x1084 };

// Once a texture file's images are in VRAM, only its header and dictionaries are kept on the heap.
void AreaDataManager_TrimTextureFile(void *file, NNSG3dResTex *tex) {
    Heap_Realloc(file, (u32)tex + tex->texInfo.ofsTex - (u32)file);
}

AreaDataManager *AreaDataManager_Alloc(u8 areaDataBank) {
    AreaDataManager *areaDataManager = Heap_Alloc(HEAP_ID_FIELD1, sizeof(AreaDataManager));
    areaDataManager->loadCtx = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(AreaDataLoadContext));
    areaDataManager->loadCtx->areaDataBank = areaDataBank;
    areaDataManager->loadCtx->unk_04 = 0;
    ReadWholeNarcMemberByIdPair(&areaDataManager->header, NARC_a_0_4_2, areaDataBank);
    if (areaDataManager->header.buildModelKind != 0) {
        areaDataManager->mapPropAnimListNarc = NARC_New(NARC_a_1_0_7, HEAP_ID_FIELD1);
        areaDataManager->loadCtx->mapPropModelNarc = NARC_New(NARC_a_0_4_0, HEAP_ID_FIELD1);
    } else {
        areaDataManager->mapPropAnimListNarc = NARC_New(NARC_a_1_0_8, HEAP_ID_FIELD1);
        areaDataManager->loadCtx->mapPropModelNarc = NARC_New(NARC_a_1_4_8, HEAP_ID_FIELD1);
    }
    return areaDataManager;
}

NARC *AreaDataManager_GetMapPropAnimListNarc(AreaDataManager *areaDataManager) {
    return areaDataManager->mapPropAnimListNarc;
}

// Map props that are not loaded in this area fall back to map prop 0.
NNSG3dResFileHeader **AreaDataManager_GetMapPropModel(u32 mapPropId, AreaDataManager *areaDataManager) {
    GF_ASSERT(areaDataManager != NULL);
    if (areaDataManager->mapPropModels[mapPropId] == NULL) {
        GF_ASSERT(areaDataManager->mapPropModels[0] != NULL);
        return &areaDataManager->mapPropModels[0];
    }
    return &areaDataManager->mapPropModels[mapPropId];
}

NNSG3dResFileHeader **AreaDataManager_GetMapPropModelFile(AreaDataManager *areaDataManager) {
    GF_ASSERT(areaDataManager != NULL);
    return &areaDataManager->mapPropModels[0];
}

void AreaDataManager_Free(AreaDataManager **areaDataManager) {
    int i;

    NARC_Delete((*areaDataManager)->mapPropAnimListNarc);
    if ((*areaDataManager)->buildModelShapes != NULL) {
        BuildModelShapes_Free((*areaDataManager)->buildModelShapes);
    }
    for (i = 0; i < AREA_DATA_MAX_MAP_PROPS; i++) {
        if ((*areaDataManager)->mapPropModels[i] != NULL) {
            Heap_Free((*areaDataManager)->mapPropModels[i]);
        }
    }
    Heap_Free((*areaDataManager)->mapPropIdList);
    Heap_Free((*areaDataManager)->mapTexFile);
    (*areaDataManager)->mapTexFile = NULL;
    Heap_Free((*areaDataManager)->mapPropTexFile);
    (*areaDataManager)->mapPropTexFile = NULL;
    Heap_Free(*areaDataManager);
    *areaDataManager = NULL;
}

NNSG3dResTex *AreaDataManager_GetMapTexture(AreaDataManager *areaDataManager) {
    GF_ASSERT(areaDataManager != NULL);
    return areaDataManager->mapTex;
}

NNSG3dResTex *AreaDataManager_GetMapPropTexture(AreaDataManager *areaDataManager) {
    GF_ASSERT(areaDataManager != NULL);
    return areaDataManager->mapPropTex;
}

BuildModelShapes *AreaDataManager_GetBuildModelShapes(AreaDataManager *areaDataManager) {
    return areaDataManager->buildModelShapes;
}

BOOL AreaDataManager_IsLit(AreaDataManager *areaDataManager) {
    if (areaDataManager->header.lightKind != 0) {
        return TRUE;
    }
    return FALSE;
}

u32 AreaDataManager_GetAreaLightArchiveID(AreaDataManager *areaDataManager) {
    switch (areaDataManager->header.lightKind) {
    case 0:
        return 1;
    case 1:
        return 0;
    case 2:
        return 3;
    }
    return 0;
}

void AreaDataManager_Load(AreaDataManager *areaDataManager, Field3dRenderObjManager *renderObjManager, MapPropAnimationManager *mapPropAnimationManager, AreaMapAnimation *areaMapAnimation, TimeOfDayPropAnimManager *timeOfDayPropAnims) {
    MapPropAnimListFile animListFile;
    char path[256];
    AreaDataLoadContext *ctx = areaDataManager->loadCtx;
    int i;
    int count;
    s32 animListFileCount;

    areaDataManager->mapPropIdList = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_4_3, areaDataManager->header.mapPropNarcMemberId, HEAP_ID_FIELD1);
    ctx->mapPropCount = areaDataManager->mapPropIdList[0];
    if (ctx->mapPropCount >= AREA_DATA_MAX_MAP_PROPS) {
        GF_AssertFail();
    }
    areaDataManager->mapTexFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_4_4, areaDataManager->header.mapTexNarcMemberId, HEAP_ID_FIELD1);
    areaDataManager->mapPropTexFile = AllocAndReadWholeNarcMemberByIdPair(NARC_a_0_7_0, areaDataManager->header.mapPropNarcMemberId, HEAP_ID_FIELD1);
    areaDataManager->mapTex = NNS_G3dGetTex(areaDataManager->mapTexFile);
    if (ctx->mapPropCount) {
        areaDataManager->mapPropTex = NNS_G3dGetTex(areaDataManager->mapPropTexFile);
    } else {
        areaDataManager->mapPropTex = NULL;
    }
    if (!GF3dRender_AllocAndLoadTexResources(areaDataManager->mapTex)) {
        GF_AssertFail();
    }
    AreaDataManager_TrimTextureFile(areaDataManager->mapTexFile, areaDataManager->mapTex);
    if (areaDataManager->mapPropTex) {
        if (!GF3dRender_AllocAndLoadTexResources(areaDataManager->mapPropTex)) {
            GF_AssertFail();
        }
        AreaDataManager_TrimTextureFile(areaDataManager->mapPropTexFile, areaDataManager->mapPropTex);
    }
    if (areaDataManager->header.mapPropAnimFileId != 0xFFFF) {
        AreaMapAnimation_Load(areaMapAnimation, areaDataManager->header.mapPropAnimFileId);
    }
    for (i = 0; i < AREA_DATA_MAX_MAP_PROPS; i++) {
        areaDataManager->mapPropModels[i] = NULL;
    }
    areaDataManager->buildModelShapes = NULL;
    animListFileCount = MapPropAnimationManager_GetAnimListNARCFileCount(mapPropAnimationManager);
    for (count = 0; count < ctx->mapPropCount; count++) {
        u16 id = areaDataManager->mapPropIdList[count + 1];
        NNSG3dRenderObj *renderObj;

        if (areaDataManager->mapPropModels[id]) {
            GF_AssertFail();
        }
        areaDataManager->mapPropModels[id] = NARC_AllocAndReadWholeMember(ctx->mapPropModelNarc, id, HEAP_ID_FIELD1);
        NARC_ReadWholeMember(areaDataManager->mapPropAnimListNarc, id, &animListFile);
        if (!GF3dRender_BindModelSet(areaDataManager->mapPropModels[id], areaDataManager->mapPropTex)) {
            GF_AssertFail();
        }
        if (!animListFile.filler_3) {
            renderObj = (NNSG3dRenderObj *)Field3dObjectList_AddShared(renderObjManager, id);
            if (id < animListFileCount) {
                MapPropAnimationManager_LoadPropAnimations(id, NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(areaDataManager->mapPropModels[id]), 0), renderObj, areaDataManager->mapPropTex, &animListFile, mapPropAnimationManager, timeOfDayPropAnims);
            }
        }
    }
    if (!areaDataManager->mapPropModels[0]) {
        NNSG3dResTex *tex;

        areaDataManager->mapPropModels[0] = NARC_AllocAndReadWholeMember(ctx->mapPropModelNarc, 0, HEAP_ID_FIELD1);
        Field3dObjectList_AddShared(renderObjManager, 0);
        tex = NNS_G3dGetTex(areaDataManager->mapPropModels[0]);
        if (tex) {
            if (!GF3dRender_AllocAndLoadTexResources(tex)) {
                GF_AssertFail();
            }
            if (!GF3dRender_BindModelSet(areaDataManager->mapPropModels[0], tex)) {
                GF_AssertFail();
            }
        }
    }
    NARC_Delete(ctx->mapPropModelNarc);
    G3X_EdgeMarking(TRUE);
    if (!areaDataManager->header.lightKind) {
        G3X_SetEdgeColorTable(sAreaEdgeColorTableLightKind0);
    } else {
        G3X_SetEdgeColorTable(sAreaEdgeColorTableLightKindN);
    }
    areaDataManager->buildModelShapes = BuildModelShapes_New();
    if (areaDataManager->header.buildModelKind) {
        sprintf(path, sBuildModelShapesFieldPath);
    } else {
        sprintf(path, sBuildModelShapesRoomPath);
    }
    BuildModelShapes_Load(path, areaDataManager->buildModelShapes);
    Heap_Free(areaDataManager->loadCtx);
    areaDataManager->loadCtx = NULL;
}

void Field3dModel_LoadFromFilesystem(Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID) {
    model->dataRaw = GfGfxLoader_LoadFromNarc(narcId, fileId, FALSE, heapID, FALSE);
    model->modelSet = NNS_G3dGetMdlSet(model->dataRaw);
    model->model = NNS_G3dGetMdlByIdx(model->modelSet, 0);
    model->texture = NNS_G3dGetTex(model->dataRaw);
    if (model->texture != NULL) {
        SysTask_CreateOnVWaitQueue(Field3dModel_LoadTexturesTask, model, 1024);
    }
}

void Field3dModel_LoadFromData(Field3dModel *model, void *data) {
    model->dataRaw = data;
    model->modelSet = NNS_G3dGetMdlSet(data);
    model->model = NNS_G3dGetMdlByIdx(model->modelSet, 0);
    model->texture = NNS_G3dGetTex(model->dataRaw);
    if (model->texture != NULL) {
        SysTask_CreateOnVWaitQueue(Field3dModel_LoadTexturesTask, model, 1024);
    }
}

void Field3dModel_LoadTexturesTask(SysTask *task, void *data) {
    Field3dModel *model = data;
    GF3dRender_AllocAndLoadTexResources(model->texture);
    GF3dRender_BindModelSet(model->dataRaw, model->texture);
    SysTask_Destroy(task);
}

void Field3dModel_Unload(Field3dModel *model) {
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;

    if (model->texture != NULL) {
        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
    }
    if (model->dataRaw != NULL) {
        Heap_Free(model->dataRaw);
    }
    memset(model, 0, sizeof(Field3dModel));
}

// Like Field3dModel_Unload, but the model data belongs to the caller.
void Field3dModel_UnloadKeepData(Field3dModel *model) {
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;

    if (model->texture != NULL) {
        NNS_G3dTexReleaseTexKey(model->texture, &texKey, &tex4x4Key);
        NNS_GfdDefaultFuncFreeTexVram(texKey);
        NNS_GfdDefaultFuncFreeTexVram(tex4x4Key);
        NNS_GfdDefaultFuncFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
    }
    memset(model, 0, sizeof(Field3dModel));
}

void Field3dModelAnimation_LoadFromFilesystem(Field3DModelAnimation *anim, const Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID, NNSFndAllocator *alloc) {
    Field3dModelAnimation_Init(anim, model, GfGfxLoader_LoadFromNarc(narcId, fileId, FALSE, heapID, FALSE), alloc);
    anim->shouldRetainData = FALSE;
}

void Field3dModelAnimation_LoadFromData(Field3DModelAnimation *anim, Field3dModel *model, void *animRaw, NNSFndAllocator *alloc) {
    Field3dModelAnimation_Init(anim, model, animRaw, alloc);
    anim->shouldRetainData = TRUE;
}

void Field3dModelAnimation_Unload(Field3DModelAnimation *anim, NNSFndAllocator *alloc) {
    if (anim->fileDataRaw != NULL) {
        NNS_G3dFreeAnmObj(alloc, anim->animObj);
        if (!anim->shouldRetainData) {
            Heap_Free(anim->fileDataRaw);
        }
    }
    memset(anim, 0, sizeof(Field3DModelAnimation));
}

void Field3dModelAnimation_FrameAdvanceAndLoop(Field3DModelAnimation *anim, fx32 delta) {
    fx32 numFrames = ((const FieldAnmRes *)anim->animObj->resAnm)->numFrames << FX32_SHIFT;
    if (delta > 0) {
        anim->frame = (anim->frame + delta) % numFrames;
    } else {
        anim->frame += delta;
        if (anim->frame < 0) {
            anim->frame += numFrames;
        }
    }
    anim->animObj->frame = anim->frame;
}

BOOL Field3dModelAnimation_FrameAdvanceAndCheck(Field3DModelAnimation *anim, fx32 delta) {
    fx32 numFrames = ((const FieldAnmRes *)anim->animObj->resAnm)->numFrames << FX32_SHIFT;
    BOOL ret = FALSE;
    if (delta > 0) {
        if (anim->frame + delta < numFrames) {
            anim->frame = anim->frame + delta;
        } else {
            anim->frame = numFrames;
            ret = TRUE;
        }
    } else {
        if (anim->frame + delta >= 0) {
            anim->frame = anim->frame + delta;
        } else {
            anim->frame = 0;
            ret = TRUE;
        }
    }
    anim->animObj->frame = anim->frame;
    return ret;
}

void Field3dModelAnimation_FrameSet(Field3DModelAnimation *anim, fx32 frame) {
    anim->frame = frame;
    anim->animObj->frame = frame;
}

fx32 Field3dModelAnimation_FrameGet(Field3DModelAnimation *anim) {
    return anim->frame;
}

void Field3dObject_InitFromModel(Field3dObject *object, Field3dModel *model) {
    memset(object, 0, sizeof(Field3dObject));
    NNS_G3dRenderObjInit(&object->renderObj, model->model);
    object->active = TRUE;
    object->scale.x = FX32_ONE;
    object->scale.y = FX32_ONE;
    object->scale.z = FX32_ONE;
}

void Field3dObject_AddAnimation(Field3dObject *object, Field3DModelAnimation *anim) {
    NNS_G3dRenderObjAddAnmObj(&object->renderObj, anim->animObj);
}

void Field3dObject_RemoveAnimation(Field3dObject *object, Field3DModelAnimation *anim) {
    NNS_G3dRenderObjRemoveAnmObj(&object->renderObj, anim->animObj);
}

void Field3dObject_Draw(Field3dObject *object) {
    MtxFx33 rot;
    MtxFx33 tmp;

    if (object->active) {
        MTX_Identity33_(&rot);
        MTX_RotX33_(&tmp, FX_SinIdx(object->rotation[0]), FX_CosIdx(object->rotation[0]));
        MTX_Concat33(&tmp, &rot, &rot);
        MTX_RotZ33_(&tmp, FX_SinIdx(object->rotation[2]), FX_CosIdx(object->rotation[2]));
        MTX_Concat33(&tmp, &rot, &rot);
        MTX_RotY33_(&tmp, FX_SinIdx(object->rotation[1]), FX_CosIdx(object->rotation[1]));
        MTX_Concat33(&tmp, &rot, &rot);
        GF3dRender_DrawModel(&object->renderObj, &object->matrix, &rot, &object->scale);
    }
}

void Field3dObject_SetActiveFlag(Field3dObject *object, BOOL active) {
    object->active = active;
}

BOOL Field3dObject_GetActiveFlag(Field3dObject *object) {
    return object->active;
}

void Field3dObject_SetPosEx(Field3dObject *object, fx32 x, fx32 y, fx32 z) {
    object->matrix.x = x;
    object->matrix.y = y;
    object->matrix.z = z;
}

void Field3dObject_SetPos(Field3dObject *object, const VecFx32 *pos) {
    object->matrix = *pos;
}

void Field3dObject_SetXRotation(Field3dObject *object, u16 angle, int axis) {
    object->rotation[axis] = angle;
}

void Field3dModelAnimation_Init(Field3DModelAnimation *anim, const Field3dModel *model, void *fileData, NNSFndAllocator *alloc) {
    anim->fileDataRaw = fileData;
    anim->animRaw = NNS_G3dGetAnmByIdx(fileData, 0);
    anim->animObj = NNS_G3dAllocAnmObj(alloc, anim->animRaw, model->model);
    NNS_G3dAnmObjInit(anim->animObj, anim->animRaw, model->model, model->texture);
}
