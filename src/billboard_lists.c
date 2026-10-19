#include "billboard_lists.h"

#include <nnsys/g3d_internal.h>

#include "global.h"

#include "billboard_internal.h"
#include "gf_3d_render.h"
#include "heap.h"
#include "texture_animation.h"

enum BillboardObjectType {
    BILLBOARD_OBJECT_FREE,
    BILLBOARD_OBJECT_NEW,
    BILLBOARD_OBJECT_OWN_TEXTURE,
    BILLBOARD_OBJECT_SHARED_TEXTURE,
};

enum BillboardFreeState {
    BILLBOARD_FREE_NONE,
    BILLBOARD_FREE_REQUESTED,
    BILLBOARD_FREE_PENDING,
};

static void Billboard_Clear(Billboard *billboard);
static void BillboardObject_Clear(BillboardObject *object);
static BOOL Billboard_DeleteAllObjects(Billboard *billboard);
static Billboard *BillboardLists_GetFree(void);
static void Billboard_Draw(Billboard *billboard);
static void BillboardObject_LoadResource(BillboardObject *object, BillboardResource *resource);
static void BillboardObject_LoadResourceWithKeys(BillboardObject *object, BillboardResource *resource);
static void Billboard_AllocTextureVram(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
static void Billboard_ReleaseTextureKeys(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
static void Billboard_BindTexture(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
static void Billboard_FreeTextureVram(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey);
static BOOL Billboard_TexturesNeedSameVram(NNSG3dResTex *a, NNSG3dResTex *b);
static void BillboardObject_RemoveTextureAnimation(Billboard *billboard, BillboardObject *object);
static void BillboardObject_LoadModel(BillboardObject *object, BillboardResource *resource);
static void BillboardObject_AddTextureAnimation(Billboard *billboard, BillboardObject *object, BillboardResource *resource);
static void BillboardObject_LoadModelWithKeys(BillboardObject *object, BillboardResource *resource);
static void BillboardObject_SetResourceData(BillboardObject *object, BillboardResource *resource);
static inline G3dResMatData *Billboard_GetMatData(G3dResMat *mat, u32 index);
static void BillboardObject_UpdateTexPlttSequence(BillboardObject *object);
static void Billboard_SetModelTexture(NNSG3dResMdl *model, NNSG3dResTex *texture, u8 index);
static void Billboard_OffsetMaterialTexImage(G3dResMat *mat, G3dResBindEntry *bind, u32 value);
static void Billboard_SetModelPltt(NNSG3dResMdl *model, NNSG3dResTex *texture, u8 index);
static void Billboard_OffsetMaterialPltt(G3dResMat *mat, G3dResBindEntry *bind, u32 value);
static void Billboard_InitObjectPool(Billboard *billboard);
static BillboardObject *Billboard_AllocObject(Billboard *billboard);
static BOOL Billboard_FreeObject(Billboard *billboard, BillboardObject *object);
static void BillboardObject_InsertBefore(BillboardObject *root, BillboardObject *object);
static void BillboardObject_Unlink(BillboardObject *object);
static NNSG3dResMdlSet *BillboardResource_GetModelSet(BillboardResource *resource, NNSG3dResMdl **model, NNSG3dResTex **texture);
static NNSG3dResTex *BillboardResource_GetTexture(BillboardResource *resource);
static void BillboardObject_ApplyResource(BillboardObject *object, BillboardResource *resource);
static fx32 BillboardObject_GetSequenceStart(BillboardObject *object, u32 index);
static BOOL BillboardObject_StepFrame(BillboardObject *object, fx32 delta);
static void BillboardObject_BindTexture(BillboardObject *object);
static void BillboardObject_ReleaseTexture(BillboardObject *object);
static void *BillboardResource_GetFile(BillboardResource *resource, u32 which);
static const BillboardSequenceRange *BillboardSequence_Get(const BillboardSequenceRange *ranges, u32 index);
static BOOL BillboardSequence_Step(const BillboardSequenceRange *range, fx32 *frame, fx32 delta);

Billboard *Billboard_Create(BillboardCreateParams *params);
BOOL Billboard_Delete(Billboard *billboard);
BOOL Billboard_HasPendingFree(Billboard *billboard);
BillboardObject *BillboardObject_Create(BillboardObjectCreateParams *params);
BOOL BillboardObject_Delete(BillboardObject *object);
void BillboardObject_SetPosition(BillboardObject *, VecFx32 *);
VecFx32 *BillboardObject_GetPosition(BillboardObject *);
void BillboardObject_SetScale(BillboardObject *, VecFx32 *);
VecFx32 *BillboardObject_GetScale(BillboardObject *);
u32 BillboardObject_IsVisible(BillboardObject *);
void BillboardObject_SetSequence(BillboardObject *, u32);
u32 BillboardObject_GetSequence(BillboardObject *);
BOOL BillboardObject_AdvanceFrame(BillboardObject *, fx32);
void BillboardObject_SetFrame(BillboardObject *, fx32);
fx32 BillboardObject_GetFrame(BillboardObject *);
void BillboardObject_SetSequenceFrame(BillboardObject *, fx32);
fx32 BillboardObject_GetSequenceFrame(BillboardObject *);
NNSG3dTexKey BillboardObject_GetTexKey(BillboardObject *);
NNSG3dPlttKey BillboardObject_GetPlttKey(BillboardObject *);
void BillboardObject_UpdateTextureAnimation(BillboardObject *object);
NNSG3dResMdl *BillboardObject_GetModelNoCast(BillboardObject *);
void BillboardObject_SetCallback(BillboardObject *, BillboardCallback, void *);

static BillboardListManager sBillboardLists;

static void Billboard_Clear(Billboard *billboard) {
    billboard->initialized = 0;
    billboard->draw = 0;
    billboard->objects = NULL;
    billboard->objectCount = 0;
    billboard->freeObjects = NULL;
    billboard->usedCount = 0;
    billboard->allocator = NULL;
    billboard->textureAnimationPool = NULL;
    billboard->state = BILLBOARD_FREE_NONE;
}

static void BillboardObject_Clear(BillboardObject *object) {
    object->owner = 0;
    object->ranges = NULL;
    object->modelSet = 0;
    object->model = 0;
    object->texture = 0;
    object->resourceTexture = NULL;
    memset(&object->gfxSequence, 0, sizeof(object->gfxSequence));
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
    object->type = BILLBOARD_OBJECT_FREE;
    object->next = NULL;
    object->prev = 0;
    object->visible = 0;
    object->callback = 0;
}

void BillboardLists_Create(int count, enum HeapID heapID) {
    int i;

    GF_ASSERT(sBillboardLists.list == NULL);
    sBillboardLists.list = Heap_Alloc(heapID, count * sizeof(Billboard));
    sBillboardLists.count = count;
    for (i = 0; i < count; i++) {
        Billboard_Clear(&sBillboardLists.list[i]);
    }
}

void BillboardLists_Delete(void) {
    int i;

    for (i = 0; i < sBillboardLists.count; i++) {
        Billboard_Delete(&sBillboardLists.list[i]);
    }
    Heap_Free(sBillboardLists.list);
    sBillboardLists.list = NULL;
    sBillboardLists.count = 0;
}

void BillboardLists_Draw(void) {
    int i;

    for (i = 0; i < sBillboardLists.count; i++) {
        if (sBillboardLists.list[i].draw == 1) {
            Billboard_Draw(&sBillboardLists.list[i]);
        }
        if (sBillboardLists.list[i].state == BILLBOARD_FREE_REQUESTED) {
            sBillboardLists.list[i].state = BILLBOARD_FREE_PENDING;
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

static BOOL Billboard_DeleteAllObjects(Billboard *billboard) {
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
    if (billboard->state == BILLBOARD_FREE_NONE) {
        return FALSE;
    }
    return TRUE;
}

void Billboard_ClearPendingFree(void *data) {
    Billboard *billboard = data;
    if (billboard->state == BILLBOARD_FREE_PENDING) {
        billboard->state = BILLBOARD_FREE_NONE;
    }
}

static Billboard *BillboardLists_GetFree(void) {
    int i;
    for (i = 0; i < sBillboardLists.count; i++) {
        if (!sBillboardLists.list[i].initialized) {
            return &sBillboardLists.list[i];
        }
    }
    return NULL;
}

static void Billboard_Draw(Billboard *billboard) {
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
            if (object->type == BILLBOARD_OBJECT_SHARED_TEXTURE) {
                BillboardObject_UpdateTexPlttSequence(object);
            } else if (object->type == BILLBOARD_OBJECT_OWN_TEXTURE) {
                BillboardObject_UpdateTextureAnimation(object);
            }
            GF3dRender_DrawModel(&object->renderObj, &object->position, object->rotation != NULL ? object->rotation : &identity, &object->scale);
            BillboardObject_ReleaseTexture(object);
        }
        object = object->next;
    }
}

static void BillboardObject_LoadResource(BillboardObject *object, BillboardResource *resource) {
    Billboard *billboard = object->owner;
    BillboardObject_RemoveTextureAnimation(billboard, object);
    BillboardObject_LoadModel(object, resource);
    BillboardObject_AddTextureAnimation(billboard, object, resource);
    if (object->type == BILLBOARD_OBJECT_NEW) {
        BillboardObject_InsertBefore(&billboard->root, object);
    }
    object->type = BILLBOARD_OBJECT_OWN_TEXTURE;
    object->ranges = resource->ranges;
    object->sequence = 0;
    object->frame = 0;
}

static void BillboardObject_LoadResourceWithKeys(BillboardObject *object, BillboardResource *resource) {
    Billboard *billboard = object->owner;
    BillboardObject_RemoveTextureAnimation(billboard, object);
    if (object->type == BILLBOARD_OBJECT_OWN_TEXTURE) {
        Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
    object->texKey = resource->texKey;
    object->tex4x4Key = resource->tex4x4Key;
    object->paletteKey = resource->plttKey;
    BillboardObject_LoadModelWithKeys(object, resource);
    BillboardObject_SetResourceData(object, resource);
    if (object->type == BILLBOARD_OBJECT_NEW) {
        BillboardObject_InsertBefore(&billboard->root, object);
    }
    object->type = BILLBOARD_OBJECT_SHARED_TEXTURE;
    object->ranges = resource->ranges;
    object->sequence = 0;
    object->frame = 0;
}

static void Billboard_AllocTextureVram(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
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
        *paletteKey = NNS_GfdDefaultFuncAllocPlttVram(paletteSize, texture->tex4x4Info.flag & 0x8000, 0);
        GF_ASSERT(*paletteKey != 0);
    } else {
        *paletteKey = 0;
    }
}

static void Billboard_ReleaseTextureKeys(NNSG3dResTex *texture, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    NNS_G3dTexReleaseTexKey(texture, texKey, tex4x4Key);
    *paletteKey = NNS_G3dPlttReleasePlttKey(texture);
}

static void Billboard_BindTexture(NNSG3dResTex *texture, NNSG3dResMdlSet *modelSet, NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
    NNS_G3dTexSetTexKey(texture, *texKey, *tex4x4Key);
    NNS_G3dPlttSetPlttKey(texture, *paletteKey);
    NNS_G3dBindMdlSet(modelSet, texture);
}

static void Billboard_FreeTextureVram(NNSG3dTexKey *texKey, NNSG3dTexKey *tex4x4Key, NNSG3dPlttKey *paletteKey) {
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

static BOOL Billboard_TexturesNeedSameVram(NNSG3dResTex *a, NNSG3dResTex *b) {
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

static void BillboardObject_RemoveTextureAnimation(Billboard *billboard, BillboardObject *object) {
    if (object->textureAnimation != NULL) {
        TextureAnimation_Remove(object->textureAnimation);
        object->textureAnimation = NULL;
    }
}

static void BillboardObject_LoadModel(BillboardObject *object, BillboardResource *resource) {
    NNSG3dResTex *oldTexture = object->texture;
    BOOL same;
    object->modelSet = BillboardResource_GetModelSet(resource, &object->model, &object->texture);
    NNS_G3dRenderObjInit(&object->renderObj, object->model);
    if (object->type != BILLBOARD_OBJECT_SHARED_TEXTURE) {
        same = Billboard_TexturesNeedSameVram(oldTexture, object->texture);
    } else {
        same = FALSE;
    }
    if (!same) {
        if (object->type == BILLBOARD_OBJECT_OWN_TEXTURE) {
            Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
        }
        Billboard_AllocTextureVram(object->texture, &object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
}

static void BillboardObject_AddTextureAnimation(Billboard *billboard, BillboardObject *object, BillboardResource *resource) {
    object->resourceTexture = BillboardResource_GetTexture(resource);
    object->gfxSequence = resource->gfxSequence;
    object->textureAnimation = TextureAnimationPool_Add(billboard->textureAnimationPool, &object->gfxSequence, object->resourceTexture, object->texKey, object->paletteKey, object->frame);
}

static void BillboardObject_LoadModelWithKeys(BillboardObject *object, BillboardResource *resource) {
    object->modelSet = BillboardResource_GetModelSet(resource, &object->model, &object->texture);
    NNS_G3dRenderObjInit(&object->renderObj, object->model);
    object->resourceTexture = BillboardResource_GetTexture(resource);
}

static void BillboardObject_SetResourceData(BillboardObject *object, BillboardResource *resource) {
    object->gfxSequence = resource->gfxSequence;
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
    object->type = BILLBOARD_OBJECT_NEW;
    BillboardObject_ApplyResource(object, params->resource);
    return object;
}

BOOL BillboardObject_Delete(BillboardObject *object) {
    Billboard *billboard;
    GF_ASSERT(object != NULL);
    GF_ASSERT(object->type != BILLBOARD_OBJECT_NEW);
    billboard = object->owner;
    if (object->type == BILLBOARD_OBJECT_FREE) {
        return FALSE;
    }
    BillboardObject_Unlink(object);
    if (object->type == BILLBOARD_OBJECT_OWN_TEXTURE) {
        Billboard_FreeTextureVram(&object->texKey, &object->tex4x4Key, &object->paletteKey);
    }
    BillboardObject_RemoveTextureAnimation(billboard, object);
    Billboard_FreeObject(billboard, object);
    billboard->state = BILLBOARD_FREE_REQUESTED;
    return TRUE;
}

static inline G3dResMatData *Billboard_GetMatData(G3dResMat *mat, u32 index) {
    u32 *entry;
    if (mat != NULL) {
        entry = NNS_G3dGetResDataByIdx(&mat->dict, index);
        if (entry != NULL) {
            return (G3dResMatData *)((u8 *)mat + *entry);
        }
    }
    return NULL;
}

void BillboardResource_InitWithKeys(BillboardResource *resource, void *modelFile, NNSG3dResTex *texture, const BillboardSequenceRange *ranges, BillboardGfxSequence *gfxSequence, NNSG3dTexKey texKey, NNSG3dTexKey tex4x4Key, NNSG3dPlttKey plttKey) {
    resource->modelFile = modelFile;
    resource->texture = texture;
    resource->ranges = ranges;
    resource->gfxSequence = *gfxSequence;
    resource->texKey = texKey;
    resource->tex4x4Key = tex4x4Key;
    resource->plttKey = plttKey;
}

void BillboardResource_Init(BillboardResource *resource, void *modelFile, NNSG3dResTex *texture, const BillboardSequenceRange *ranges, BillboardGfxSequence *gfxSequence) {
    resource->modelFile = modelFile;
    resource->texture = texture;
    resource->ranges = ranges;
    resource->gfxSequence = *gfxSequence;
    resource->texKey = 0;
    resource->tex4x4Key = 0;
    resource->plttKey = 0;
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

void BillboardObject_SetResource(BillboardObject *object, BillboardResource *resource) {
    GF_ASSERT(object != NULL);
    BillboardObject_ApplyResource(object, resource);
}

void BillboardObject_SetSequence(BillboardObject *object, u32 frame) {
    GF_ASSERT(object != NULL);
    object->sequence = frame;
}

u32 BillboardObject_GetSequence(BillboardObject *object) {
    GF_ASSERT(object != NULL);
    return object->sequence;
}

BOOL BillboardObject_AdvanceFrame(BillboardObject *object, fx32 delta) {
    GF_ASSERT(object != NULL);
    return BillboardObject_StepFrame(object, delta);
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
    if (object->type == BILLBOARD_OBJECT_OWN_TEXTURE) {
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

static void BillboardObject_UpdateTexPlttSequence(BillboardObject *object) {
    BillboardTexPlttIndex index = BillboardGfxSequence_GetTexPlttIndex(&object->gfxSequence, (object->frame << 4) >> 16);
    Billboard_SetModelTexture(object->model, object->resourceTexture, index.textureIdx);
    Billboard_SetModelPltt(object->model, object->resourceTexture, index.plttIdx);
}

static void Billboard_SetModelTexture(NNSG3dResMdl *model, NNSG3dResTex *texture, u8 index) {
    G3dResMat *mat;
    NNSG3dResDict *bindDict;
    G3dResTexData *texData;
    u16 texImageParam;
    int i;
    G3dResBindEntry *bind;

    if (model != NULL && model->ofsMat != 0) {
        mat = (G3dResMat *)((u8 *)model + model->ofsMat);
    } else {
        mat = NULL;
    }
    // BUG: without materials (or without the texture entry below) this reads through a NULL pointer
    bindDict = (NNSG3dResDict *)((u8 *)mat + mat->ofsDictTexToMatList);
    if (texture != NULL) {
        texData = NNS_G3dGetResDataByIdx(&texture->dict, index);
    } else {
        texData = NULL;
    }
    texImageParam = texData->texImageParam;
    for (i = 0; i < bindDict->numEntry; i++) {
        bind = NNS_G3dGetResDataByIdx(bindDict, i);
        if (bind->flag & G3D_RESBIND_BOUND) {
            Billboard_OffsetMaterialTexImage(mat, bind, texImageParam);
        }
    }
}

static void Billboard_OffsetMaterialTexImage(G3dResMat *mat, G3dResBindEntry *bind, u32 value) {
    G3dResMatData *matData;
    int i;
    u8 *matIndices = (u8 *)mat + bind->ofsMatIdxList;
    for (i = 0; i < bind->numMat; i++) {
        matData = Billboard_GetMatData(mat, matIndices[i]);
        GF_ASSERT((u16)matData->texImageParam + value <= 0xFFFF);
        matData->texImageParam += value;
    }
}

static void Billboard_SetModelPltt(NNSG3dResMdl *model, NNSG3dResTex *texture, u8 index) {
    u32 plttBase;
    G3dResMat *mat;
    NNSG3dResDict *bindDict;
    u8 *plttDict;
    G3dResPlttData *plttData;
    u32 i;
    G3dResBindEntry *bind;

    if (model != NULL && model->ofsMat != 0) {
        mat = (G3dResMat *)((u8 *)model + model->ofsMat);
    } else {
        mat = NULL;
    }
    // BUG: without materials (or without the texture entry below) this reads through a NULL pointer
    bindDict = (NNSG3dResDict *)((u8 *)mat + mat->ofsDictPlttToMatList);
    if (texture != NULL && texture->plttInfo.ofsDict != 0) {
        plttDict = (u8 *)texture + texture->plttInfo.ofsDict;
        plttData = NNS_G3dGetResDataByIdx((NNSG3dResDict *)plttDict, index);
    } else {
        plttData = NULL;
    }
    plttBase = plttData->offset;
    if (!(plttData->flag & G3D_RESPLTTDATA_4COLOR)) {
        plttBase >>= 1;
    }
    for (i = 0; i < bindDict->numEntry; i++) {
        bind = NNS_G3dGetResDataByIdx(bindDict, i);
        if (bind->flag & G3D_RESBIND_BOUND) {
            Billboard_OffsetMaterialPltt(mat, bind, plttBase);
        }
    }
}

static void Billboard_OffsetMaterialPltt(G3dResMat *mat, G3dResBindEntry *bind, u32 value) {
    u8 *matIndices = (u8 *)mat + bind->ofsMatIdxList;
    u32 i;
    G3dResMatData *matData;
    for (i = 0; i < bind->numMat; i++) {
        matData = Billboard_GetMatData(mat, matIndices[i]);
        GF_ASSERT((matData->texPlttBase & 0x1FFF) + value <= 0x1FFF);
        matData->texPlttBase += value;
    }
}

static void Billboard_InitObjectPool(Billboard *billboard) {
    int i;
    for (i = 0; i < billboard->objectCount; i++) {
        BillboardObject_Clear(&billboard->objects[i]);
        billboard->freeObjects[i] = (u32)&billboard->objects[i];
    }
    billboard->usedCount = 0;
}

static BillboardObject *Billboard_AllocObject(Billboard *billboard) {
    BillboardObject *object;
    if ((int)billboard->usedCount >= billboard->objectCount) {
        return NULL;
    }
    object = (BillboardObject *)billboard->freeObjects[billboard->usedCount];
    billboard->usedCount++;
    return object;
}

static BOOL Billboard_FreeObject(Billboard *billboard, BillboardObject *object) {
    if ((int)billboard->usedCount <= 0) {
        return FALSE;
    }
    BillboardObject_Clear(object);
    billboard->usedCount--;
    billboard->freeObjects[billboard->usedCount] = (u32)object;
    return TRUE;
}

static void BillboardObject_InsertBefore(BillboardObject *root, BillboardObject *object) {
    object->prev = root->prev;
    root->prev->next = object;
    object->next = root;
    root->prev = object;
}

static void BillboardObject_Unlink(BillboardObject *object) {
    object->prev->next = object->next;
    object->next->prev = object->prev;
}

static NNSG3dResMdlSet *BillboardResource_GetModelSet(BillboardResource *resource, NNSG3dResMdl **model, NNSG3dResTex **texture) {
    void *file = BillboardResource_GetFile(resource, 0);
    NNSG3dResMdlSet *modelSet = NNS_G3dGetMdlSet(file);
    *model = NNS_G3dGetMdlByIdx(modelSet, 0);
    if (texture != NULL) {
        *texture = NNS_G3dGetTex(file);
    }
    return modelSet;
}

static NNSG3dResTex *BillboardResource_GetTexture(BillboardResource *resource) {
    return BillboardResource_GetFile(resource, 1);
}

static void BillboardObject_ApplyResource(BillboardObject *object, BillboardResource *resource) {
    if (resource->texKey == 0) {
        BillboardObject_LoadResource(object, resource);
    } else {
        BillboardObject_LoadResourceWithKeys(object, resource);
    }
}

static fx32 BillboardObject_GetSequenceStart(BillboardObject *object, u32 index) {
    const BillboardSequenceRange *range = BillboardSequence_Get(object->ranges, index);
    return range->start << FX32_SHIFT;
}

static BOOL BillboardObject_StepFrame(BillboardObject *object, fx32 delta) {
    const BillboardSequenceRange *range = BillboardSequence_Get(object->ranges, object->sequence);
    return BillboardSequence_Step(range, &object->frame, delta);
}

static BOOL BillboardSequence_Step(const BillboardSequenceRange *range, fx32 *frame, fx32 delta) {
    BOOL finished = FALSE;
    fx32 start = range->start << FX32_SHIFT;
    fx32 current = *frame;
    fx32 end;
    if (start > current || (end = range->end << FX32_SHIFT) < current) {
        *frame = start;
    } else if (end < current + delta) {
        if (range->stopAtEnd == 0) {
            *frame = start;
        } else {
            finished = TRUE;
            *frame = end;
        }
    } else {
        *frame = current + delta;
    }
    return finished;
}

static void BillboardObject_BindTexture(BillboardObject *object) {
    Billboard_BindTexture(object->texture, object->modelSet, &object->texKey, &object->tex4x4Key, &object->paletteKey);
}

static void BillboardObject_ReleaseTexture(BillboardObject *object) {
    NNSG3dTexKey tex4x4Key;
    NNSG3dTexKey texKey;
    NNSG3dPlttKey paletteKey;
    NNS_G3dReleaseMdlSet(object->modelSet);
    Billboard_ReleaseTextureKeys(object->texture, &texKey, &tex4x4Key, &paletteKey);
}

static void *BillboardResource_GetFile(BillboardResource *resource, u32 which) {
    switch (which) {
    case 0:
        return resource->modelFile;
    case 1:
        return resource->texture;
    default:
        return NULL;
    }
}

static const BillboardSequenceRange *BillboardSequence_Get(const BillboardSequenceRange *ranges, u32 index) {
    return &ranges[index];
}
