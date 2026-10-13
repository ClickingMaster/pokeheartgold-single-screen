#include "billboard_lists.h"

#include "global.h"

#include "billboard_internal.h"
#include "gf_3d_render.h"
#include "heap.h"

// asm/billboard_lists_data.s
extern BillboardListManager gBillboardLists;

// src/texture_animation.c, src/billboard_gfx_sequence.c, NitroSystem
void *TextureAnimationPool_New(int count, enum HeapID heapId);
void TextureAnimationPool_Free(void *arg);
void *TextureAnimationPool_Add(void *ctx, BillboardResourceData *resourceData, void *resourceHeader, NNSG3dTexKey texKey, NNSG3dPlttKey paletteKey, u32 unkB8);
void TextureAnimation_Remove(void *arg);
void TextureAnimation_SetFrame(void *, fx32);
u32 BillboardGfxSequence_GetTexPlttIndex(BillboardResourceData *, u32);
void NNS_G3dReleaseMdlSet(NNSG3dResMdlSet *);

void Billboard_Clear(Billboard *billboard);
void BillboardObject_Clear(BillboardObject *object);
Billboard *Billboard_Create(BillboardCreateParams *params);
BOOL Billboard_Delete(Billboard *billboard);
BOOL Billboard_DeleteAllObjects(Billboard *billboard);
BOOL Billboard_HasPendingFree(Billboard *billboard);
Billboard *BillboardLists_GetFree(void);
void Billboard_Draw(Billboard *billboard);
void BillboardObject_LoadResource(BillboardObject *object, BillboardResource *resource);
void BillboardObject_LoadResourceWithKeys(BillboardObject *object, BillboardResource *resource);
void Billboard_AllocTextureVram(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
void Billboard_ReleaseTextureKeys(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
void Billboard_BindTexture(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
void Billboard_FreeTextureVram(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
BOOL Billboard_TexturesNeedSameVram(NNSG3dResTex *a, NNSG3dResTex *b);
void BillboardObject_RemoveTextureAnimation(Billboard *billboard, BillboardObject *object);
void BillboardObject_LoadModel(BillboardObject *object, BillboardResource *resource);
void BillboardObject_AddTextureAnimation(Billboard *billboard, BillboardObject *object, BillboardResource *resource);
void BillboardObject_LoadModelWithKeys(BillboardObject *object, BillboardResource *resource);
void BillboardObject_SetResourceData(BillboardObject *object, BillboardResource *resource);
BillboardObject *BillboardObject_Create(BillboardObjectCreateParams *params);
BOOL BillboardObject_Delete(BillboardObject *object);
void BillboardResource_InitWithKeys(BillboardResource *, void *, u32, u32, BillboardResourceData *, u32, u32, u32);
void BillboardResource_Init(BillboardResource *, void *, u32, u32, BillboardResourceData *);
void BillboardObject_SetPosition(BillboardObject *, VecFx32 *);
VecFx32 *BillboardObject_GetPosition(BillboardObject *);
void BillboardObject_SetScale(BillboardObject *, VecFx32 *);
VecFx32 *BillboardObject_GetScale(BillboardObject *);
u32 BillboardObject_IsVisible(BillboardObject *);
void BillboardObject_SetResource(BillboardObject *, u32);
void BillboardObject_SetSequence(BillboardObject *, u32);
u32 BillboardObject_GetSequence(BillboardObject *);
void BillboardObject_AdvanceFrame(BillboardObject *, fx32);
void BillboardObject_SetFrame(BillboardObject *, fx32);
fx32 BillboardObject_GetFrame(BillboardObject *);
void BillboardObject_SetSequenceFrame(BillboardObject *, fx32);
fx32 BillboardObject_GetSequenceFrame(BillboardObject *);
NNSG3dTexKey BillboardObject_GetTexKey(BillboardObject *);
NNSG3dPlttKey BillboardObject_GetPlttKey(BillboardObject *);
void BillboardObject_UpdateTextureAnimation(BillboardObject *object);
NNSG3dResMdl *BillboardObject_GetModelNoCast(BillboardObject *);
void BillboardObject_SetCallback(BillboardObject *, BillboardCallback, void *);
void BillboardObject_UpdateTexPlttSequence(BillboardObject *object);
void Billboard_SetModelTexture(NNSG3dResMdl *, void *, u8);
void Billboard_OffsetMaterialTexImage(void *, void *, u32);
void Billboard_SetModelPltt(NNSG3dResMdl *, void *, u8);
void Billboard_OffsetMaterialPltt(void *, void *, u32);
void Billboard_InitObjectPool(Billboard *billboard);
BillboardObject *Billboard_AllocObject(Billboard *billboard);
BOOL Billboard_FreeObject(Billboard *billboard, BillboardObject *object);
void BillboardObject_InsertBefore(BillboardObject *root, BillboardObject *object);
void BillboardObject_Unlink(BillboardObject *object);
NNSG3dResMdlSet *BillboardResource_GetModelSet(BillboardResource *resource, NNSG3dResMdl **model, NNSG3dResTex **texture);
void *BillboardResource_GetAnimFile(BillboardResource *resource);
void BillboardObject_ApplyResource(BillboardObject *object, u32 resourceIndex);
fx32 BillboardObject_GetSequenceStart(BillboardObject *, u32);
void BillboardObject_StepFrame(BillboardObject *, fx32);
BOOL BillboardSequence_Step(void *, fx32 *, fx32);
void BillboardObject_BindTexture(BillboardObject *object);
void BillboardObject_ReleaseTexture(BillboardObject *object);
void *BillboardResource_GetFile(BillboardResource *, u32);
void *BillboardSequence_Get(void *, u32);

void Billboard_Clear(Billboard *billboard) {
    billboard->initialized = 0;
    billboard->draw = 0;
    billboard->objects = NULL;
    billboard->objectCount = 0;
    billboard->freeObjects = NULL;
    billboard->usedCount = 0;
    billboard->allocator = NULL;
    billboard->textureAnimationPool = NULL;
    billboard->state = 0;
}

void BillboardObject_Clear(BillboardObject *object) {
    object->owner = 0;
    object->resourceId = 0;
    object->modelSet = 0;
    object->model = 0;
    object->texture = 0;
    object->resourceHeader = 0;
    memset(&object->resourceData, 0, sizeof(object->resourceData));
    object->texKey = 0;
    object->tex4x4Key = 0;
    object->paletteKey = 0;
    object->textureAnimation = 0;
    object->position.x = 0;
    object->position.y = 0;
    object->position.z = 0;
    object->scale.x = FX32_ONE;
    object->scale.y = FX32_ONE;
    object->scale.z = FX32_ONE;
    object->rotation = 0;
    object->sequence = 0;
    object->type = 0;
    object->next = NULL;
    object->prev = 0;
    object->visible = 0;
    object->callback = 0;
}

void BillboardLists_Create(int count, enum HeapID heapID) {
    int i;

    GF_ASSERT(gBillboardLists.list == NULL);
    gBillboardLists.list = Heap_Alloc(heapID, count * sizeof(Billboard));
    gBillboardLists.count = count;
    for (i = 0; i < count; i++) {
        Billboard_Clear(&gBillboardLists.list[i]);
    }
}

void BillboardLists_Delete(void) {
    int i;

    for (i = 0; i < gBillboardLists.count; i++) {
        Billboard_Delete(&gBillboardLists.list[i]);
    }
    Heap_Free(gBillboardLists.list);
    gBillboardLists.list = NULL;
    gBillboardLists.count = 0;
}

void BillboardLists_Draw(void) {
    int i;

    for (i = 0; i < gBillboardLists.count; i++) {
        if (gBillboardLists.list[i].draw == 1) {
            Billboard_Draw(&gBillboardLists.list[i]);
        }
        if (gBillboardLists.list[i].state == 1) {
            gBillboardLists.list[i].state = 2;
        }
    }
}

Billboard *Billboard_Create(BillboardCreateParams *params) {
    Billboard *billboard = BillboardLists_GetFree();
    if (billboard == NULL) {
        GF_ASSERT(FALSE);
        return NULL;
    }
    billboard->initialized = TRUE;
    billboard->draw = TRUE;
    billboard->objects = Heap_Alloc(params->heapId, params->count * sizeof(BillboardObject));
    billboard->objectCount = params->count;
    BillboardObject_Clear(&billboard->root);
    billboard->root.next = &billboard->root;
    billboard->root.prev = &billboard->root;
    billboard->freeObjects = Heap_Alloc(params->heapId, params->count * sizeof(u32));
    Billboard_InitObjectPool(billboard);
    billboard->allocator = Heap_Alloc(params->heapId, sizeof(NNSFndAllocator));
    HeapExp_FndInitAllocator(billboard->allocator, params->heapId, 4);
    billboard->textureAnimationPool = TextureAnimationPool_New(params->count, params->heapId);
    return billboard;
}

BOOL Billboard_Delete(Billboard *billboard) {
    if (billboard == NULL) {
        GF_ASSERT(FALSE);
        return FALSE;
    }
    if (billboard->initialized != FALSE) {
        Billboard_DeleteAllObjects(billboard);
        Heap_Free(billboard->objects);
        Heap_Free(billboard->freeObjects);
        Heap_Free(billboard->allocator);
        TextureAnimationPool_Free(billboard->textureAnimationPool);
        Billboard_Clear(billboard);
    }
    return TRUE;
}

BOOL Billboard_DeleteAllObjects(Billboard *billboard) {
    BillboardObject *object;
    BillboardObject *next;
    if (billboard == NULL) {
        GF_ASSERT(billboard != NULL);
        return FALSE;
    }
    if (billboard->initialized != FALSE) {
        object = billboard->root.next;
        while (object != &billboard->root) {
            next = object->next;
            BillboardObject_Delete(object);
            object = next;
        }
    }
    return TRUE;
}

BOOL Billboard_HasPendingFree(Billboard *billboard) {
    GF_ASSERT(billboard != NULL);
    if (billboard->state == 0) {
        return FALSE;
    }
    return TRUE;
}

void Billboard_ClearPendingFree(void *arg0) {
    Billboard *billboard = arg0;
    if (billboard->state == 2) {
        billboard->state = 0;
    }
}

Billboard *BillboardLists_GetFree(void) {
    int i;
    for (i = 0; i < gBillboardLists.count; i++) {
        if (!gBillboardLists.list[i].initialized) {
            return &gBillboardLists.list[i];
        }
    }
    return NULL;
}

void Billboard_Draw(Billboard *billboard) {
    MtxFx33 identity;
    BillboardObject *object;
    GF_ASSERT(billboard != NULL);
    MTX_Identity33(&identity);
    object = billboard->root.next;
    while (object != &billboard->root) {
        if (object->visible == TRUE) {
            if (object->callback != NULL) {
                object->callback(object, object->callbackArg);
            }
            BillboardObject_BindTexture(object);
            if (object->type == 3) {
                BillboardObject_UpdateTexPlttSequence(object);
            } else if (object->type == 2) {
                BillboardObject_UpdateTextureAnimation(object);
            }
            GF3dRender_DrawModel(&object->renderObj, &object->position, object->rotation != NULL ? object->rotation : &identity, &object->scale);
            BillboardObject_ReleaseTexture(object);
        }
        object = object->next;
    }
}

void BillboardObject_LoadResource(BillboardObject *object, BillboardResource *resource) {
    Billboard *billboard = object->owner;
    BillboardObject_RemoveTextureAnimation(billboard, object);
    BillboardObject_LoadModel(object, resource);
    BillboardObject_AddTextureAnimation(billboard, object, resource);
    if (object->type == 1) {
        BillboardObject_InsertBefore(&billboard->root, object);
    }
    object->type = 2;
    object->resourceId = resource->resourceId;
    object->sequence = 0;
    object->frame = 0;
}

void BillboardObject_LoadResourceWithKeys(BillboardObject *object, BillboardResource *resource) {
    Billboard *billboard = object->owner;
    BillboardObject_RemoveTextureAnimation(billboard, object);
    if (object->type == 2) {
        Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
    object->texKey = resource->texKey;
    object->tex4x4Key = resource->tex4x4Key;
    object->paletteKey = resource->paletteKey;
    BillboardObject_LoadModelWithKeys(object, resource);
    BillboardObject_SetResourceData(object, resource);
    if (object->type == 1) {
        BillboardObject_InsertBefore(&billboard->root, object);
    }
    object->type = 3;
    object->resourceId = resource->resourceId;
    object->sequence = 0;
    object->frame = 0;
}

void Billboard_AllocTextureVram(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    u32 texSize = NNS_G3dTexGetRequiredSize(texture);
    u32 tex4x4Size = NNS_G3dTex4x4GetRequiredSize(texture);
    u32 paletteSize = NNS_G3dPlttGetRequiredSize(texture);
    if (texSize != 0) {
        *texKey = NNS_GfdDefaultFuncAllocTexVram(texSize, FALSE, 0);
        GF_ASSERT(*texKey != 0);
    } else {
        *texKey = 0;
    }
    if (tex4x4Size != 0) {
        *tex4x4Key = NNS_GfdDefaultFuncAllocTexVram(tex4x4Size, TRUE, 0);
        GF_ASSERT(*tex4x4Key != 0);
    } else {
        *tex4x4Key = 0;
    }
    if (paletteSize != 0) {
        *paletteKey = NNS_GfdDefaultFuncAllocPlttVram(paletteSize, *(u16 *)((u8 *)texture + 0x20) & 0x8000, 0);
        GF_ASSERT(*paletteKey != 0);
    } else {
        *paletteKey = 0;
    }
}

void Billboard_ReleaseTextureKeys(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    NNS_G3dTexReleaseTexKey(texture, texKey, tex4x4Key);
    *paletteKey = NNS_G3dPlttReleasePlttKey(texture);
}

void Billboard_BindTexture(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    NNS_G3dTexSetTexKey(texture, *texKey, *tex4x4Key);
    NNS_G3dPlttSetPlttKey(texture, *paletteKey);
    NNS_G3dBindMdlSet(modelSet, texture);
}

void Billboard_FreeTextureVram(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    if (*texKey != 0) {
        NNS_GfdDefaultFuncFreeTexVram(*texKey);
    }
    if (*tex4x4Key != 0) {
        NNS_GfdDefaultFuncFreeTexVram(*tex4x4Key);
    }
    if (*paletteKey != 0) {
        NNS_GfdDefaultFuncFreePlttVram(*paletteKey);
    }
}

BOOL Billboard_TexturesNeedSameVram(NNSG3dResTex *a, NNSG3dResTex *b) {
    u32 aTex, a4x4, aPltt;
    u32 bTex, b4x4, bPltt;
    if (a == NULL || b == NULL) {
        return FALSE;
    }
    aTex = NNS_G3dTexGetRequiredSize(a);
    a4x4 = NNS_G3dTex4x4GetRequiredSize(a);
    aPltt = NNS_G3dPlttGetRequiredSize(a);
    bTex = NNS_G3dTexGetRequiredSize(b);
    b4x4 = NNS_G3dTex4x4GetRequiredSize(b);
    bPltt = NNS_G3dPlttGetRequiredSize(b);
    if (aTex != bTex || a4x4 != b4x4 || aPltt != bPltt) {
        return FALSE;
    }
    return TRUE;
}

void BillboardObject_RemoveTextureAnimation(Billboard *billboard, BillboardObject *object) {
    if (object->textureAnimation != NULL) {
        TextureAnimation_Remove(object->textureAnimation);
        object->textureAnimation = NULL;
    }
}

void BillboardObject_LoadModel(BillboardObject *object, BillboardResource *resource) {
    NNSG3dResTex *oldTexture = object->texture;
    BOOL same;
    object->modelSet = BillboardResource_GetModelSet(resource, &object->model, &object->texture);
    NNS_G3dRenderObjInit(&object->renderObj, object->model);
    if (object->type != 3) {
        same = Billboard_TexturesNeedSameVram(oldTexture, object->texture);
    } else {
        same = FALSE;
    }
    if (!same) {
        if (object->type == 2) {
            Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
        }
        Billboard_AllocTextureVram(object->texture, &object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
}

void BillboardObject_AddTextureAnimation(Billboard *billboard, BillboardObject *object, BillboardResource *resource) {
    register BillboardResource *res = resource;
    register BillboardObject *obj = object;
    register Billboard *parent = billboard;
    obj->resourceHeader = BillboardResource_GetAnimFile(res);
    obj->resourceData = res->data;
    obj->textureAnimation = TextureAnimationPool_Add(parent->textureAnimationPool, &obj->resourceData, obj->resourceHeader, obj->texKey, obj->paletteKey, obj->frame);
}

void BillboardObject_LoadModelWithKeys(BillboardObject *object, BillboardResource *resource) {
    object->modelSet = BillboardResource_GetModelSet(resource, &object->model, &object->texture);
    NNS_G3dRenderObjInit(&object->renderObj, object->model);
    object->resourceHeader = BillboardResource_GetAnimFile(resource);
}

void BillboardObject_SetResourceData(BillboardObject *object, BillboardResource *resource) {
    object->resourceData = resource->data;
    object->textureAnimation = NULL;
}

BillboardObject *BillboardObject_Create(BillboardObjectCreateParams *params) {
    Billboard *billboard = params->billboard;
    BillboardObject *object;
    if (billboard == NULL) {
        return NULL;
    }
    object = Billboard_AllocObject(billboard);
    if (object == NULL) {
        return NULL;
    }
    object->owner = billboard;
    object->position = params->position;
    object->scale = params->scale;
    object->sequence = 0;
    object->visible = TRUE;
    object->type = 1;
    BillboardObject_ApplyResource(object, params->resourceIndex);
    return object;
}

BOOL BillboardObject_Delete(BillboardObject *object) {
    Billboard *billboard;
    GF_ASSERT(object != NULL);
    GF_ASSERT(object->type != 1);
    billboard = object->owner;
    if (object->type == 0) {
        return FALSE;
    }
    BillboardObject_Unlink(object);
    if (object->type == 2) {
        Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
    BillboardObject_RemoveTextureAnimation(billboard, object);
    Billboard_FreeObject(billboard, object);
    billboard->state = 1;
    return TRUE;
}

static inline u8 *Billboard_GetIndexedData(u8 *base, u32 index) {
    u32 *entry;
    if (base != NULL) {
        entry = NNS_G3dGetResDataByIdx((NNSG3dResDict *)(base + 4), index);
        if (entry != NULL) {
            return base + *entry;
        }
    }
    return NULL;
}

void BillboardResource_InitWithKeys(BillboardResource *resource, void *file0, u32 resourceId, u32 file1, BillboardResourceData *data, u32 texKey, u32 tex4x4Key, u32 paletteKey) {
    *(void **)&resource->padding_00[0] = file0;
    *(u32 *)&resource->padding_00[4] = resourceId;
    resource->resourceId = file1;
    resource->data = *data;
    resource->texKey = texKey;
    resource->tex4x4Key = tex4x4Key;
    resource->paletteKey = paletteKey;
}

void BillboardResource_Init(BillboardResource *resource, void *file0, u32 resourceId, u32 file1, BillboardResourceData *data) {
    *(void **)&resource->padding_00[0] = file0;
    *(u32 *)&resource->padding_00[4] = resourceId;
    resource->resourceId = file1;
    resource->data = *data;
    resource->texKey = 0;
    resource->tex4x4Key = 0;
    resource->paletteKey = 0;
}

void BillboardObject_SetPosition(BillboardObject *object, VecFx32 *position) {
    GF_ASSERT(object != NULL);
    object->position = *position;
}

VecFx32 *BillboardObject_GetPosition(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return &object->position;
}

void BillboardObject_SetScale(BillboardObject *object, VecFx32 *scale) {
    GF_ASSERT(object != NULL);
    object->scale = *scale;
}

VecFx32 *BillboardObject_GetScale(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return &object->scale;
}

void BillboardObject_SetVisible(Sprite *sprite, int draw) {
    BillboardObject *object = (BillboardObject *)sprite;
    GF_ASSERT(object != NULL);
    object->visible = draw;
}

u32 BillboardObject_IsVisible(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->visible;
}

void BillboardObject_SetResource(BillboardObject *object, u32 resourceIndex) {
    GF_ASSERT(object != NULL);
    BillboardObject_ApplyResource(object, resourceIndex);
}

void BillboardObject_SetSequence(BillboardObject *object, u32 frame) {
    GF_ASSERT(object != NULL);
    object->sequence = frame;
}

u32 BillboardObject_GetSequence(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->sequence;
}

void BillboardObject_AdvanceFrame(BillboardObject *object, fx32 delta) {
    GF_ASSERT(object != NULL);
    BillboardObject_StepFrame(object, delta);
}

void BillboardObject_SetFrame(BillboardObject *object, fx32 frame) {
    GF_ASSERT(object != NULL);
    object->frame = frame;
}

fx32 BillboardObject_GetFrame(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->frame;
}

void BillboardObject_SetSequenceFrame(BillboardObject *object, fx32 delta) {
    GF_ASSERT(object != NULL);
    object->frame = BillboardObject_GetSequenceStart(object, object->sequence);
    object->frame += delta;
}

fx32 BillboardObject_GetSequenceFrame(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->frame - BillboardObject_GetSequenceStart(object, object->sequence);
}

NNSG3dResMdl *BillboardObject_GetModel(void *arg) {
    BillboardObject *object = arg;
    GF_ASSERT(object != NULL);
    return object->model;
}

NNSG3dTexKey BillboardObject_GetTexKey(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->texKey;
}

NNSG3dPlttKey BillboardObject_GetPlttKey(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->paletteKey;
}

void BillboardObject_UpdateTextureAnimation(BillboardObject *object) {
    if (object->type == 2) {
        TextureAnimation_SetFrame(object->textureAnimation, object->frame);
    }
}

NNSG3dResMdl *BillboardObject_GetModelNoCast(BillboardObject *object) {
    return object->model;
}

void BillboardObject_SetCallback(BillboardObject *object, BillboardCallback callback, void *callbackArg) {
    object->callbackArg = callbackArg;
    object->callback = callback;
}

void BillboardObject_UpdateTexPlttSequence(BillboardObject *object) {
    volatile u8 data[4];
    u32 value = BillboardGfxSequence_GetTexPlttIndex(&object->resourceData, (object->frame << 4) >> 16);
    data[0] = value;
    data[1] = value >> 8;
    data[2] = data[0];
    data[3] = data[1];
    Billboard_SetModelTexture(object->model, object->resourceHeader, data[2]);
    Billboard_SetModelPltt(object->model, object->resourceHeader, data[3]);
}

void Billboard_SetModelTexture(NNSG3dResMdl *model, void *resourceHeader, u8 index) {
    register u8 *materialBlock;
    NNSG3dResDict *materialDict;
    u32 *entry;
    u16 value;
    int i;
    void *materialData;

    if (model != NULL && model->ofsMat != 0) {
        materialBlock = (u8 *)model + model->ofsMat;
    } else {
        materialBlock = NULL;
    }
    materialDict = (NNSG3dResDict *)(materialBlock + *(u16 *)materialBlock);
    if (resourceHeader != NULL) {
        entry = NNS_G3dGetResDataByIdx((NNSG3dResDict *)((u8 *)resourceHeader + 0x3C), index);
    } else {
        entry = NULL;
    }
    value = *entry;
    for (i = 0; i < materialDict->numEntry; i++) {
        materialData = NNS_G3dGetResDataByIdx(materialDict, i);
        if (*((u8 *)materialData + 3) & 1) {
            Billboard_OffsetMaterialTexImage(materialBlock, materialData, value);
        }
    }
}

void Billboard_OffsetMaterialTexImage(void *materialBlock, void *materialData, u32 value) {
    u8 *material;
    u8 *base = materialBlock;
    int i;
    u8 *indices = base + *(u16 *)materialData;
    for (i = 0; i < *((u8 *)materialData + 2); i++) {
        material = Billboard_GetIndexedData(base, indices[i]);
        GF_ASSERT((u16) * (u32 *)(material + 0x14) + value <= 0xFFFF);
        *(u32 *)(material + 0x14) += value;
    }
}

void Billboard_SetModelPltt(NNSG3dResMdl *model, void *resourceHeader, u8 index) {
    u32 value;
    u8 *materialBlock;
    NNSG3dResDict *shapeDict;
    u8 *subDict;
    u16 *entry;
    u32 i;
    void *shapeData;

    if (model != NULL && model->ofsMat != 0) {
        materialBlock = (u8 *)model + model->ofsMat;
    } else {
        materialBlock = NULL;
    }
    shapeDict = (NNSG3dResDict *)(materialBlock + *((u16 *)materialBlock + 1));
    if (resourceHeader != NULL && *(u16 *)((u8 *)resourceHeader + 0x34) != 0) {
        subDict = (u8 *)resourceHeader + *(u16 *)((u8 *)resourceHeader + 0x34);
        entry = NNS_G3dGetResDataByIdx((NNSG3dResDict *)subDict, index);
    } else {
        entry = NULL;
    }
    value = entry[0];
    if (!(entry[1] & 1)) {
        value >>= 1;
    }
    for (i = 0; i < shapeDict->numEntry; i++) {
        shapeData = NNS_G3dGetResDataByIdx(shapeDict, i);
        if (*((u8 *)shapeData + 3) & 1) {
            Billboard_OffsetMaterialPltt(materialBlock, shapeData, value);
        }
    }
}

void Billboard_OffsetMaterialPltt(void *materialBlock, void *shapeData, u32 value) {
    u8 *indices = (u8 *)materialBlock + *(u16 *)shapeData;
    u32 i;
    u8 *shape;
    for (i = 0; i < *((u8 *)shapeData + 2); i++) {
        shape = Billboard_GetIndexedData(materialBlock, indices[i]);
        GF_ASSERT((*(u16 *)(shape + 0x1C) & 0x1FFF) + value <= 0x1FFF);
        *(u16 *)(shape + 0x1C) += value;
    }
}

void Billboard_InitObjectPool(Billboard *billboard) {
    int i;
    for (i = 0; i < billboard->objectCount; i++) {
        BillboardObject_Clear(&billboard->objects[i]);
        billboard->freeObjects[i] = (u32)&billboard->objects[i];
    }
    billboard->usedCount = 0;
}

BillboardObject *Billboard_AllocObject(Billboard *billboard) {
    BillboardObject *object;
    if ((int)billboard->usedCount >= billboard->objectCount) {
        return NULL;
    }
    object = (BillboardObject *)billboard->freeObjects[billboard->usedCount];
    billboard->usedCount++;
    return object;
}

BOOL Billboard_FreeObject(Billboard *billboard, BillboardObject *object) {
    if ((int)billboard->usedCount <= 0) {
        return FALSE;
    }
    BillboardObject_Clear(object);
    billboard->usedCount--;
    billboard->freeObjects[billboard->usedCount] = (u32)object;
    return TRUE;
}

void BillboardObject_InsertBefore(BillboardObject *root, BillboardObject *object) {
    object->prev = root->prev;
    root->prev->next = object;
    object->next = root;
    root->prev = object;
}

void BillboardObject_Unlink(BillboardObject *object) {
    object->prev->next = object->next;
    object->next->prev = object->prev;
}

NNSG3dResMdlSet *BillboardResource_GetModelSet(BillboardResource *resource, NNSG3dResMdl **model, NNSG3dResTex **texture) {
    void *file = BillboardResource_GetFile(resource, 0);
    NNSG3dResMdlSet *modelSet = NNS_G3dGetMdlSet(file);
    *model = NNS_G3dGetMdlByIdx(modelSet, 0);
    if (texture != NULL) {
        *texture = NNS_G3dGetTex(file);
    }
    return modelSet;
}

void *BillboardResource_GetAnimFile(BillboardResource *resource) {
    return BillboardResource_GetFile(resource, 1);
}

void BillboardObject_ApplyResource(BillboardObject *object, u32 resourceIndex) {
    BillboardResource *resource = (BillboardResource *)resourceIndex;
    if (resource->texKey == 0) {
        BillboardObject_LoadResource(object, resource);
    } else {
        BillboardObject_LoadResourceWithKeys(object, resource);
    }
}

fx32 BillboardObject_GetSequenceStart(BillboardObject *object, u32 index) {
    u32 *entry = BillboardSequence_Get((void *)object->resourceId, index);
    return *entry << FX32_SHIFT;
}

void BillboardObject_StepFrame(BillboardObject *object, fx32 delta) {
    void *entry = BillboardSequence_Get((void *)object->resourceId, object->sequence);
    BillboardSequence_Step(entry, &object->frame, delta);
}

BOOL BillboardSequence_Step(void *entry, fx32 *value, fx32 delta) {
    BOOL wrapped = FALSE;
    fx32 start = *(s32 *)entry << FX32_SHIFT;
    fx32 current = *value;
    fx32 end;
    if (start > current || (end = *((s32 *)entry + 1) << FX32_SHIFT) < current) {
        *value = start;
    } else if (end < current + delta) {
        if (*((s32 *)entry + 2) == 0) {
            *value = start;
        } else {
            wrapped = TRUE;
            *value = end;
        }
    } else {
        *value = current + delta;
    }
    return wrapped;
}

void BillboardObject_BindTexture(BillboardObject *object) {
    Billboard_BindTexture(object->texture, object->modelSet, &object->texKey, &object->tex4x4Key, &object->paletteKey);
}

void BillboardObject_ReleaseTexture(BillboardObject *object) {
    NNSG3dTexKey tex4x4Key;
    NNSG3dTexKey texKey;
    NNSG3dPlttKey paletteKey;
    NNS_G3dReleaseMdlSet(object->modelSet);
    Billboard_ReleaseTextureKeys(object->texture, &texKey, &tex4x4Key, &paletteKey);
}

void *BillboardResource_GetFile(BillboardResource *resource, u32 which) {
    switch (which) {
    case 0:
        return *(void **)&resource->padding_00[0];
    case 1:
        return *(void **)&resource->padding_00[4];
    default:
        return NULL;
    }
}

void *BillboardSequence_Get(void *base, u32 index) {
    return (u8 *)base + index * 12;
}
