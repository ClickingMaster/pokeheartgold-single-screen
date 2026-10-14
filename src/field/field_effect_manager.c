#include "global.h"

#include "field/field_effect_manager_internal.h"

#include "field_system.h"
#include "gf_3d_render.h"
#include "map_object.h"

// Not declared in any header yet.
void *FieldEffectPool_New(enum HeapID heapID, u32 count);
void FieldEffectPool_Delete(void *manager);
void FieldEffectPool_DrawAll(void *manager);
FieldEffect *FieldEffectPool_AddBasicEffect(void *manager, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority);
void FieldEffect_Destroy(int effect);
void *Billboard_Create(const FieldEffectBillboardListTemplate *template);
void Billboard_Delete(void *billboards);
void *BillboardObject_Create(const FieldEffectBillboardTemplate *template);
NNSG3dResMdl *BillboardObject_GetModel(void *object);
void BillboardGfxSequence_Init(void *data, FieldEffectGfxSequence *sequence);
void AreaLight_ClearMaterialModifyFlags(NNSG3dResMdl *mdl);
void NNS_G3dMdlSetMdlFogEnableFlagAll(NNSG3dResMdl *mdl, int flag);
void RenderModel_LoadFromNarc(void *model, int index, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd);
void RenderModel_BindTexturesOnVBlank(void *model);
void RenderAnimation_LoadFromNarc(void *animation, int unused, NARC *narc, int fileId, enum HeapID heapID, BOOL atEnd);
void RenderAnimation_InitForModel(void *animation, void *model, void *source, int unused, enum HeapID heapID);

extern const VecFx32 sBillboardDefaultScale;

FieldEffectManager *FieldEffectManager_New(FieldSystem *fieldSystem, u32 rendererCount, enum HeapID heapID) {
    FieldEffectManager *manager = Heap_Alloc(heapID, sizeof(FieldEffectManager));
    memset(manager, 0, sizeof(FieldEffectManager));
    manager->heapID = heapID;
    manager->rendererCount = rendererCount;
    manager->fieldSystem = fieldSystem;
    manager->renderers = Heap_Alloc(heapID, rendererCount * sizeof(FieldEffectRendererEntry));
    FieldEffectManager_ClearRenderers(manager);
    FieldEffectManager_OpenNarc(manager);
    return manager;
}

void FieldEffectManager_InitAnimManagerList(FieldEffectManager *manager, u32 animManagerCount) {
    manager->animManagerCount = animManagerCount;
    FieldEffectManager_InitEffects(manager);
}

void FieldEffectManager_InitGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 resourceCount, u32 sequenceCount, u32 resCount, int resourceHeapSize, int sequenceHeapSize, int a8) {
    FieldEffectManager_CreateGfx(manager, heapID, recordCount, resourceCount, sequenceCount, resCount, resourceHeapSize, sequenceHeapSize, a8);
}

void FieldEffectManager_InitRenderer(FieldEffectManager *manager, u32 id) {
    if (FieldEffectManager_FindRendererEntry(manager, id) != NULL) {
        GF_AssertFail();
        return;
    }
    FieldEffectManager_AddRenderer(manager, id);
}

void FieldEffectManager_InitRenderers(FieldEffectManager *manager, const u32 *ids) {
    while (*ids != 0x17) {
        FieldEffectManager_InitRenderer(manager, *ids);
        ids++;
    }
}

void FieldEffectManager_Render(FieldEffectManager *manager) {
    FieldEffectManager_DrawEffects(manager);
}

void FieldEffectManager_Free(FieldEffectManager *manager) {
    FieldEffectManager_FreeEffects(manager);
    FieldEffectManager_FreeRenderers(manager);
    FieldEffectManager_FreeGfx(manager);
    FieldEffectManager_CloseNarc(manager);
    Heap_Free(manager->renderers);
    Heap_Free(manager);
}

void *FieldEffectManager_HeapAlloc(FieldEffectManager *manager, u32 size, BOOL atEnd) {
    if (atEnd == FALSE) {
        return Heap_Alloc(manager->heapID, size);
    }
    return Heap_AllocAtEnd(manager->heapID, size);
}

void *FieldEffectManager_HeapAllocAndFill(FieldEffectManager *manager, u32 size, BOOL atEnd, int fill) {
    void *ptr = FieldEffectManager_HeapAlloc(manager, size, atEnd);
    memset(ptr, fill, size);
    return ptr;
}

void FieldEffectManager_HeapFree(void *ptr) {
    Heap_Free(ptr);
}

void *FieldEffectManager_GetRendererData(FieldEffectManager *manager, u32 id) {
    FieldEffectRendererEntry *entry = FieldEffectManager_FindRendererEntry(manager, id);
    if (entry == NULL) {
        GF_AssertFail();
        return NULL;
    }
    return entry->data;
}

FieldSystem *FieldEffectManager_GetFieldSystem(FieldEffectManager *manager) {
    return manager->fieldSystem;
}

FieldEffectManager *FieldEffectManager_GetFromMapObject(LocalMapObject *mapObject) {
    // fieldSystem->fieldEffectManager (field_system.h declares the opaque FieldEffectManager)
    return MapObject_GetFieldSystem(mapObject)->fieldEffectManager;
}

enum HeapID FieldEffectManager_GetHeapID(FieldEffectManager *manager) {
    return manager->heapID;
}

void FieldEffectManager_OpenNarc(FieldEffectManager *manager) {
    manager->narc = NARC_New(NARC_a_1_0_3, FieldEffectManager_GetHeapID(manager));
}

void FieldEffectManager_CloseNarc(FieldEffectManager *manager) {
    NARC_Delete(manager->narc);
}

u32 FieldEffectManager_GetNarcMemberSize(FieldEffectManager *manager, u32 fileId) {
    return NARC_GetMemberSize(manager->narc, fileId);
}

void FieldEffectManager_ReadNarcMember(FieldEffectManager *manager, u32 fileId, void *dest) {
    NARC_ReadWholeMember(manager->narc, fileId, dest);
}

void *FieldEffectManager_LoadNarcMember(FieldEffectManager *manager, u32 fileId, BOOL atEnd) {
    void *data = FieldEffectManager_HeapAlloc(manager, NARC_GetMemberSize(manager->narc, fileId), atEnd);
    NARC_ReadWholeMember(manager->narc, fileId, data);
    return data;
}

void FieldEffectManager_ClearRenderers(FieldEffectManager *manager) {
    u32 i = manager->rendererCount;
    FieldEffectRendererEntry *entry = manager->renderers;
    while (i != 0) {
        FieldEffectRendererEntry_Clear(entry);
        entry++;
        i--;
    }
}

void FieldEffectManager_AddRenderer(FieldEffectManager *manager, u32 id) {
    void *data = FieldEffect_GetRendererTemplate(id)->init(manager);
    FieldEffectRendererEntry_Set(FieldEffectManager_GetFreeRendererEntry(manager), id, data);
}

void FieldEffectManager_FreeRenderer(FieldEffectManager *manager, FieldEffectRendererEntry *entry) {
    FieldEffect_GetRendererTemplate(entry->id)->free(entry->data);
    FieldEffectRendererEntry_Clear(entry);
}

void FieldEffectManager_FreeRenderers(FieldEffectManager *manager) {
    u32 i = manager->rendererCount;
    FieldEffectRendererEntry *entry = manager->renderers;
    while (i != 0) {
        if (!FieldEffectRendererEntry_IsFree(entry)) {
            FieldEffectManager_FreeRenderer(manager, entry);
        }
        entry++;
        i--;
    }
}

FieldEffectRendererEntry *FieldEffectManager_GetFreeRendererEntry(FieldEffectManager *manager) {
    u32 i = manager->rendererCount;
    FieldEffectRendererEntry *entry = manager->renderers;
    while (i != 0) {
        if (FieldEffectRendererEntry_IsFree(entry) == TRUE) {
            return entry;
        }
        entry++;
        i--;
    }
    GF_AssertFail();
    return NULL;
}

FieldEffectRendererEntry *FieldEffectManager_FindRendererEntry(FieldEffectManager *manager, u32 id) {
    u32 i = manager->rendererCount;
    FieldEffectRendererEntry *entry = manager->renderers;
    while (i != 0) {
        if (entry->id == id) {
            return entry;
        }
        entry++;
        i--;
    }
    return NULL;
}

void FieldEffectRendererEntry_Clear(FieldEffectRendererEntry *entry) {
    entry->id = 0x17;
    entry->data = NULL;
}

void FieldEffectRendererEntry_Set(FieldEffectRendererEntry *entry, u32 id, void *data) {
    entry->id = id;
    entry->data = data;
}

BOOL FieldEffectRendererEntry_IsFree(FieldEffectRendererEntry *entry) {
    if (entry->id == 0x17) {
        return TRUE;
    }
    return FALSE;
}

const FieldEffectRendererTemplate *FieldEffect_GetRendererTemplate(u32 id) {
    const FieldEffectRendererTemplate *renderer = gFieldEffectRenderers;
    while (renderer->id != 0x17) {
        if (renderer->id == id) {
            return renderer;
        }
        renderer++;
    }
    GF_AssertFail();
    return NULL;
}

void FieldEffectManager_InitEffects(FieldEffectManager *manager) {
    manager->effects = FieldEffectPool_New(manager->heapID, manager->animManagerCount);
}

void FieldEffectManager_FreeEffects(FieldEffectManager *manager) {
    if (manager->effects != NULL) {
        FieldEffectPool_Delete(manager->effects);
        manager->effects = NULL;
    }
}

void FieldEffectManager_DrawEffects(FieldEffectManager *manager) {
    if (manager->effects != NULL) {
        FieldEffectPool_DrawAll(manager->effects);
    }
}

FieldEffect *FieldEffectManager_CreateEffect(FieldEffectManager *manager, const FieldEffectBasicTemplate *template, const VecFx32 *position, int parameter, void *userData, u32 priority) {
    FieldEffect *effect = FieldEffectPool_AddBasicEffect(manager->effects, template, position, parameter, userData, priority);
    GF_ASSERT(effect != NULL);
    return effect;
}

void FieldEffectManager_DestroyEffect(int effect) {
    FieldEffect_Destroy(effect);
}

void FieldEffectManager_CreateGfx(FieldEffectManager *manager, enum HeapID heapID, u32 recordCount, u32 resourceCount, u32 sequenceCount, u32 resCount, int resourceHeapSize, int sequenceHeapSize, int a8) {
    FieldEffectBillboardListTemplate template;
    FieldEffectGfx *gfx = FieldEffectManager_HeapAllocAndFill(manager, sizeof(FieldEffectGfx), FALSE, 0);
#pragma unused(a8)
    manager->gfx = gfx;
    gfx->heapID = heapID;
    gfx->recordCount = recordCount;
    gfx->resourceCount = resourceCount;
    gfx->sequenceCount = sequenceCount;
    gfx->resCount = resCount;
    gfx->resourceBank = ResourceBank_Create(heapID, HEAP_ID_68, resourceHeapSize, resourceCount);
    gfx->sequenceBank = ResourceBank_Create(heapID, HEAP_ID_69, sequenceHeapSize, sequenceCount);
    gfx->resMan = GF3dGfxRawResMan_Create(resCount, heapID);
    FieldEffectGfx_InitRecords(manager, gfx, recordCount);
    template.count = recordCount;
    template.heapID = FieldEffectManager_GetHeapID(manager);
    gfx->billboards = Billboard_Create(&template);
}

void FieldEffectManager_FreeGfx(FieldEffectManager *manager) {
    FieldEffectGfx *gfx = manager->gfx;
    if (gfx != NULL) {
        Billboard_Delete(gfx->billboards);
        FieldEffectGfx_FreeRecords(gfx);
        ResourceBank_Destroy(gfx->resourceBank);
        ResourceBank_Destroy(gfx->sequenceBank);
        GF3dGfxRawResMan_Destroy(gfx->resMan);
        FieldEffectManager_HeapFree(gfx);
        manager->gfx = NULL;
    }
}

void *FieldEffectManager_CreateBillboard(FieldEffectManager *manager, FieldEffectRecord *record, const VecFx32 *position) {
    FieldEffectBillboardTemplate template;
    void *billboard;
    template.billboards = manager->gfx->billboards;
    template.record = record;
    template.position = *position;
    template.scale = sBillboardDefaultScale;
    billboard = BillboardObject_Create(&template);
    GF_ASSERT(billboard != NULL);
    if (billboard != NULL) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(BillboardObject_GetModel(billboard), 1);
        AreaLight_ClearMaterialModifyFlags(BillboardObject_GetModel(billboard));
    }
    return billboard;
}

void *FieldEffectManager_CreateBillboardFromRecord(FieldEffectManager *manager, u32 id, const VecFx32 *position) {
    return FieldEffectManager_CreateBillboard(manager, FieldEffectGfx_GetRecord(manager->gfx, id), position);
}

#pragma force_active on

static inline FieldEffectGfx *FieldEffectManager_GetGfx(FieldEffectManager *manager) {
    return manager->gfx;
}

FieldEffectRecord *FieldEffectManager_AddRecord(FieldEffectManager *manager, u32 id, int resourceId, int sequenceId, int resId, int noTexKeys, const void *params) {
    FieldEffectGfxSequence sequence;
    int seqId = sequenceId;
    FieldEffectGfx *gfx = FieldEffectManager_GetGfx(manager);
    void *data = ResourceBank_Get(gfx->resourceBank, resourceId);
    GF_3DGfxRawResObj *texObj;
    NNSG3dResTex *tex;
    FieldEffectRecord *record;
    BillboardGfxSequence_Init(ResourceBank_Get(gfx->sequenceBank, seqId), &sequence);
    texObj = GF3dGfxRawResMan_GetObjById(gfx->resMan, resId);
    GF_ASSERT(texObj);
    tex = GF3dGfxRawResObj_GetTex(texObj);
    if (noTexKeys == TRUE) {
        texObj = NULL;
    }
    record = FieldEffectGfx_AddRecord(gfx, id, data, &sequence, tex, texObj, (int)params);
    GF_ASSERT(record);
    return record;
}

#pragma force_active reset

void FieldEffectGfx_InitRecords(FieldEffectManager *manager, FieldEffectGfx *gfx, u32 count) {
    FieldEffectRecord *records = FieldEffectManager_HeapAlloc(manager, count * sizeof(FieldEffectRecord), FALSE);
    FieldEffectRecordSlot *slots;
    gfx->records = records;
    slots = FieldEffectManager_HeapAlloc(manager, count * sizeof(FieldEffectRecordSlot), FALSE);
    gfx->slots = slots;
    do {
        slots->id = 0xF;
        slots->record = records;
        slots++;
        records++;
    } while (--count != 0);
}

void FieldEffectGfx_FreeRecords(FieldEffectGfx *gfx) {
    FieldEffectManager_HeapFree(gfx->slots);
    FieldEffectManager_HeapFree(gfx->records);
}

void FieldEffectGfx_ReleaseRecord(FieldEffectGfx *gfx, u32 id) {
    u32 i = gfx->recordCount;
    FieldEffectRecordSlot *slot = gfx->slots;
    do {
        if (slot->id == id) {
            slot->id = 0xF;
            return;
        }
        slot++;
    } while (--i != 0);
    GF_AssertFail();
}

#pragma force_active on

FieldEffectRecord *FieldEffectGfx_AddRecord(FieldEffectGfx *gfx, u32 id, void *a2, const FieldEffectGfxSequence *sequence, NNSG3dResTex *tex, GF_3DGfxRawResObj *texObj, int a6) {
    FieldEffectRecord *record = NULL;
    u32 count = gfx->recordCount;
    FieldEffectRecordSlot *slots = gfx->slots;
    u32 i;
    FieldEffectRecordSlot *slot;
    i = count;
    slot = slots;
    do {
        if (slot->id == id) {
            return slot->record;
        }
        slot++;
    } while (--i);
    i = count;
    slot = slots;
    do {
        if (slot->id == 0xF) {
            slot->id = id;
            record = slot->record;
            break;
        }
        slot++;
    } while (--i);
    if (record == NULL) {
        GF_AssertFail();
    }
    memset(record, 0, sizeof(FieldEffectRecord));
    record->unk_00 = a2;
    record->sequence = *sequence;
    record->tex = tex;
    if (texObj != NULL) {
        record->texKey = GF3dGfxRawResObj_GetTexKey(texObj);
        record->tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(texObj);
        record->plttKey = GF3dGfxRawResObj_GetPlttKey(texObj);
    }
    record->unk_08 = a6;
    return record;
}

#pragma force_active reset

FieldEffectRecord *FieldEffectGfx_GetRecord(FieldEffectGfx *gfx, u32 id) {
    u32 i = gfx->recordCount;
    FieldEffectRecordSlot *slot = gfx->slots;
    do {
        if (slot->id == id) {
            return slot->record;
        }
        slot++;
    } while (--i != 0);
    GF_AssertFail();
    return NULL;
}

void FieldEffectManager_ReleaseRecord(FieldEffectManager *manager, u32 id) {
    FieldEffectGfx_ReleaseRecord(manager->gfx, id);
}

void FieldEffectManager_LoadBillboardResource(FieldEffectManager *manager, int id, int fileId) {
    ResourceBank_LoadFromNarc(manager->gfx->resourceBank, id, manager->narc, fileId, FALSE);
}

void *FieldEffectManager_GetBillboardResource(FieldEffectManager *manager, int id) {
    return ResourceBank_Get(manager->gfx->resourceBank, id);
}

void FieldEffectManager_FreeBillboardResource(FieldEffectManager *manager, int id) {
    ResourceBank_Free(manager->gfx->resourceBank, id);
}

void FieldEffectManager_LoadBillboardSequence(FieldEffectManager *manager, int id, int fileId) {
    ResourceBank_LoadFromNarc(manager->gfx->sequenceBank, id, manager->narc, fileId, FALSE);
}

void FieldEffectManager_FreeBillboardSequence(FieldEffectManager *manager, int id) {
    ResourceBank_Free(manager->gfx->sequenceBank, id);
}

void FieldEffectManager_LoadTexResource(FieldEffectManager *manager, int id, u32 fileId, BOOL copyWithoutTex) {
    FieldEffectGfx *gfx = manager->gfx;
    void *data = FieldEffectManager_LoadNarcMember(manager, fileId, TRUE);
    GF_ASSERT(GF3dGfxRawResMan_AllocObjAndKeys(gfx->resMan, data, id, copyWithoutTex, FieldEffectManager_GetHeapID(manager)) != NULL);
    FieldEffectManager_ScheduleTexTransfer(manager, id, gfx->resMan);
}

void FieldEffectManager_FreeTexResource(FieldEffectManager *manager, int id) {
    GF3dGfxRawResMan_FreeObjById(manager->gfx->resMan, id);
}

// Uploads the texture in the next VBlank, then drops its CPU copy.
void FieldEffectManager_ScheduleTexTransfer(FieldEffectManager *manager, int id, GF_3DGfxRawResMan *resMan) {
    FieldEffectTexTransfer *transfer = FieldEffectManager_HeapAlloc(manager, sizeof(FieldEffectTexTransfer), TRUE);
    transfer->state = 1;
    transfer->id = id;
    transfer->resMan = resMan;
    SysTask_CreateOnVBlankQueue(FieldEffectTexTransfer_LoadVramTask, transfer, 0xFF);
    SysTask_CreateOnVWaitQueue(FieldEffectTexTransfer_FreeHeaderTask, transfer, 0xFF);
}

void FieldEffectTexTransfer_LoadVramTask(SysTask *task, void *data) {
    FieldEffectTexTransfer *transfer = data;
    GF3dGfxRawResMan_LoadObjTexById(transfer->resMan, transfer->id);
    transfer->state = 1;
    SysTask_Destroy(task);
}

void FieldEffectTexTransfer_FreeHeaderTask(SysTask *task, void *data) {
    FieldEffectTexTransfer *transfer = data;
    if (transfer->state == 1) {
        GF3dGfxRawResMan_FreeObjVramAndSecondaryHeaderById(transfer->resMan, transfer->id);
        FieldEffectManager_HeapFree(transfer);
        SysTask_Destroy(task);
    }
}

void FieldEffectManager_LoadRenderModel(FieldEffectManager *manager, void *model, int index, int fileId, BOOL atEnd) {
    RenderModel_LoadFromNarc(model, index, manager->narc, fileId, manager->heapID, atEnd);
    RenderModel_BindTexturesOnVBlank(model);
}

void FieldEffectManager_LoadRenderAnimation(FieldEffectManager *manager, void *animation, int index, int fileId, BOOL atEnd) {
    RenderAnimation_LoadFromNarc(animation, index, manager->narc, fileId, manager->heapID, atEnd);
}

void FieldEffectManager_InitRenderAnimation(FieldEffectManager *manager, void *animation, void *model, void *source, int unused) {
    RenderAnimation_InitForModel(animation, model, source, unused, manager->heapID);
}

#pragma force_active on

void FieldEffectManager_InitModel(FieldEffectManager *manager, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes) {
    NNSG3dResTex *tex = NNS_G3dGetTex(*ppRes);
    if (tex != NULL && !GF3dRender_ResTexIsLoaded(tex)) {
        DC_FlushRange(*ppRes, (*ppRes)->fileSize);
        if (!NNS_G3dResDefaultSetup(*ppRes)) {
            GF_AssertFail();
        }
    }
    *ppMdl = NNS_G3dGetMdlByIdx(NNS_G3dGetMdlSet(*ppRes), 0);
    GF_ASSERT(ppMdl != NULL);
    NNS_G3dRenderObjInit(renderObj, *ppMdl);
}

#pragma force_active reset

void FieldEffectManager_LoadModel(FieldEffectManager *manager, u32 fileId, BOOL atEnd, NNSG3dRenderObj *renderObj, NNSG3dResMdl **ppMdl, NNSG3dResFileHeader **ppRes) {
    *ppRes = FieldEffectManager_LoadNarcMember(manager, fileId, atEnd);
    FieldEffectManager_InitModel(manager, renderObj, ppMdl, ppRes);
}
