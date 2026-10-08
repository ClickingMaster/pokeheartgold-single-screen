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
void sub_02018030(ModelResource *model, NARC *narc, int fileId, enum HeapID heapID);
void sub_0201804C(SysTask *task, void *data);
void sub_02018068(ModelResource *model);
void sub_020180BC(ModelAnimation *animation, ModelResource *model, NARC *narc, int fileId, enum HeapID heapID, NNSFndAllocator *allocator);
void sub_020180E8(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator);
void sub_020180F8(ModelAnimation *animation, NNSFndAllocator *allocator);
void sub_02018124(ModelAnimation *animation, fx32 step);
BOOL sub_0201815C(ModelAnimation *animation, fx32 step);
void sub_02018198(ModelAnimation *animation, fx32 frame);
fx32 sub_020181A0(ModelAnimation *animation);
fx32 sub_020181A4(ModelAnimation *animation);
void sub_020181B0(ModelInstance *instance, ModelResource *model);
void sub_020181D4(ModelInstance *instance, ModelAnimation *animation);
void sub_020181E0(ModelInstance *instance, ModelAnimation *animation);
void sub_020181EC(ModelInstance *instance);
void sub_02018288(ModelInstance *instance, const MtxFx33 *rotation);
void sub_020182A0(ModelInstance *instance, BOOL visible);
BOOL sub_020182A4(ModelInstance *instance);
void sub_020182A8(ModelInstance *instance, fx32 x, fx32 y, fx32 z);
void sub_020182B0(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void sub_020182C4(ModelInstance *instance, fx32 x, fx32 y, fx32 z);
void sub_020182CC(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
void sub_020182E0(ModelInstance *instance, u16 rotation, int axis);
u16 sub_020182EC(ModelInstance *instance, int axis);
void sub_020182F8(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator);
void sub_02018324(ModelResource *model);
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
        sub_02018030(&w->resources[i], ctx, ov91_02261C04[i], heap);
        sub_020181B0(&w->models[i], &w->resources[i]);
        sub_020182A0(&w->models[i], FALSE);
        sub_020182A8(&w->models[i], 0, -0x122000, 0);
        sub_020182C4(&w->models[i], 0x1800, 0x1800, 0x1800);
    }
    for (i = 0; i < 6; i++) {
        sub_020180BC(&w->animations[i], &w->resources[ov91_0226274C[i]], ctx, ov91_02261C1C[i], heap, allocator);
        sub_02018198(&w->animations[i], 0);
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
    sub_020180BC(special, &w->resources[modelId], ctx, 32, heap, allocator);
    sub_020181D4(&w->models[modelId], special);
    w->selected = 4;
    w->unused22e = 0;
    ov91_02260334(w, 0);
    sub_020181D4(&w->models[1], &w->animations[4]);
    sub_02018198(&w->animations[4], 0);
    sub_020182A0(&w->models[1], TRUE);
    w->current = 4;
}
