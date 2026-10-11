#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H

#include "global.h"

#include "field_types_def.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "sys_task_api.h"

// Layouts of the field effect manager as seen by its own translation units. The other field effect
// modules still use the opaque FieldEffectManager of overlay_01_021F1348.h.

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
    u16 unk_06;
    u16 unk_08;
    u16 resCount;
    void *billboards;
    void *unk_10;
    void *unk_14;
    GF_3DGfxRawResMan *resMan;
    FieldEffectRecordSlot *slots;
    FieldEffectRecord *records;
} FieldEffectGfx;

typedef struct FieldEffectManager {
    enum HeapID heapID;
    u32 rendererCount;
    u32 animManagerCount;
    u32 unk_0C;
    FieldSystem *fieldSystem;
    FieldEffectRendererEntry *renderers;
    NARC *narc;
    void *effects;
    FieldEffectGfx *gfx;
} FieldEffectManager;

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

FieldEffectManager *FieldEffectManager_New(FieldSystem *fieldSystem, u32 rendererCount, enum HeapID heapID);
void FieldEffectManager_InitAnimManagerList(FieldEffectManager *manager, u32 animManagerCount);
void FieldEffectManager_InitGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 a3, u32 a4, u32 resCount, int a6, int a7, int a8);
void FieldEffectManager_InitRenderer(FieldEffectManager *manager, u32 id);
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
void FieldEffectManager_OpenNarc(FieldEffectManager *manager);
void FieldEffectManager_CloseNarc(FieldEffectManager *manager);
u32 FieldEffectManager_GetNarcMemberSize(FieldEffectManager *manager, u32 fileId);
void FieldEffectManager_ReadNarcMember(FieldEffectManager *manager, u32 fileId, void *dest);
void *FieldEffectManager_LoadNarcMember(FieldEffectManager *manager, u32 fileId, BOOL atEnd);
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
void *FieldEffectManager_CreateEffect(FieldEffectManager *manager, const void *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
void FieldEffectManager_DestroyEffect(int effect);
void FieldEffectManager_CreateGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 a3, u32 a4, u32 resCount, int a6, int a7, int a8);
void FieldEffectManager_FreeGfx(FieldEffectManager *manager);
void *FieldEffectManager_CreateBillboard(FieldEffectManager *manager, FieldEffectRecord *record, const VecFx32 *position);
void *FieldEffectManager_CreateBillboardFromRecord(FieldEffectManager *manager, u32 id, const VecFx32 *position);
FieldEffectRecord *FieldEffectManager_AddRecord(FieldEffectManager *manager, u32 id, int a2, int a3, int resId, int noTexKeys, int a6);
void FieldEffectGfx_InitRecords(FieldEffectManager *manager, FieldEffectGfx *gfx, u32 count);
void FieldEffectGfx_FreeRecords(FieldEffectGfx *gfx);
void FieldEffectGfx_ReleaseRecord(FieldEffectGfx *gfx, u32 id);
FieldEffectRecord *FieldEffectGfx_AddRecord(FieldEffectGfx *gfx, u32 id, void *a2, const FieldEffectGfxSequence *sequence, NNSG3dResTex *tex, GF_3DGfxRawResObj *texObj, int a6);
FieldEffectRecord *FieldEffectGfx_GetRecord(FieldEffectGfx *gfx, u32 id);
void FieldEffectManager_ReleaseRecord(FieldEffectManager *manager, u32 id);
void ov01_021F18D4(FieldEffectManager *manager, int id, int fileId);
void *ov01_021F18F0(FieldEffectManager *manager, int id);
void *ov01_021F18FC(FieldEffectManager *manager, int id);
void ov01_021F1908(FieldEffectManager *manager, int id, int fileId);
void *ov01_021F1924(FieldEffectManager *manager, int id);
void FieldEffectManager_LoadTexResource(FieldEffectManager *manager, int id, u32 fileId, BOOL copyWithoutTex);
void FieldEffectManager_FreeTexResource(FieldEffectManager *manager, int id);
void FieldEffectManager_ScheduleTexTransfer(FieldEffectManager *manager, int id, GF_3DGfxRawResMan *resMan);
void FieldEffectTexTransfer_LoadVramTask(SysTask *task, void *data);
void FieldEffectTexTransfer_FreeHeaderTask(SysTask *task, void *data);
void ov01_021F19F4(FieldEffectManager *manager, void *a1, int a2, int a3, int a4);
void ov01_021F1A18(FieldEffectManager *manager, void *a1, int a2, int a3, int a4);
void ov01_021F1A34(FieldEffectManager *manager, void *a1, int a2, int a3, int a4);
void FieldEffectManager_InitModel(FieldEffectManager *manager, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes);
void FieldEffectManager_LoadModel(FieldEffectManager *manager, u32 fileId, BOOL atEnd, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes);
NNSG3dResTex *FieldEffectManager_GetTexture(FieldEffectManager *manager, u32 id);

#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
