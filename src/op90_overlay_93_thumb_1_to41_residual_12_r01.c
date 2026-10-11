#pragma force_active on
#include "global.h"
#include "gf_3d_render.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "sys_task_api.h"
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

typedef struct {
    u32 active;
    ModelInstance instance;
    ModelAnimation animation;
} Ov93Cell;
typedef struct {
    ModelResource resource;
    ModelInstance instance;
    ModelResource templates[4];
    Ov93Cell cells[8][4];
} Ov93Bank;
typedef struct {
    ModelResource resource;
    ModelInstance instance;
} Ov93Footer;
typedef struct {
    u8 pad0[0xA8];
    NNSFndAllocator allocator;
    u8 padB8[0xD8 - 0xA8 - sizeof(NNSFndAllocator)];
    Ov93Bank bank;
    Ov93Footer footer;
} Ov93Work;
typedef struct {
    int file[5];
} FileRow;
extern const FileRow ov93_02262AF0[], ov93_02262AF4[], ov93_02262AF8[], ov93_02262AFC[], ov93_02262B00[];
extern int Comm_GetLocalNetId(void);
extern int ov93_0225E45C(Ov93Work *, int);
void ov93_0225DD2C(Ov93Work *work, NARC *narc);
// @fn ov93_0225DD2C
void ov93_0225DD2C(Ov93Work *work, NARC *narc) {
    void *shared;
    int idx;
    Ov93Bank *bank;
    int row;
    int col;
    Ov93Footer *footer;

    bank = &work->bank;
    footer = &work->footer;
    shared = NULL;
    idx = ov93_0225E45C(work, Comm_GetLocalNetId());

    ModelResource_LoadFromNarc(&bank->resource, narc, ov93_02262AF0[idx].file[0], HEAP_ID_117);
    ModelInstance_Init(&bank->instance, &bank->resource);
    ModelInstance_SetPosition(&bank->instance, 0, -102400, 0);
    ModelInstance_SetScale(&bank->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&bank->instance, TRUE);
    ModelResource_LoadFromNarc(&bank->templates[0], narc, ov93_02262AF4[idx].file[0], HEAP_ID_117);
    ModelResource_LoadFromNarc(&bank->templates[1], narc, ov93_02262AF8[idx].file[0], HEAP_ID_117);
    ModelResource_LoadFromNarc(&bank->templates[2], narc, ov93_02262AFC[idx].file[0], HEAP_ID_117);
    ModelResource_LoadFromNarc(&bank->templates[3], narc, ov93_02262B00[idx].file[0], HEAP_ID_117);
    row = 0;
    for (; row < 8; row++) {
        for (col = 0; col < 4; col++) {
            if (shared == NULL) {
                ModelAnimation_LoadFromNarc(&bank->cells[row][col].animation, &bank->templates[col], narc, col + 35, HEAP_ID_117, &work->allocator);
                shared = bank->cells[row][col].animation.file;
            } else {
                ModelAnimation_InitFromFile(&bank->cells[row][col].animation, &bank->templates[col], shared, &work->allocator);
            }
            ModelAnimation_SetFrame(&bank->cells[row][col].animation, 0);
        }
    }
    ModelResource_LoadFromNarc(&footer->resource, narc, 30, HEAP_ID_117);
    ModelInstance_Init(&footer->instance, &footer->resource);
    ModelInstance_SetPosition(&footer->instance, 0, -102400, 0);
    ModelInstance_SetScale(&footer->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    ModelInstance_SetVisible(&footer->instance, TRUE);
}
