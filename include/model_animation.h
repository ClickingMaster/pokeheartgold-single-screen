#ifndef POKEHEARTGOLD_MODEL_ANIMATION_H
#define POKEHEARTGOLD_MODEL_ANIMATION_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "sys_task_api.h"

// A model file (first model of its set), an animation bound to it, and a drawable instance.
typedef struct ModelResource {
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture; // NULL when the file has no textures
} ModelResource;

typedef struct ModelAnimation {
    void *file;
    void *resource;
    NNSG3dAnmObj *object;
    fx32 frame;
    BOOL borrowed; // the caller owns the file (ModelAnimation_InitFromFile)
} ModelAnimation;

typedef struct ModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3]; // x, y, z angles
} ModelInstance;

void ModelResource_LoadFromNarc(ModelResource *model, NARC *narc, int fileId, enum HeapID heapID);
void ModelResource_LoadTexturesTask(SysTask *task, void *data);
void ModelResource_Free(ModelResource *model);
void ModelAnimation_LoadFromNarc(ModelAnimation *animation, ModelResource *model, NARC *narc, int fileId, enum HeapID heapID, NNSFndAllocator *allocator);
void ModelAnimation_InitFromFile(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator);
void ModelAnimation_Free(ModelAnimation *animation, NNSFndAllocator *allocator);
void ModelAnimation_AdvanceLooped(ModelAnimation *animation, fx32 step);
BOOL ModelAnimation_Advance(ModelAnimation *animation, fx32 step);
void ModelAnimation_SetFrame(ModelAnimation *animation, fx32 frame);
fx32 ModelAnimation_GetFrame(ModelAnimation *animation);
fx32 ModelAnimation_GetFrameCount(ModelAnimation *animation);
void ModelInstance_Init(ModelInstance *instance, ModelResource *model);
void ModelInstance_AddAnimation(ModelInstance *instance, ModelAnimation *animation);
void ModelInstance_RemoveAnimation(ModelInstance *instance, ModelAnimation *animation);
void ModelInstance_Draw(ModelInstance *instance);
void ModelInstance_DrawWithRotation(ModelInstance *instance, const MtxFx33 *rotation);
void ModelInstance_SetVisible(ModelInstance *instance, BOOL visible);
BOOL ModelInstance_IsVisible(ModelInstance *instance);
void ModelInstance_SetPosition(ModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_GetPosition(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetScale(ModelInstance *instance, fx32 x, fx32 y, fx32 z);
void ModelInstance_GetScale(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void ModelInstance_SetRotation(ModelInstance *instance, u16 rotation, int axis);
u16 ModelInstance_GetRotation(ModelInstance *instance, int axis);
void ModelAnimation_Init(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator);
void ModelResource_Init(ModelResource *model);

#endif // POKEHEARTGOLD_MODEL_ANIMATION_H
