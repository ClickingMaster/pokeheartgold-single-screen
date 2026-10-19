#ifndef POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H

#include "global.h"

#include "billboard_internal.h"
#include "field_types_def.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "sys_task_api.h"

#include "field/field_effect_manager.h"
#include "field/resource_bank.h"

// Layouts of the field effect manager and its private helpers.

typedef struct FieldEffectRendererEntry {
    u32 id; // 0x17: free
    void *data;
} FieldEffectRendererEntry;

typedef struct FieldEffectRecordSlot {
    u32 id; // 0xF: free
    BillboardResource *record;
} FieldEffectRecordSlot;

typedef struct FieldEffectGfx {
    enum HeapID heapID;
    u16 recordCount;
    u16 resourceCount;
    u16 sequenceCount;
    u16 resCount;
    Billboard *billboards;
    ResourceBank *resourceBank;
    ResourceBank *sequenceBank;
    GF_3DGfxRawResMan *resMan;
    FieldEffectRecordSlot *slots;
    BillboardResource *records;
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

extern const FieldEffectRendererTemplate gFieldEffectRenderers[];


#endif // POKEHEARTGOLD_FIELD_FIELD_EFFECT_MANAGER_INTERNAL_H
