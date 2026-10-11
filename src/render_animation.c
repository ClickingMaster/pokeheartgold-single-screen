#include "render_animation.h"

#include <string.h>

#include "global.h"

#include "geometry_math.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"

typedef struct RenderAnimation {
    u32 flags;
    fx32 frame;
    void *file;
    void *resource;
    NNSG3dAnmObj *object;
    NNSFndAllocator allocator;
} RenderAnimation;

typedef struct RenderModel {
    u32 flags;
    void *file;
    NNSG3dResMdlSet *set;
    NNSG3dResMdl *model;
    NNSG3dResTex *texture;
} RenderModel;

extern const VecFx32 gRenderObjUnitScale;
extern const MtxFx33 gRenderObjIdentityRotation;
void RenderAnimation_Clear(RenderAnimation *animation);
void RenderAnimation_InitOwned(RenderAnimation *animation, void *file, int unused);
void RenderAnimation_InitShared(RenderAnimation *animation, void *file, int unused);
void RenderAnimation_InitFromAnimation(RenderAnimation *animation, RenderAnimation *source, int unused);
void RenderAnimation_LoadFromNarc(RenderAnimation *animation, int unused, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd);
void RenderAnimation_SetFile(RenderAnimation *animation, void *file, int unused);
void RenderAnimation_FreeFile(RenderAnimation *animation);
void RenderAnimation_AllocObject(RenderAnimation *animation, NNSG3dResMdl *model, enum HeapID heapID);
void RenderAnimation_AllocObjectForModel(RenderAnimation *animation, RenderModel *model, enum HeapID heapID);
void RenderAnimation_InitObject(RenderAnimation *animation, NNSG3dResMdl *model, NNSG3dResTex *texture);
void RenderAnimation_InitObjectForModel(RenderAnimation *animation, RenderModel *model);
void RenderAnimation_InitForModel(RenderAnimation *animation, RenderModel *model, RenderAnimation *source, int unused, enum HeapID heapID);
void RenderAnimation_FreeObject(RenderAnimation *animation);
void RenderAnimation_Free(RenderAnimation *animation);
BOOL RenderAnimation_Advance(RenderAnimation *animation, fx32 step, BOOL loop);
BOOL RenderAnimation_IsDone(RenderAnimation *animation);
void RenderObj_Clear(NNSG3dRenderObj *object);
void RenderObj_Init(NNSG3dRenderObj *object, NNSG3dResMdl *model);
void RenderObj_InitFromModel(NNSG3dRenderObj *object, void *model);
void RenderObj_AddAnimObj(NNSG3dRenderObj *object, NNSG3dAnmObj *animation);
void RenderObj_AddAnimation(NNSG3dRenderObj *object, RenderAnimation *animation);
void RenderObj_InitWithAnimation(NNSG3dRenderObj *object, UnkOv01_021FFECC_sub *model, RenderAnimation *animation);
void RenderObj_Draw(NNSG3dRenderObj *object, const VecFx32 *position, const VecFx32 *scale, const MtxFx33 *rotation);
void RenderObj_DrawAt(int object, VecFx32 *position);
void RenderObj_DrawRotated(NNSG3dRenderObj *object, const VecFx32 *position, const VecFx32 *scale, const u16 *angles);

void RenderAnimation_Clear(RenderAnimation *animation) {
    memset(animation, 0, sizeof(*animation));
}

void RenderAnimation_InitOwned(RenderAnimation *animation, void *file, int unused) {
    RenderAnimation_SetFile(animation, file, unused);
    animation->flags |= 1;
}

void RenderAnimation_InitShared(RenderAnimation *animation, void *file, int unused) {
    RenderAnimation_SetFile(animation, file, unused);
    animation->flags &= ~1;
}

void RenderAnimation_InitFromAnimation(RenderAnimation *animation, RenderAnimation *source, int unused) {
    RenderAnimation_InitShared(animation, source->file, unused);
}

void RenderAnimation_LoadFromNarc(RenderAnimation *animation, int unused, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd) {
    void *file = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, atEnd);
    RenderAnimation_InitOwned(animation, file, unused);
}

void RenderAnimation_SetFile(RenderAnimation *animation, void *file, int unused) {
    RenderAnimation_Clear(animation);
    animation->file = file;
    animation->resource = NNS_G3dGetAnmByIdx(file, 0);
}

void RenderAnimation_FreeFile(RenderAnimation *animation) {
    if (animation->flags & 1) {
        Heap_Free(animation->file);
        animation->flags &= ~1;
    }
    animation->file = NULL;
    animation->resource = NULL;
}

void RenderAnimation_AllocObject(RenderAnimation *animation, NNSG3dResMdl *model, enum HeapID heapID) {
    HeapExp_FndInitAllocator(&animation->allocator, heapID, 4);
    animation->object = NNS_G3dAllocAnmObj(&animation->allocator, animation->resource, model);
    GF_ASSERT(animation->object != NULL);
}

void RenderAnimation_AllocObjectForModel(RenderAnimation *animation, RenderModel *model, enum HeapID heapID) {
    RenderAnimation_AllocObject(animation, model->model, heapID);
}

void RenderAnimation_InitObject(RenderAnimation *animation, NNSG3dResMdl *model, NNSG3dResTex *texture) {
    NNS_G3dAnmObjInit(animation->object, animation->resource, model, texture);
}

void RenderAnimation_InitObjectForModel(RenderAnimation *animation, RenderModel *model) {
    RenderAnimation_InitObject(animation, model->model, model->texture);
}

void RenderAnimation_InitForModel(RenderAnimation *animation, RenderModel *model, RenderAnimation *source, int unused, enum HeapID heapID) {
    RenderAnimation_InitFromAnimation(animation, source, unused);
    RenderAnimation_AllocObjectForModel(animation, model, heapID);
    RenderAnimation_InitObjectForModel(animation, model);
}

void RenderAnimation_FreeObject(RenderAnimation *animation) {
    if (animation->object) {
        NNS_G3dFreeAnmObj(&animation->allocator, animation->object);
        animation->object = NULL;
    }
}

void RenderAnimation_Free(RenderAnimation *animation) {
    RenderAnimation_FreeObject(animation);
    RenderAnimation_FreeFile(animation);
    RenderAnimation_Clear(animation);
}

BOOL RenderAnimation_Advance(RenderAnimation *animation, fx32 step, BOOL loop) {
    fx32 next = animation->frame;
    BOOL done = FALSE;
    fx32 frames = *(u16 *)((u8 *)animation->object->resAnm + 4) << 12;
    animation->frame = next + step;
    if (step > 0) {
        if (animation->frame >= frames) {
            done = TRUE;
            if (loop == TRUE) {
                animation->frame -= frames;
            } else {
                animation->frame = frames;
            }
        }
    } else {
        if (animation->frame <= 0) {
            done = TRUE;
            if (loop == TRUE) {
                animation->frame += frames;
            } else {
                animation->frame = 0;
            }
        }
    }
    animation->object->frame = animation->frame;
    if (done == TRUE) {
        animation->flags |= 2;
    } else {
        animation->flags &= ~2;
    }
    return done;
}

BOOL RenderAnimation_IsDone(RenderAnimation *animation) {
    if (animation->flags & 2) {
        return TRUE;
    }
    return FALSE;
}

void RenderObj_Clear(NNSG3dRenderObj *object) {
    memset(object, 0, sizeof(*object));
}

void RenderObj_Init(NNSG3dRenderObj *object, NNSG3dResMdl *model) {
    RenderObj_Clear(object);
    NNS_G3dRenderObjInit(object, model);
}

void RenderObj_InitFromModel(NNSG3dRenderObj *object, void *model) {
    RenderObj_Init(object, *(NNSG3dResMdl **)((u8 *)model + 0xC));
}

void RenderObj_AddAnimObj(NNSG3dRenderObj *object, NNSG3dAnmObj *animation) {
    NNS_G3dRenderObjAddAnmObj(object, animation);
}

void RenderObj_AddAnimation(NNSG3dRenderObj *object, RenderAnimation *animation) {
    RenderObj_AddAnimObj(object, animation->object);
}

void RenderObj_InitWithAnimation(NNSG3dRenderObj *object, UnkOv01_021FFECC_sub *model, RenderAnimation *animation) {
    RenderObj_InitFromModel(object, model);
    RenderObj_AddAnimation(object, animation);
}

void RenderObj_Draw(NNSG3dRenderObj *object, const VecFx32 *position, const VecFx32 *scale, const MtxFx33 *rotation) {
    GF3dRender_DrawModel(object, position, rotation, scale);
}

void RenderObj_DrawAt(int object, VecFx32 *position) {
    RenderObj_Draw((NNSG3dRenderObj *)object, position, &gRenderObjUnitScale, &gRenderObjIdentityRotation);
}

void RenderObj_DrawRotated(NNSG3dRenderObj *object, const VecFx32 *position, const VecFx32 *scale, const u16 *angles) {
    MtxFx33 rotation;
    Math_CalcRotationMatrixXYZDeg(&rotation, angles[0], angles[1], angles[2]);
    RenderObj_Draw(object, position, scale, &rotation);
}
