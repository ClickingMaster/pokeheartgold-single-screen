#pragma force_active on
#include "global.h"
#include "gf_3d_render.h"
#include "filesystem.h"
#include "heap.h"
#include "sys_task_api.h"
#include <stddef.h>
#include <string.h>
void NNS_G3dAnmObjDisableID(NNSG3dAnmObj*,int);
void NNS_G3dAnmObjEnableID(NNSG3dAnmObj*,int);
int NNS_G3dGetResDictIdxByName(const NNSG3dResDict*,const NNSG3dResName*);
typedef struct ModelResource {
    NNSG3dResFileHeader *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} ModelResource;
typedef struct ModelAnimation {
    void *file;
    void *resource;
    NNSG3dAnmObj *object;
    fx32 frame;
    BOOL borrowed;
} ModelAnimation;
typedef struct ModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3];
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
typedef struct Scene{ModelInstance models[3];ModelResource resources[3];ModelAnimation animations[6];u16 current,state;ModelAnimation special;u32 unused228;u16 selected,unused22e;}Scene;
typedef char SceneSize[sizeof(Scene)==0x230?1:-1];
typedef char ResourceOffset[offsetof(Scene,resources)==0x168?1:-1];
typedef char AnimationOffset[offsetof(Scene,animations)==0x198?1:-1];
typedef char SpecialOffset[offsetof(Scene,special)==0x214?1:-1];
extern const u16 ov91_02261C04[3],ov91_02261C1C[6];extern const u8 ov91_0226274C[6],_02261BE8[];extern const NNSG3dResName ov91_022621AC[6][15];
extern void ov91_02260334(void*,int);
void ov91_0225FF00(Scene*,NARC*,enum HeapID,NNSFndAllocator*);

static inline const NNSG3dResName *GetNameByIdx(const NNSG3dResDict *dict, u32 idx) {
    if (dict != NULL && idx < dict->numEntry) {
        const NNSG3dResDictEntryHeader *hdr = (const NNSG3dResDictEntryHeader *)((const u8 *)dict + dict->ofsEntry);
        return (const NNSG3dResName *)((const u8 *)hdr + hdr->ofsName + idx * sizeof(NNSG3dResName));
    }
    return NULL;
}

// @fn ov91_0225FF00
void ov91_0225FF00(Scene *w, NARC *ctx, enum HeapID heap, NNSFndAllocator *allocator) {
    int i;
    int j;
    int idx;
    BOOL missing;
    NNSG3dResMdl *mdl;
    const NNSG3dResDict *dict;
    int id;
    u8 modelId;
    ModelAnimation *special;

    memset(w, 0, sizeof(Scene));
    for (i = 0; i < 3; i++) {
        ModelResource_LoadFromNarc(&w->resources[i], ctx, ov91_02261C04[i], heap);
        ModelInstance_Init(&w->models[i], &w->resources[i]);
        ModelInstance_SetVisible(&w->models[i], FALSE);
        ModelInstance_SetPosition(&w->models[i], 0, -0x122000, 0);
        ModelInstance_SetScale(&w->models[i], 0x1800, 0x1800, 0x1800);
    }
    for (i = 0; i < 6; i++) {
        ModelAnimation_LoadFromNarc(&w->animations[i], &w->resources[ov91_0226274C[i]], ctx, ov91_02261C1C[i], heap, allocator);
        ModelAnimation_SetFrame(&w->animations[i], 0);
        idx = 0;
        mdl = w->resources[ov91_0226274C[i]].model;
        dict = mdl != NULL ? &mdl->nodeInfo.dict : NULL;
        missing = dict == NULL;
        while ((!missing ? GetNameByIdx(dict, idx) : NULL) != NULL) {
            NNS_G3dAnmObjDisableID(w->animations[i].object, idx);
            idx++;
        }
        for (j = 0; j < 15; j++) {
            id = dict != NULL ? NNS_G3dGetResDictIdxByName(dict, &ov91_022621AC[i][j]) : -1;
            if (id != -1) {
                NNS_G3dAnmObjEnableID(w->animations[i].object, id);
            }
        }
    }
    modelId = _02261BE8[0];
    special = &w->special;
    ModelAnimation_LoadFromNarc(special, &w->resources[modelId], ctx, 32, heap, allocator);
    ModelInstance_AddAnimation(&w->models[modelId], special);
    w->selected = 4;
    w->unused22e = 0;
    ov91_02260334(w, 0);
    ModelInstance_AddAnimation(&w->models[1], &w->animations[4]);
    ModelAnimation_SetFrame(&w->animations[4], 0);
    ModelInstance_SetVisible(&w->models[1], TRUE);
    w->current = 4;
}
