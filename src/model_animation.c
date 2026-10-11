#include "model_animation.h"

#include <string.h>

#include "global.h"

#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task_api.h"

void ModelResource_LoadFromNarc(ModelResource *model, NARC *narc, int fileId, enum HeapID heapID) {
    model->file = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, FALSE);
    ModelResource_Init(model);
}

void ModelResource_LoadTexturesTask(SysTask *task, void *data) {
    ModelResource *model = data;
    GF3dRender_AllocAndLoadTexResources(model->texture);
    GF3dRender_BindModelSet(model->file, model->texture);
    SysTask_Destroy(task);
}

void ModelResource_Free(ModelResource *model) {
    u8 *bytes;
    u32 count;
    if (model->texture) {
        NNSG3dTexKey key, key4x4;
        NNS_G3dTexReleaseTexKey(model->texture, &key, &key4x4);
        NNS_GfdFreeTexVram(key);
        NNS_GfdFreeTexVram(key4x4);
        NNS_GfdFreePlttVram(NNS_G3dPlttReleasePlttKey(model->texture));
    }
    if (model->file) {
        Heap_Free(model->file);
    }
    bytes = (u8 *)model;
    count = sizeof(*model);
    do {
        *bytes++ = 0;
    } while (--count);
}

void ModelAnimation_LoadFromNarc(ModelAnimation *animation, ModelResource *model, NARC *narc, int fileId, enum HeapID heapID, NNSFndAllocator *allocator) {
    void *file = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapID, FALSE);
    ModelAnimation_Init(animation, model, file, allocator);
    animation->borrowed = FALSE;
}

void ModelAnimation_InitFromFile(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator) {
    ModelAnimation_Init(animation, model, file, allocator);
    animation->borrowed = TRUE;
}

void ModelAnimation_Free(ModelAnimation *animation, NNSFndAllocator *allocator) {
    u8 *bytes;
    u32 count;
    if (animation->file) {
        NNS_G3dFreeAnmObj(allocator, animation->object);
        if (!animation->borrowed) {
            Heap_Free(animation->file);
        }
    }
    bytes = (u8 *)animation;
    count = sizeof(*animation);
    do {
        *bytes++ = 0;
    } while (--count);
}

void ModelAnimation_AdvanceLooped(ModelAnimation *animation, fx32 step) {
    fx32 frames = NNS_G3dAnmObjGetNumFrame(animation->object);
    if (step > 0) {
        animation->frame = (animation->frame + step) % frames;
    } else {
        animation->frame += step;
        if (animation->frame < 0) {
            animation->frame += frames;
        }
    }
    animation->object->frame = animation->frame;
}

BOOL ModelAnimation_Advance(ModelAnimation *animation, fx32 step) {
    fx32 frames = NNS_G3dAnmObjGetNumFrame(animation->object);
    BOOL done = FALSE;
    if (step > 0) {
        if (animation->frame + step < frames) {
            animation->frame += step;
        } else {
            animation->frame = frames;
            done = TRUE;
        }
    } else {
        if (animation->frame + step >= 0) {
            animation->frame += step;
        } else {
            animation->frame = 0;
            done = TRUE;
        }
    }
    animation->object->frame = animation->frame;
    return done;
}

void ModelAnimation_SetFrame(ModelAnimation *animation, fx32 frame) {
    animation->frame = frame;
    animation->object->frame = frame;
}

fx32 ModelAnimation_GetFrame(ModelAnimation *animation) {
    return animation->frame;
}

fx32 ModelAnimation_GetFrameCount(ModelAnimation *animation) {
    return NNS_G3dAnmObjGetNumFrame(animation->object);
}

void ModelInstance_Init(ModelInstance *instance, ModelResource *model) {
    memset(instance, 0, sizeof(*instance));
    NNS_G3dRenderObjInit(&instance->render, model->model);
    instance->visible = TRUE;
    instance->scale.x = FX32_ONE;
    instance->scale.y = FX32_ONE;
    instance->scale.z = FX32_ONE;
}

void ModelInstance_AddAnimation(ModelInstance *instance, ModelAnimation *animation) {
    NNS_G3dRenderObjAddAnmObj(&instance->render, animation->object);
}

void ModelInstance_RemoveAnimation(ModelInstance *instance, ModelAnimation *animation) {
    NNS_G3dRenderObjRemoveAnmObj(&instance->render, animation->object);
}

void ModelInstance_Draw(ModelInstance *instance) {
    MtxFx33 rotation, part;
    if (instance->visible) {
        MTX_Identity33(&rotation);
        MTX_RotX33(&part, FX_SinIdx(instance->rotation[0]), FX_CosIdx(instance->rotation[0]));
        MTX_Concat33(&part, &rotation, &rotation);
        MTX_RotZ33(&part, FX_SinIdx(instance->rotation[2]), FX_CosIdx(instance->rotation[2]));
        MTX_Concat33(&part, &rotation, &rotation);
        MTX_RotY33(&part, FX_SinIdx(instance->rotation[1]), FX_CosIdx(instance->rotation[1]));
        MTX_Concat33(&part, &rotation, &rotation);
        GF3dRender_DrawModel(&instance->render, &instance->position, &rotation, &instance->scale);
    }
}

void ModelInstance_DrawWithRotation(ModelInstance *instance, const MtxFx33 *rotation) {
    if (instance->visible) {
        GF3dRender_DrawModel(&instance->render, &instance->position, rotation, &instance->scale);
    }
}

void ModelInstance_SetVisible(ModelInstance *instance, BOOL visible) {
    instance->visible = visible;
}

BOOL ModelInstance_IsVisible(ModelInstance *instance) {
    return instance->visible;
}

void ModelInstance_SetPosition(ModelInstance *instance, fx32 x, fx32 y, fx32 z) {
    instance->position.x = x;
    instance->position.y = y;
    instance->position.z = z;
}

void ModelInstance_GetPosition(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z) {
    *x = instance->position.x;
    *y = instance->position.y;
    *z = instance->position.z;
}

void ModelInstance_SetScale(ModelInstance *instance, fx32 x, fx32 y, fx32 z) {
    instance->scale.x = x;
    instance->scale.y = y;
    instance->scale.z = z;
}

void ModelInstance_GetScale(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z) {
    *x = instance->scale.x;
    *y = instance->scale.y;
    *z = instance->scale.z;
}

void ModelInstance_SetRotation(ModelInstance *instance, u16 rotation, int axis) {
    instance->rotation[axis] = rotation;
}

u16 ModelInstance_GetRotation(ModelInstance *instance, int axis) {
    return instance->rotation[axis];
}

void ModelAnimation_Init(ModelAnimation *animation, ModelResource *model, void *file, NNSFndAllocator *allocator) {
    animation->file = file;
    animation->resource = NNS_G3dGetAnmByIdx(file, 0);
    animation->object = NNS_G3dAllocAnmObj(allocator, animation->resource, model->model);
    NNS_G3dAnmObjInit(animation->object, animation->resource, model->model, model->texture);
}

void ModelResource_Init(ModelResource *model) {
    GF_ASSERT(model->file != NULL);
    model->set = NNS_G3dGetMdlSet(model->file);
    model->model = NNS_G3dGetMdlByIdx(model->set, 0);
    model->texture = NNS_G3dGetTex(model->file);
    if (model->texture) {
        SysTask_CreateOnVWaitQueue(ModelResource_LoadTexturesTask, model, 1024);
    }
}
