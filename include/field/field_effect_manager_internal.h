#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H

#include "global.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "sys_task_api.h"

#include "field/field_effect_manager.h"

// Layouts of the field effect manager and its private helpers, for its own two translation units.

typedef struct FieldEffectRendererEntry {
    u32 id; // 0x17: free
    void *data;
} FieldEffectRendererEntry;

typedef struct FieldEffectRecordSlot {
    u32 id; // 0xF: free
    struct FieldEffectRecord *record;
} FieldEffectRecordSlot;

// Filled by BillboardGfxSequence_Init.
typedef struct FieldEffectGfxSequence {
    u32 unk_00[4];
} FieldEffectGfxSequence;

typedef struct FieldEffectRecord {
    void *unk_00;
    NNSG3dResTex *tex;
    int unk_08;
    FieldEffectGfxSequence sequence;
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey plttKey;
} FieldEffectRecord;

typedef struct FieldEffectGfx {
    enum HeapID heapID;
    u16 recordCount;
    u16 resourceCount;
    u16 sequenceCount;
    u16 resCount;
    void *billboards;
    void *resourceBank; // ov01_021FC4C4 banks, keyed by id
    void *sequenceBank;
    GF_3DGfxRawResMan *resMan;
    FieldEffectRecordSlot *slots;
    FieldEffectRecord *records;
} FieldEffectGfx;

struct FieldEffectManager {
    enum HeapID heapID;
    u32 rendererCount;
    u32 animManagerCount;
    u32 unk_0C;
    FieldSystem *fieldSystem;
    FieldEffectRendererEntry *renderers;
    NARC *narc;
    void *effects;
    FieldEffectGfx *gfx;
};

typedef struct FieldEffectRendererTemplate {
    u32 id;
    void *(*init)(FieldEffectManager *manager);
    void (*free)(void *data);
} FieldEffectRendererTemplate;

typedef struct FieldEffectTexTransfer {
    int state;
    int id;
    GF_3DGfxRawResMan *resMan;
} FieldEffectTexTransfer;

typedef struct FieldEffectBillboardListTemplate {
    u32 count;
    enum HeapID heapID;
} FieldEffectBillboardListTemplate;

typedef struct FieldEffectBillboardTemplate {
    void *billboards;
    FieldEffectRecord *record;
    VecFx32 position;
    VecFx32 scale;
} FieldEffectBillboardTemplate;

extern const FieldEffectRendererTemplate gFieldEffectRenderers[];

void FieldEffectManager_InitRenderer(FieldEffectManager *manager, u32 id);
void FieldEffectManager_OpenNarc(FieldEffectManager *manager);
void FieldEffectManager_CloseNarc(FieldEffectManager *manager);
void FieldEffectManager_ClearRenderers(FieldEffectManager *manager);
void FieldEffectManager_AddRenderer(FieldEffectManager *manager, u32 id);
void FieldEffectManager_FreeRenderer(FieldEffectManager *manager, FieldEffectRendererEntry *entry);
void FieldEffectManager_FreeRenderers(FieldEffectManager *manager);
FieldEffectRendererEntry *FieldEffectManager_GetFreeRendererEntry(FieldEffectManager *manager);
FieldEffectRendererEntry *FieldEffectManager_FindRendererEntry(FieldEffectManager *manager, u32 id);
void FieldEffectRendererEntry_Clear(FieldEffectRendererEntry *entry);
void FieldEffectRendererEntry_Set(FieldEffectRendererEntry *entry, u32 id, void *data);
BOOL FieldEffectRendererEntry_IsFree(FieldEffectRendererEntry *entry);
const FieldEffectRendererTemplate *FieldEffect_GetRendererTemplate(u32 id);
void FieldEffectManager_InitEffects(FieldEffectManager *manager);
void FieldEffectManager_FreeEffects(FieldEffectManager *manager);
void FieldEffectManager_DrawEffects(FieldEffectManager *manager);
void FieldEffectManager_CreateGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 resourceCount, u32 sequenceCount, u32 resCount, int resourceHeapSize, int sequenceHeapSize, int a8);
void FieldEffectManager_FreeGfx(FieldEffectManager *manager);
void FieldEffectGfx_InitRecords(FieldEffectManager *manager, FieldEffectGfx *gfx, u32 count);
void FieldEffectGfx_FreeRecords(FieldEffectGfx *gfx);
void FieldEffectGfx_ReleaseRecord(FieldEffectGfx *gfx, u32 id);
FieldEffectRecord *FieldEffectGfx_AddRecord(FieldEffectGfx *gfx, u32 id, void *a2, const FieldEffectGfxSequence *sequence, NNSG3dResTex *tex, GF_3DGfxRawResObj *texObj, int a6);
FieldEffectRecord *FieldEffectGfx_GetRecord(FieldEffectGfx *gfx, u32 id);
void FieldEffectManager_ScheduleTexTransfer(FieldEffectManager *manager, int id, GF_3DGfxRawResMan *resMan);
void FieldEffectTexTransfer_LoadVramTask(SysTask *task, void *data);
void FieldEffectTexTransfer_FreeHeaderTask(SysTask *task, void *data);
void FieldEffectManager_InitModel(FieldEffectManager *manager, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes);

#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
