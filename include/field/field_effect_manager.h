#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H

#include "filesystem.h"
#include "map_object.h"
#include "script.h"
#include "task.h"
#include "field_effects.h"

// Owns the field effect renderers (one per renderer id, see gFieldEffectRenderers), the billboard
// records and resource banks they load from, and the pool of running field effects. The layout is
// private to field_effect_manager.c (field_effect_manager_internal.h).
typedef struct FieldEffectManager FieldEffectManager;

// A running effect of the field effect pool (field_effects.c; these four types match its private ones).
// The pool hands each effect a zeroed state block of FieldEffectBasicTemplate.stateSize bytes; the
// userData passed to FieldEffectManager_CreateEffect is read back with FieldEffect_GetUserData and the
// position with FieldEffect_GetPosition.
typedef struct FieldEffect FieldEffect;

// A billboard graphics record (FieldEffectManager_AddRecord); layout in field_effect_manager_internal.h.
struct FieldEffectRecord;

typedef BOOL (*FieldEffectInitFunc)(FieldEffect *effect, void *state);
typedef void (*FieldEffectFunc)(FieldEffect *effect, void *state);

typedef struct FieldEffectBasicTemplate {
    u32 stateSize;
    FieldEffectInitFunc init;
    FieldEffectFunc destroy;
    FieldEffectFunc update;
    FieldEffectFunc draw;
} FieldEffectBasicTemplate;

typedef struct {
    u8 unk0[0xc];
    NNSG3dResMdl *model;
    u8 unk10[4];
} RenderModelStorage;

FieldEffectManager *FieldEffectManager_New(FieldSystem *fieldSystem, u32 rendererCount, enum HeapID heapID);
void FieldEffectManager_InitAnimManagerList(FieldEffectManager *manager, u32 animManagerCount);
void FieldEffectManager_InitGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 resourceCount, u32 sequenceCount, u32 resCount, int resourceHeapSize, int sequenceHeapSize, int a8);
void FieldEffectManager_InitRenderers(FieldEffectManager *manager, const u32 *ids);
void FieldEffectManager_Render(FieldEffectManager *manager);
void FieldEffectManager_Free(FieldEffectManager *manager);
void *FieldEffectManager_HeapAlloc(FieldEffectManager *manager, u32 size, BOOL atEnd);
void *FieldEffectManager_HeapAllocAndFill(FieldEffectManager *manager, u32 size, BOOL atEnd, int fill);
void FieldEffectManager_HeapFree(void *ptr);
void *FieldEffectManager_GetRendererData(FieldEffectManager *manager, u32 id);
FieldSystem *FieldEffectManager_GetFieldSystem(FieldEffectManager *manager);
FieldEffectManager *FieldEffectManager_GetFromMapObject(LocalMapObject *mapObject);
enum HeapID FieldEffectManager_GetHeapID(FieldEffectManager *manager);
u32 FieldEffectManager_GetNarcMemberSize(FieldEffectManager *manager, u32 fileId);
void FieldEffectManager_ReadNarcMember(FieldEffectManager *manager, u32 fileId, void *dest);
void *FieldEffectManager_LoadNarcMember(FieldEffectManager *manager, u32 fileId, BOOL atEnd);
FieldEffect *FieldEffectManager_CreateEffect(FieldEffectManager *manager, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
// Callers keep the FieldEffect * from FieldEffectManager_CreateEffect as an int handle.
void FieldEffectManager_DestroyEffect(int effect);
void *FieldEffectManager_CreateBillboard(FieldEffectManager *manager, struct FieldEffectRecord *record, const VecFx32 *position);
void *FieldEffectManager_CreateBillboardFromRecord(FieldEffectManager *manager, u32 id, const VecFx32 *position);
struct FieldEffectRecord *FieldEffectManager_AddRecord(FieldEffectManager *manager, u32 id, int resourceId, int sequenceId, int resId, int noTexKeys, const void *params);
void FieldEffectManager_ReleaseRecord(FieldEffectManager *manager, u32 id);
void FieldEffectManager_LoadBillboardResource(FieldEffectManager *manager, int id, int fileId);
void *FieldEffectManager_GetBillboardResource(FieldEffectManager *manager, int id);
void FieldEffectManager_FreeBillboardResource(FieldEffectManager *manager, int id);
void FieldEffectManager_LoadBillboardSequence(FieldEffectManager *manager, int id, int fileId);
void FieldEffectManager_FreeBillboardSequence(FieldEffectManager *manager, int id);
void FieldEffectManager_LoadTexResource(FieldEffectManager *manager, int id, u32 fileId, BOOL copyWithoutTex);
void FieldEffectManager_FreeTexResource(FieldEffectManager *manager, int id);
void FieldEffectManager_LoadRenderModel(FieldEffectManager *manager, void *model, int index, int fileId, BOOL atEnd);
void FieldEffectManager_LoadRenderAnimation(FieldEffectManager *manager, void *animation, int index, int fileId, BOOL atEnd);
void FieldEffectManager_InitRenderAnimation(FieldEffectManager *manager, void *animation, void *model, void *anim, int unused);
void FieldEffectManager_LoadModel(FieldEffectManager *manager, u32 fileId, BOOL atEnd, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes);
NNSG3dResTex *FieldEffectManager_GetTexture(FieldEffectManager *manager, u32 id);

#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_H
