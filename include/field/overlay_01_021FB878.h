#ifndef POKEHEARTGOLD_FIELD_OVERLAY_01_021FB878_H
#define POKEHEARTGOLD_FIELD_OVERLAY_01_021FB878_H

#include "field/area_data.h"

#include "filesystem_files_def.h"
#include "heap.h"
#include "sys_task.h"

typedef struct Field3dModel {
    void *dataRaw;
    NNSG3dResMdlSet *modelSet;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} Field3dModel;

typedef struct Field3DModelAnimation {
    void *fileDataRaw;
    void *animRaw;
    NNSG3dAnmObj *animObj;
    fx32 frame;
    BOOL shouldRetainData; // if FALSE, fileDataRaw is freed on unload
} Field3DModelAnimation;

typedef struct Field3dObject {
    NNSG3dRenderObj renderObj;
    VecFx32 matrix;
    VecFx32 scale;
    BOOL active;
    u16 rotation[3];
    u16 padding;
} Field3dObject;

void Field3dModel_LoadFromFilesystem(Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID);
void Field3dModel_LoadFromData(Field3dModel *model, void *data);
void Field3dModel_LoadTexturesTask(SysTask *task, void *data);
void Field3dModel_Unload(Field3dModel *model);
void Field3dModel_UnloadKeepData(Field3dModel *model);
void Field3dModelAnimation_LoadFromFilesystem(Field3DModelAnimation *anim, const Field3dModel *model, NarcId narcId, s32 fileId, enum HeapID heapID, NNSFndAllocator *alloc);
void Field3dModelAnimation_LoadFromData(Field3DModelAnimation *anim, Field3dModel *model, void *animRaw, NNSFndAllocator *alloc);
void Field3dModelAnimation_Unload(Field3DModelAnimation *anim, NNSFndAllocator *alloc);
void Field3dModelAnimation_FrameAdvanceAndLoop(Field3DModelAnimation *anim, fx32 delta);
BOOL Field3dModelAnimation_FrameAdvanceAndCheck(Field3DModelAnimation *anim, fx32 delta);
void Field3dModelAnimation_FrameSet(Field3DModelAnimation *anim, fx32 frame);
fx32 Field3dModelAnimation_FrameGet(Field3DModelAnimation *anim);
void Field3dObject_InitFromModel(Field3dObject *object, Field3dModel *model);
void Field3dObject_AddAnimation(Field3dObject *object, Field3DModelAnimation *anim);
void Field3dObject_RemoveAnimation(Field3dObject *object, Field3DModelAnimation *anim);
void Field3dObject_Draw(Field3dObject *object);
void Field3dObject_SetActiveFlag(Field3dObject *object, BOOL active);
BOOL Field3dObject_GetActiveFlag(Field3dObject *object);
void Field3dObject_SetPosEx(Field3dObject *object, fx32 x, fx32 y, fx32 z);
void Field3dObject_SetPos(Field3dObject *object, const VecFx32 *pos);
void Field3dModelAnimation_Init(Field3DModelAnimation *anim, const Field3dModel *model, void *fileData, NNSFndAllocator *alloc);

#endif // POKEHEARTGOLD_FIELD_OVERLAY_01_021FB878_H
