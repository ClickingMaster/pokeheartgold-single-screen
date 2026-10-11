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

    sub_02018030(&bank->resource, narc, ov93_02262AF0[idx].file[0], HEAP_ID_117);
    sub_020181B0(&bank->instance, &bank->resource);
    sub_020182A8(&bank->instance, 0, -102400, 0);
    sub_020182C4(&bank->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    sub_020182A0(&bank->instance, TRUE);
    sub_02018030(&bank->templates[0], narc, ov93_02262AF4[idx].file[0], HEAP_ID_117);
    sub_02018030(&bank->templates[1], narc, ov93_02262AF8[idx].file[0], HEAP_ID_117);
    sub_02018030(&bank->templates[2], narc, ov93_02262AFC[idx].file[0], HEAP_ID_117);
    sub_02018030(&bank->templates[3], narc, ov93_02262B00[idx].file[0], HEAP_ID_117);
    row = 0;
    for (; row < 8; row++) {
        for (col = 0; col < 4; col++) {
            if (shared == NULL) {
                sub_020180BC(&bank->cells[row][col].animation, &bank->templates[col], narc, col + 35, HEAP_ID_117, &work->allocator);
                shared = bank->cells[row][col].animation.file;
            } else {
                sub_020180E8(&bank->cells[row][col].animation, &bank->templates[col], shared, &work->allocator);
            }
            sub_02018198(&bank->cells[row][col].animation, 0);
        }
    }
    sub_02018030(&footer->resource, narc, 30, HEAP_ID_117);
    sub_020181B0(&footer->instance, &footer->resource);
    sub_020182A8(&footer->instance, 0, -102400, 0);
    sub_020182C4(&footer->instance, FX32_ONE, FX32_ONE, FX32_ONE);
    sub_020182A0(&footer->instance, TRUE);
}
