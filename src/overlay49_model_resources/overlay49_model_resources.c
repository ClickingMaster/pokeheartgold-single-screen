#include <string.h>

#include "global.h"

#include "gf_3d_render.h"
#include "gf_3d_loader.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_42.h"
#include "unk_0201F990.h"

typedef struct ModelInstance {
    NNSG3dRenderObj render;
    VecFx32 position;
    VecFx32 scale;
    BOOL visible;
    u16 rotation[3];
} ModelInstance;

typedef struct Ov49Model {
    u8 padding[8];
    NNSG3dResMdl *model;
} Ov49Model;

typedef struct Ov49Resource {
    UnkStruct_ov42_02227F68 *resource;
} Ov49Resource;

extern void sub_020182B0(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
extern void sub_020182CC(ModelInstance *instance, fx32 *x, fx32 *y, fx32 *z);
extern u16 sub_020182EC(ModelInstance *instance, int axis);
void ov49_02258830(NNSG3dResFileHeader **dest, NARC *narc, int fileId, enum HeapID heapId);
BOOL ov49_022588A0(Ov49Model *model, ModelInstance *instance);
Ov49Resource *ov49_02258958(enum HeapID heapId);

void ov49_02258830(NNSG3dResFileHeader **dest, NARC *narc, int fileId, enum HeapID heapId) {
    NNSG3dResFileHeader *source;
    NNSG3dResTex *texture;
    NNSG3dResTex *newTexture;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey paletteKey;
    u32 size;

    source = GfGfxLoader_LoadFromOpenNarc(narc, fileId, FALSE, heapId, TRUE);
    texture = NNS_G3dGetTex(source);
    GF3dRender_AllocAndLoadTexResources(texture);
    NNS_G3dTexReleaseTexKey(texture, &texKey, &tex4x4Key);
    paletteKey = NNS_G3dPlttReleasePlttKey(texture);
    size = G3dResFileHeader_GetSizeWithoutTex(source);
    *dest = Heap_Alloc(heapId, size);
    memcpy(*dest, source, size);
    newTexture = NNS_G3dGetTex(*dest);
    NNS_G3dTexSetTexKey(newTexture, texKey, tex4x4Key);
    NNS_G3dPlttSetPlttKey(newTexture, paletteKey);
    Heap_Free(source);
}

BOOL ov49_022588A0(Ov49Model *model, ModelInstance *instance) {
    VecFx32 position;
    VecFx32 scale;
    MtxFx33 rotation;
    MtxFx33 part;
    u16 x;
    u16 y;
    u16 z;

    sub_020182B0(instance, &position.x, &position.y, &position.z);
    sub_020182CC(instance, &scale.x, &scale.y, &scale.z);
    x = sub_020182EC(instance, 0);
    y = sub_020182EC(instance, 1);
    z = sub_020182EC(instance, 2);
    MTX_Identity33_(&rotation);
    MTX_RotX33_(&part, FX_SinIdx(x), FX_CosIdx(x));
    MTX_Concat33(&part, &rotation, &rotation);
    MTX_RotZ33_(&part, FX_SinIdx(z), FX_CosIdx(z));
    MTX_Concat33(&part, &rotation, &rotation);
    MTX_RotY33_(&part, FX_SinIdx(y), FX_CosIdx(y));
    MTX_Concat33(&part, &rotation, &rotation);
    return sub_0201F990(model->model, &position, &rotation, &scale);
}

Ov49Resource *ov49_02258958(enum HeapID heapId) {
    Ov49Resource *result;
    void *file;

    result = Heap_Alloc(heapId, sizeof(*result));
    result->resource = ov42_02227EE0(0x23, 0x2A, heapId);
    file = GfGfxLoader_LoadFromNarc((NarcId)0xCA, 0, FALSE, heapId, TRUE);
    ov42_02227F48(result->resource, file);
    Heap_Free(file);
    return result;
}
