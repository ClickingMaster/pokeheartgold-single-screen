#include "field/map_object_render.h"

#include "global.h"

#include "field/field_effect_helpers.h"
#include "field/field_effect_manager.h"
#include "field/map_object_billboard_motion.h"
#include "field/resource_bank.h"

#include "billboard_internal.h"
#include "billboard_lists.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "map_object.h"
#include "metatile_behavior.h"
#include "sys_task_api.h"

// One cached billboard resource: the model and texture of a sprite, its texture ranges and gfx sequence, as
// passed to BillboardObject_Create. 0x28 bytes, one per map object.
typedef struct MapObjectBillboardResource {
    void *modelFile;
    NNSG3dResTex *texture;
    u32 ranges;
    u8 sequence[0x10];
    NNSG3dTexKey texKey;
    NNSG3dTexKey tex4x4Key;
    NNSG3dPlttKey plttKey;
} MapObjectBillboardResource;

// Sprite ID -> its cached resource; spriteId 0xFFFF marks a free slot
typedef struct BillboardResourceSlot {
    u32 spriteId;
    MapObjectBillboardResource *resource;
} BillboardResourceSlot;

// A map object waiting for its billboard (created by the create task once the textures are in VRAM)
typedef struct PendingBillboard {
    u32 spriteId;
    LocalMapObject *object;
    void **billboard;
} PendingBillboard;

// A texture to load from the model NARC on a later frame; textures is NULL once done
typedef struct TextureLoadRequest {
    u32 modelId;
    u32 spriteId;
    u32 dynamic;
    GF_3DGfxRawResMan *textures;
} TextureLoadRequest;

// A texture to transfer to (loaded = 0) or release from (loaded = 1) VRAM during the vblank
typedef struct TextureTransfer {
    u32 loaded;
    u32 spriteId;
    GF_3DGfxRawResMan *textures;
} TextureTransfer;

typedef struct MapObjectRenderQueue {
    s16 count;
    s16 loadsPerFrame;
    s16 loadsThisFrame;
    u16 busy;
    PendingBillboard *pending;
    TextureLoadRequest *loads;
    TextureTransfer *transfers;
    u32 unused14;
    SysTask *loadTask;
    SysTask *createTask;
    SysTask *transferTask;
    SysTask *releaseTask;
} MapObjectRenderQueue; // size: 0x28

// The id lists hold the resident ids first (loaded at init, kept) and the dynamic ids after them (loaded on
// demand, released when no map object uses them any more). Free entries are 0xFF (models, sequences) or 0xFFFF
// (textures).
struct MapObjectRender {
    u32 unused00;
    u32 objectCount;
    u32 residentModelCount;
    u32 dynamicModelCount;
    u32 residentSequenceCount;
    u32 dynamicSequenceCount;
    u32 residentTextureCount;
    u32 dynamicTextureCount;
    u32 modelIds[8];
    u32 sequenceIds[8];
    u32 textureIds[32];
    void *billboards;
    MapObjectBillboardResource *resources;
    void *unkE8;
    void *unkEC;
    GF_3DGfxRawResMan *textures;
    BillboardResourceSlot *resourceSlots;
    ResourceBank *modelBank;
    ResourceBank *sequenceBank;
    MapObjectRenderQueue *queue;
    MapObjectManager *manager;
};

// Model / gfx sequence NARC member of a billboard model or sequence id, 0xFF-terminated
typedef struct BillboardModelEntry {
    u16 id;
    u16 member;
} BillboardModelEntry;

// The billboard info of a sprite (ov01_022074A8, 0xFFFF-terminated): index into ov01_02207318
typedef struct SpriteBillboardIndex {
    u16 spriteId;
    u16 unused;
    u16 unk4 : 10;
    u16 index : 6;
} SpriteBillboardIndex;

typedef struct SpriteBillboardInfo {
    u8 unk0[2];
    u8 modelId;
    u8 sequenceId;
    u32 ranges;
} SpriteBillboardInfo;

// What BillboardObject_SaveState keeps of a billboard while its map object is paused
typedef struct BillboardState {
    u32 visible;
    u16 unused4;
    u16 sequence;
    fx32 frame;
} BillboardState;

typedef struct MapObjectBillboardCreateParams {
    void *billboards;
    MapObjectBillboardResource *resource;
    VecFx32 position;
    VecFx32 scale;
} MapObjectBillboardCreateParams;

// Keeps a copy of the old billboard on screen while the new sprite's billboard is being loaded
typedef struct SpriteChangeWork {
    u32 spriteId;
    u32 state;
    void *modelFile;
    void *sequenceFile;
    NNSG3dResFileHeader *textureFile;
    u8 sequence[0x10];
    void *billboard;
    MapObjectBillboardResource resource;
    LocalMapObject *object;
    void *reflection;
} SpriteChangeWork;

extern const VecFx32 ov01_02208B64;
extern const u32 ov01_02208B70[];
extern const u32 ov01_02208B80[];
extern const u32 ov01_02208B90[];
extern const BillboardModelEntry ov01_02207294[];
extern const BillboardModelEntry ov01_022072CC[];
extern const u32 ov01_02207260[];
extern const u32 ov01_02206CF0[];
extern const SpriteBillboardIndex ov01_022074A8[];
extern const u8 ov01_02207318[];

// billboard_lists.c
void *Billboard_Create(BillboardCreateParams *params);
void Billboard_Delete(void *billboards);
BOOL sub_020238F8(void *billboards);
void *BillboardObject_Create(MapObjectBillboardCreateParams *params);
VecFx32 *BillboardObject_GetPosition(void *billboard);
u32 BillboardObject_IsVisible(void *billboard);
void BillboardGfxSequence_Init(void *sequenceFile, void *sequence);
void BillboardResource_Init(MapObjectBillboardResource *resource, void *modelFile, NNSG3dResTex *texture, u32 ranges, void *sequence);
// field_object_models.c
void MapObject_GetDrawPosition(LocalMapObject *object, VecFx32 *position);
void *ReadMModelFromNarcInternal(MapObjectManager *manager, u32 memberIdx, BOOL atStart);
int GetMoveModelNoBySpriteId(u32 spriteId);
// render_animation.c, field reflections
void ov01_021EA3B0(NNSG3dResMdl *model);
void *Reflection_SpawnForBillboard(void *manager, MapObjectBillboardResource *resource, void *billboard, VecFx32 *position, int kind, int priority);
// g3d_util_internal.h is not included here (it takes const / u32)
int NNS_G3dMdlGetMdlFogEnableFlag(NNSG3dResMdl *model, int matId);
int MapObject_GetMetatileBehaviorInDirection(LocalMapObject *object, int direction);
// field_object_models.c: the graphics info flags of a map object (bits 0-3: 1 for billboards)
typedef struct MapObjectGfxFlags {
    u16 low : 4;
    u16 rest : 12;
} MapObjectGfxFlags;
MapObjectGfxFlags *MapObject_GetEffectFlags(LocalMapObject *object);

void MapObjectRender_Init(MapObjectRender *render, MapObjectManager *manager, u32 count, u32 priority, u32 textureCount, u32 residentTextureCount, const u32 *textureIds, u32 loadsPerFrame);
void MapObjectRender_Free(MapObjectRender *render);
int MapObject_RequestBillboard(LocalMapObject *object, void **billboard, u32 spriteId);
void *ov01_021F9528(LocalMapObject *object, u32 spriteId);
void ov01_021F9574(LocalMapObject *object, void **billboard, u32 spriteId);
void ov01_021F95CC(LocalMapObject *object, void **billboard, u32 spriteId);
void BillboardObject_SaveState(void *billboard, BillboardState *state);
void BillboardObject_RestoreState(void *billboard, BillboardState *state);
void ov01_021F9654(LocalMapObject *object, void **billboard, u32 spriteId);
void ov01_021F9688(LocalMapObject *object);
void ov01_021F9698(MapObjectRender *render, u32 count);
void ov01_021F96E4(MapObjectRender *render);
MapObjectBillboardResource *ov01_021F9704(MapObjectRender *render, u32 spriteId);
BOOL MapObjectRender_GetBillboardResource(MapObjectManager *manager, u32 spriteId, MapObjectBillboardResource *dest);
void ov01_021F9778(MapObjectRender *render, u32 spriteId);
void ov01_021F9798(MapObjectRender *render);
BOOL ov01_021F97BC(MapObjectManager *manager, LocalMapObject *self, u32 spriteId);
void ov01_021F9808(MapObjectRender *render, u32 modelCount, u32 residentModelCount, u32 sequenceCount, u32 residentSequenceCount, u32 textureCount, u32 residentTextureCount);
void ov01_021F9890(MapObjectRender *render);
void *ov01_021F98B4(int id, int terminator, const BillboardModelEntry *entries);
BOOL ov01_021F98CC(MapObjectRender *render, ResourceBank *bank, int id, int terminator, const BillboardModelEntry *entries);
int ov01_021F9918(MapObjectRender *render, u32 dynamic, u32 spriteId);
void ov01_021F9968(MapObjectRender *render, u32 spriteId);
int ov01_021F9974(MapObjectRender *render, u32 spriteId);
void ov01_021F9980(MapObjectRender *render, const u32 *spriteIds);
void ov01_021F99A4(MapObjectRender *render, u32 id);
void ov01_021F99D0(MapObjectRender *render, u32 id);
void ov01_021F99FC(MapObjectRender *render, const u32 *ids);
void ov01_021F9A18(MapObjectRender *render, u32 id);
void ov01_021F9A44(MapObjectRender *render, u32 id);
void ov01_021F9A70(MapObjectRender *render, const u32 *ids);
void ov01_021F9A8C(MapObjectRender *render, const u32 *textureIds, const u32 *modelIds, const u32 *sequenceIds);
void ov01_021F9AAC(u32 *ids, u32 value, u32 count);
BOOL ov01_021F9AB4(u32 *ids, u32 value, u32 old, u32 count);
BOOL ov01_021F9AD0(u32 *ids, u32 value, u32 count);
BOOL ov01_021F9AE4(u32 *ids, u32 old, u32 value, u32 count);
void ov01_021F9B00(MapObjectRender *render);
void ov01_021F9B10(MapObjectRender *render, u32 id);
BOOL ov01_021F9B38(MapObjectRender *render, u32 id);
void ov01_021F9B54(MapObjectRender *render, u32 id);
BOOL ov01_021F9B84(MapObjectRender *render, u32 id);
BOOL ov01_021F9BAC(MapObjectRender *render, u32 id);
void ov01_021F9BD4(MapObjectManager *manager, MapObjectRender *render);
void ov01_021F9C24(MapObjectRender *render);
void ov01_021F9C34(MapObjectRender *render, u32 id);
BOOL ov01_021F9C5C(MapObjectRender *render, u32 id);
void ov01_021F9C78(MapObjectRender *render, u32 id);
BOOL ov01_021F9CA8(MapObjectRender *render, u32 id);
BOOL ov01_021F9CD0(MapObjectRender *render, u32 id);
void ov01_021F9CF8(MapObjectManager *manager, MapObjectRender *render);
void ov01_021F9D48(MapObjectRender *render);
void ov01_021F9D5C(MapObjectRender *render, u32 id);
BOOL ov01_021F9D88(MapObjectRender *render, u32 id);
int ov01_021F9DA4(MapObjectRender *render, u32 id);
void ov01_021F9DD0(MapObjectRender *render, u32 id);
BOOL ov01_021F9E04(MapObjectRender *render, u32 id);
void ov01_021F9E30(MapObjectManager *manager, MapObjectRender *render);
BOOL ov01_021F9E9C(MapObjectRender *render, u32 id);
int ov01_021F9EC4(MapObjectRender *render, u32 id);
int ov01_021F9EEC(MapObjectRender *render, u32 id);
int ov01_021F9F14(MapObjectRender *render, u32 id);
void ov01_021F9F3C(MapObjectRender *render, u32 spriteId);
void ov01_021F9F54(MapObjectRender *render, u32 modelId);
void ov01_021F9F6C(MapObjectRender *render, u32 sequenceId);
void ov01_021F9F84(MapObjectRender *render, u32 spriteId);
BOOL ov01_021F9FCC(MapObjectManager *manager, u32 spriteId, LocalMapObject *skip);
BOOL ov01_021FA01C(MapObjectManager *manager, u32 modelId, LocalMapObject *skip);
BOOL ov01_021FA094(MapObjectManager *manager, u32 sequenceId, LocalMapObject *skip);
void ov01_021FA1C8(MapObjectRender *render, void *billboards);
void ov01_021FA1D8(MapObjectRender *render, u32 count);
void *ov01_021FA1DC(MapObjectRender *render);
void *ov01_021FA1E4(MapObjectRender *render);
void ov01_021FA1EC(MapObjectRender *render, GF_3DGfxRawResMan *textures);
GF_3DGfxRawResMan *ov01_021FA1F4(MapObjectRender *render);
u32 *ov01_021FA1FC(MapObjectRender *render);
u32 *ov01_021FA200(MapObjectRender *render);
u32 *ov01_021FA204(MapObjectRender *render);
void ov01_021FA208(MapObjectRender *render, u32 count);
u32 ov01_021FA20C(MapObjectRender *render);
void ov01_021FA210(MapObjectRender *render, u32 count);
u32 ov01_021FA214(MapObjectRender *render);
void ov01_021FA218(MapObjectRender *render, u32 count);
u32 ov01_021FA21C(MapObjectRender *render);
void ov01_021FA220(MapObjectRender *render, u32 count);
u32 ov01_021FA224(MapObjectRender *render);
void ov01_021FA228(MapObjectRender *render, u32 count);
u32 ov01_021FA22C(MapObjectRender *render);
void ov01_021FA230(MapObjectRender *render, u32 count);
u32 ov01_021FA234(MapObjectRender *render);
void *FldObjSys_ReadMModelFromNarc(MapObjectRender *render, u32 memberIdx, BOOL atStart);
const SpriteBillboardInfo *SpriteId_GetBillboardInfo(u32 spriteId);
u8 SpriteId_GetBillboardModelId(u32 spriteId);
u8 ov01_021FA298(u32 spriteId);
u8 SpriteId_GetBillboardSequenceId(u32 spriteId);
u32 SpriteId_GetBillboardRanges(u32 spriteId);
void MapObject_SetBillboardPending(LocalMapObject *object, BOOL set);
void ov01_021FA2EC(MapObjectRender *render, u32 count);
void ov01_021FA314(void *billboards);
void *ov01_021FA31C(void *billboards, MapObjectBillboardResource *resource, VecFx32 *position);
void ov01_021FA370(MapObjectRender *render, u32 spriteId, MapObjectBillboardResource *resource);
MapObjectRender *MapObject_GetRender(LocalMapObject *object);
void ov01_021FA40C(LocalMapObject *object, void *billboard);
int ov01_021FA470(MapObjectRender *render, u32 spriteId, u32 modelId, u32 dynamic);
void ov01_021FA4F0(MapObjectRender *render, u32 spriteId);
int ov01_021FA524(MapObjectRender *render, u32 spriteId);
void ov01_021FA564(SysTask *task, void *data);
void ov01_021FA61C(MapObjectRender *render, u32 spriteId, void *textureFile, u32 dynamic);
void ov01_021FA668(MapObjectRender *render, u32 spriteId);
void ov01_021FA6A4(MapObjectRender *render, u32 spriteId);
void ov01_021FA6E0(SysTask *task, void *data);
void ov01_021FA71C(SysTask *task, void *data);
void ov01_021FA75C(MapObjectRender *render, LocalMapObject *object, void **billboard, u32 spriteId);
void ov01_021FA798(MapObjectRender *render, LocalMapObject *object);
void ov01_021FA7C8(MapObjectRender *render);
void ov01_021FA7F8(SysTask *task, void *data);
void ov01_021FA804(MapObjectRender *render, PendingBillboard *pending);
void ov01_021FA854(MapObjectRender *render, u32 count, u32 loadsPerFrame);
void ov01_021FA8F8(MapObjectRender *render);
void ov01_021FA97C(LocalMapObject *object, u32 spriteId);
void ov01_021FAB9C(SysTask *task, SpriteChangeWork *work);

void MapObjectRender_Init(MapObjectRender *render, MapObjectManager *manager, u32 count, u32 priority, u32 textureCount, u32 residentTextureCount, const u32 *textureIds, u32 loadsPerFrame) {
    render->manager = manager;
    ov01_021FA2EC(render, count);
    ov01_021FA854(render, count, loadsPerFrame);
    ov01_021F9808(render, 8, 4, 8, 4, textureCount, residentTextureCount);
    ov01_021F9698(render, count);
    ov01_021F9A8C(render, textureIds, ov01_02207260, ov01_02206CF0);
}

void MapObjectRender_Free(MapObjectRender *render) {
    ov01_021FA8F8(render);
    ov01_021F96E4(render);
    ov01_021FA314(MapObjectRender_GetBillboards(render));
    ov01_021F9890(render);
}

// Returns 0 when the textures are in VRAM (billboard created now, or queued if the texture transfer is still
// pending), 3/4 when they are still being loaded (queued), 1/2 for a resident/dynamic texture (created now)
int MapObject_RequestBillboard(LocalMapObject *object, void **billboard, u32 spriteId) {
    MapObjectRender *render;
    int result;
    *billboard = NULL;
    render = MapObject_GetRender(object);
    result = ov01_021F9DA4(render, spriteId);
    if (result == 0) {
        ov01_021F9654(object, billboard, spriteId);
        return result;
    }
    if (result == 3 || result == 4) {
        ov01_021FA75C(render, object, billboard, spriteId);
        return result;
    }
    *billboard = ov01_021F9528(object, spriteId);
    return result;
}

void MapObject_CreateBillboard(LocalMapObject *object, void *work) {
    MapObject_RequestBillboard(object, work, MapObject_GetSpriteID(object));
}

void *ov01_021F9528(LocalMapObject *object, u32 spriteId) {
    VecFx32 position;
    MapObjectRender *render = MapObject_GetRender(object);
    void *billboards = MapObjectRender_GetBillboards(render);
    MapObjectBillboardResource *resource;
    void *billboard;
    ov01_021F9F84(render, spriteId);
    resource = ov01_021F9704(render, spriteId);
    if (resource == NULL) {
        GF_AssertFail();
    }
    MapObject_GetDrawPosition(object, &position);
    billboard = ov01_021FA31C(billboards, resource, &position);
    if (billboard == NULL) {
        GF_AssertFail();
    }
    return billboard;
}

void ov01_021F9574(LocalMapObject *object, void **billboard, u32 spriteId) {
    MapObjectManager *manager;
    BillboardObject_Delete(*billboard);
    *billboard = NULL;
    manager = MapObject_GetManager(object);
    if (!ov01_021F97BC(manager, object, spriteId)) {
        ov01_021F9778(MapObject_GetRender(object), spriteId);
    }
}

void MapObject_DeleteBillboard(LocalMapObject *object, void *work) {
    void **billboard = work;
    ov01_021F9688(object);
    if (*billboard != NULL) {
        ov01_021F9574(object, billboard, MapObject_GetSpriteID(object));
    }
}

void ov01_021F95CC(LocalMapObject *object, void **billboard, u32 spriteId) {
    ov01_021F9688(object);
    if (*billboard != NULL) {
        BillboardObject_Delete(*billboard);
        *billboard = NULL;
    }
    if (spriteId != 0xFFFF && !ov01_021F97BC(MapObject_GetManager(object), object, spriteId)) {
        ov01_021F9778(MapObject_GetRender(object), spriteId);
    }
}

void BillboardObject_SaveState(void *billboard, BillboardState *state) {
    state->visible = BillboardObject_IsVisible(billboard);
    state->sequence = BillboardObject_GetSequence(billboard);
    state->frame = BillboardObject_GetFrame(billboard);
}

void BillboardObject_RestoreState(void *billboard, BillboardState *state) {
    BillboardObject_SetVisible(billboard, (u8)state->visible);
    BillboardObject_SetSequence(billboard, state->sequence);
    BillboardObject_SetFrame(billboard, state->frame);
}

void ov01_021F9654(LocalMapObject *object, void **billboard, u32 spriteId) {
    MapObjectRender *render = MapObject_GetRender(object);
    if (!ov01_021F9974(render, spriteId)) {
        *billboard = ov01_021F9528(object, spriteId);
    } else {
        ov01_021FA75C(render, object, billboard, spriteId);
    }
}

void ov01_021F9688(LocalMapObject *object) {
    ov01_021FA798(MapObject_GetRender(object), object);
}

void ov01_021F9698(MapObjectRender *render, u32 count) {
    MapObjectBillboardResource *resources = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapObjectBillboardResource) * count);
    BillboardResourceSlot *slot;
    render->resources = resources;
    if (resources == NULL) {
        GF_AssertFail();
    }
    slot = Heap_Alloc(HEAP_ID_FIELD1, count * sizeof(BillboardResourceSlot));
    render->resourceSlots = slot;
    if (slot == NULL) {
        GF_AssertFail();
    }
    do {
        slot->spriteId = 0xFFFF;
        slot->resource = resources;
        slot++;
        resources++;
    } while (--count != 0);
}

void ov01_021F96E4(MapObjectRender *render) {
    ov01_021F9798(render);
    Heap_FreeExplicit(HEAP_ID_FIELD1, render->resources);
    Heap_FreeExplicit(HEAP_ID_FIELD1, render->resourceSlots);
}

MapObjectBillboardResource *ov01_021F9704(MapObjectRender *render, u32 spriteId) {
    u32 i;
    u32 count;
    BillboardResourceSlot *slots;
    BillboardResourceSlot *scan;

    slots = render->resourceSlots;
    count = render->objectCount;
    scan = slots;
    i = count;

    do {
        if (scan->spriteId == spriteId) {
            return scan->resource;
        }
        scan++;
    } while (--i != 0);

    do {
        if (slots->spriteId == 0xFFFF) {
            slots->spriteId = spriteId;
            ov01_021FA370(render, spriteId, slots->resource);
            return slots->resource;
        }
        slots++;
    } while (--count != 0);
    return NULL;
}

BOOL MapObjectRender_GetBillboardResource(MapObjectManager *manager, u32 spriteId, MapObjectBillboardResource *dest) {
    u32 count;
    BillboardResourceSlot *slots;
    MapObjectRender *render;

    render = sub_0205F1A0(manager);
    slots = render->resourceSlots;
    count = render->objectCount;

    do {
        if (slots->spriteId == spriteId) {
            *dest = *slots->resource;
            return TRUE;
        }
        slots++;
    } while (--count != 0);
    return FALSE;
}

void ov01_021F9778(MapObjectRender *render, u32 spriteId) {
    u32 count;
    BillboardResourceSlot *slots;

    slots = render->resourceSlots;
    count = render->objectCount;

    do {
        if (slots->spriteId == spriteId) {
            slots->spriteId = 0xFFFF;
            return;
        }
        slots++;
    } while (--count != 0);
}

void ov01_021F9798(MapObjectRender *render) {
    u32 count;
    BillboardResourceSlot *slots;

    slots = render->resourceSlots;
    count = render->objectCount;

    do {
        if (slots->spriteId != 0xFFFF) {
            slots->spriteId = 0xFFFF;
        }
        slots++;
    } while (--count != 0);
}

// TRUE if another active map object uses the sprite
BOOL ov01_021F97BC(MapObjectManager *manager, LocalMapObject *self, u32 spriteId) {
    LocalMapObject *cur;
    u32 count = MapObjectManager_GetObjectCount(manager);

    cur = MapObjectManager_GetObjects2(manager);
    do {
        if (cur != self) {
            if (MapObject_CheckActive(cur) == TRUE) {
                u32 id = MapObject_GetSpriteID(cur);
                if (id != 0xFFFF) {
                    if (id == spriteId) {
                        return TRUE;
                    }
                }
            }
        }
        MapObjectArray_NextObject(&cur);
    } while (--count != 0);
    return FALSE;
}

void ov01_021F9808(MapObjectRender *render, u32 modelCount, u32 residentModelCount, u32 sequenceCount, u32 residentSequenceCount, u32 textureCount, u32 residentTextureCount) {
    render->modelBank = ResourceBank_Create(HEAP_ID_FIELD1, (enum HeapID)0x3F, modelCount << 12, modelCount);
    ov01_021F9B00(render);
    ov01_021FA208(render, residentModelCount);
    ov01_021FA210(render, modelCount - residentModelCount);
    render->sequenceBank = ResourceBank_Create(HEAP_ID_FIELD1, (enum HeapID)0x40, sequenceCount << 7, sequenceCount);
    ov01_021F9C24(render);
    ov01_021FA218(render, residentSequenceCount);
    ov01_021FA220(render, sequenceCount - residentSequenceCount);
    ov01_021FA1EC(render, GF3dGfxRawResMan_Create(textureCount, HEAP_ID_FIELD1));
    ov01_021F9D48(render);
    ov01_021FA228(render, residentTextureCount);
    ov01_021FA230(render, textureCount - residentTextureCount);
}

void ov01_021F9890(MapObjectRender *render) {
    ResourceBank_Destroy(render->modelBank);
    ResourceBank_Destroy(render->sequenceBank);
    GF3dGfxRawResMan_Destroy(ov01_021FA1F4(render));
}

void *ov01_021F98B4(int id, int terminator, const BillboardModelEntry *entries) {
    volatile u16 *scan = (volatile u16 *)entries;
    for (;;) {
        if (*scan == id) {
            return (void *)scan;
        }
        scan += 2;
        if (*scan == terminator) {
            return NULL;
        }
    }
}

#pragma force_active on
// Loads the NARC member of the model / sequence id into the bank unless it is there; FALSE if it was
BOOL ov01_021F98CC(MapObjectRender *render, ResourceBank *bank, int id, int terminator, const BillboardModelEntry *entries) {
    const BillboardModelEntry *entry;
    NARC *narc;
    if (ResourceBank_Contains(bank, id) == TRUE) {
        return FALSE;
    }
    entry = ov01_021F98B4(id, terminator, entries);
    if (entry == NULL) {
        GF_AssertFail();
    }
    narc = MapObjectManager_GetMapModelNarc(render->manager);
    ResourceBank_LoadFromNarc(bank, id, narc, entry->member, FALSE);
    return TRUE;
}

// 0: the sprite's texture is loaded (or now loaded), 1: queued, 2: the sprite has no texture
int ov01_021F9918(MapObjectRender *render, u32 dynamic, u32 spriteId) {
    int modelId;
    if (GF3dGfxRawResMan_DoesNotHaveObjWithId(render->textures, spriteId) == FALSE) {
        return 0;
    }
    if (ov01_021FA524(render, spriteId)) {
        return 1;
    }
    modelId = GetMoveModelNoBySpriteId(spriteId);
    if (modelId < 0) {
        return 2;
    }
    switch (ov01_021FA470(render, spriteId, modelId, dynamic)) {
    case 1:
        return 0;
    default:
        return 1;
    }
}
#pragma force_active reset

void ov01_021F9968(MapObjectRender *render, u32 spriteId) {
    ov01_021F9918(render, FALSE, spriteId);
}

int ov01_021F9974(MapObjectRender *render, u32 spriteId) {
    return ov01_021F9918(render, TRUE, spriteId);
}

#pragma force_active on
void ov01_021F9980(MapObjectRender *render, const u32 *spriteIds) {
    while (*spriteIds != 0xFFFF) {
        ov01_021F9968(render, *spriteIds);
        spriteIds++;
    }
}
#pragma force_active reset

void ov01_021F99A4(MapObjectRender *render, u32 id) {
    ov01_021FA1DC(render);
    ov01_021F98CC(render, render->modelBank, id, 0xFF, ov01_02207294);
    ov01_021F9B10(render, id);
}

void ov01_021F99D0(MapObjectRender *render, u32 id) {
    ov01_021FA1DC(render);
    ov01_021F98CC(render, render->modelBank, id, 0xFF, ov01_02207294);
    ov01_021F9B54(render, id);
}

void ov01_021F99FC(MapObjectRender *render, const u32 *ids) {
    while (*ids != 0xFF) {
        ov01_021F99A4(render, *ids);
        ids++;
    }
}

void ov01_021F9A18(MapObjectRender *render, u32 id) {
    ov01_021FA1E4(render);
    ov01_021F98CC(render, render->sequenceBank, id, 0xFF, ov01_022072CC);
    ov01_021F9C34(render, id);
}

void ov01_021F9A44(MapObjectRender *render, u32 id) {
    ov01_021FA1E4(render);
    ov01_021F98CC(render, render->sequenceBank, id, 0xFF, ov01_022072CC);
    ov01_021F9C78(render, id);
}

void ov01_021F9A70(MapObjectRender *render, const u32 *ids) {
    while (*ids != 0xFF) {
        ov01_021F9A18(render, *ids);
        ids++;
    }
}

void ov01_021F9A8C(MapObjectRender *render, const u32 *textureIds, const u32 *modelIds, const u32 *sequenceIds) {
    ov01_021F9980(render, textureIds);
    ov01_021F99FC(render, modelIds);
    ov01_021F9A70(render, sequenceIds);
}

void ov01_021F9AAC(u32 *ids, u32 value, u32 count) {
    do {
        *ids++ = value;
    } while (--count != 0);
}

// Replaces the first entry equal to old by value
BOOL ov01_021F9AB4(u32 *ids, u32 value, u32 old, u32 count) {
    do {
        if (*ids == old) {
            *ids = value;
            return TRUE;
        }
        ids++;
    } while (--count != 0);
    return FALSE;
}

BOOL ov01_021F9AD0(u32 *ids, u32 value, u32 count) {
    do {
        if (*ids == value) {
            return TRUE;
        }
        ids++;
    } while (--count != 0);
    return FALSE;
}

BOOL ov01_021F9AE4(u32 *ids, u32 old, u32 value, u32 count) {
    do {
        if (*ids == old) {
            *ids = value;
            return TRUE;
        }
        ids++;
    } while (--count != 0);
    return FALSE;
}

void ov01_021F9B00(MapObjectRender *render) {
    ov01_021F9AAC(ov01_021FA200(render), 0xFF, 8);
}

void ov01_021F9B10(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA20C(render);
    u32 *ids = ov01_021FA200(render);
    if (!ov01_021F9AB4(ids, id, 0xFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9B38(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA20C(render);
    u32 *ids = ov01_021FA200(render);
    return ov01_021F9AD0(ids, id, count);
}

void ov01_021F9B54(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA200(render);
    u32 offset = ov01_021FA20C(render) << 2;
    u32 count = ov01_021FA214(render);
    if (!ov01_021F9AB4((u32 *)((u8 *)ids + offset), id, 0xFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9B84(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA200(render);
    u32 offset = ov01_021FA20C(render) << 2;
    u32 count = ov01_021FA214(render);
    return ov01_021F9AD0((u32 *)((u8 *)ids + offset), id, count);
}

BOOL ov01_021F9BAC(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA200(render);
    u32 offset = ov01_021FA20C(render) << 2;
    u32 count = ov01_021FA214(render);
    return ov01_021F9AE4((u32 *)((u8 *)ids + offset), id, 0xFF, count);
}

#pragma force_active on
// Frees the dynamic models no map object uses any more
void ov01_021F9BD4(MapObjectManager *manager, MapObjectRender *render) {
    u32 *ids;
    int count;
    u32 *cursor;
    ov01_021FA1DC(render);
    ids = ov01_021FA200(render);
    cursor = ids + ov01_021FA20C(render);
    count = ov01_021FA214(render);
    do {
        if (*cursor != 0xFF && ov01_021FA01C(manager, *cursor, NULL) == FALSE) {
            ResourceBank_Free(render->modelBank, *cursor);
            *cursor = 0xFF;
        }
        cursor++;
        count--;
    } while (count);
}
#pragma force_active reset

void ov01_021F9C24(MapObjectRender *render) {
    ov01_021F9AAC(ov01_021FA204(render), 0xFF, 8);
}

void ov01_021F9C34(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA21C(render);
    u32 *ids = ov01_021FA204(render);
    if (!ov01_021F9AB4(ids, id, 0xFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9C5C(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA21C(render);
    u32 *ids = ov01_021FA204(render);
    return ov01_021F9AD0(ids, id, count);
}

void ov01_021F9C78(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA204(render);
    u32 offset = ov01_021FA21C(render) << 2;
    u32 count = ov01_021FA224(render);
    if (!ov01_021F9AB4((u32 *)((u8 *)ids + offset), id, 0xFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9CA8(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA204(render);
    u32 offset = ov01_021FA21C(render) << 2;
    u32 count = ov01_021FA224(render);
    return ov01_021F9AD0((u32 *)((u8 *)ids + offset), id, count);
}

BOOL ov01_021F9CD0(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA204(render);
    u32 offset = ov01_021FA21C(render) << 2;
    u32 count = ov01_021FA224(render);
    return ov01_021F9AE4((u32 *)((u8 *)ids + offset), id, 0xFF, count);
}

#pragma force_active on
// Frees the dynamic gfx sequences no map object uses any more
void ov01_021F9CF8(MapObjectManager *manager, MapObjectRender *render) {
    u32 *ids;
    int count;
    u32 *cursor;
    ov01_021FA1E4(render);
    ids = ov01_021FA204(render);
    cursor = ids + ov01_021FA21C(render);
    count = ov01_021FA224(render);
    do {
        if (*cursor != 0xFF && ov01_021FA094(manager, *cursor, NULL) == FALSE) {
            ResourceBank_Free(render->sequenceBank, *cursor);
            *cursor = 0xFF;
        }
        cursor++;
        count--;
    } while (count);
}
#pragma force_active reset

void ov01_021F9D48(MapObjectRender *render) {
    ov01_021F9AAC(ov01_021FA1FC(render), 0xFFFF, 32);
}

void ov01_021F9D5C(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA22C(render);
    u32 *ids = ov01_021FA1FC(render);
    if (!ov01_021F9AB4(ids, id, 0xFFFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9D88(MapObjectRender *render, u32 id) {
    u32 count = ov01_021FA22C(render);
    u32 *ids = ov01_021FA1FC(render);
    return ov01_021F9AD0(ids, id, count);
}

// 1: resident texture, 2: dynamic texture, else the load queue state (ov01_021FA524)
int ov01_021F9DA4(MapObjectRender *render, u32 id) {
    if (ov01_021F9D88(render, id) == TRUE) {
        return 1;
    }
    if (ov01_021F9E9C(render, id) == TRUE) {
        return 2;
    }
    return ov01_021FA524(render, id);
}

void ov01_021F9DD0(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA1FC(render);
    u32 offset = ov01_021FA22C(render) << 2;
    u32 count = ov01_021FA234(render);
    if (!ov01_021F9AB4((u32 *)((u8 *)ids + offset), id, 0xFFFF, count)) {
        GF_AssertFail();
    }
}

BOOL ov01_021F9E04(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA1FC(render);
    u32 offset = ov01_021FA22C(render) << 2;
    u32 count = ov01_021FA234(render);
    return ov01_021F9AE4((u32 *)((u8 *)ids + offset), id, 0xFFFF, count);
}

#pragma force_active on
// Frees the dynamic textures no map object uses any more
void ov01_021F9E30(MapObjectManager *manager, MapObjectRender *render) {
    u32 id;
    GF_3DGfxRawResMan *textures = ov01_021FA1F4(render);
    u32 ids = (u32)ov01_021FA1FC(render);
    u32 offset = ov01_021FA22C(render) << 2;
    u32 *cursor = (u32 *)(ids + offset);
    int count = ov01_021FA234(render);
    do {
        id = *cursor;
        if (id != 0xFFFF && ov01_021F9FCC(manager, id, NULL) == FALSE) {
            GF3dGfxRawResMan_FreeObjById(textures, id);
            ov01_021FA4F0(render, id);
            ov01_021FA6A4(render, id);
            *cursor = 0xFFFF;
        }
        cursor++;
        count--;
    } while (count != 0);
}
#pragma force_active reset

BOOL ov01_021F9E9C(MapObjectRender *render, u32 id) {
    u32 *ids = ov01_021FA1FC(render);
    u32 offset = ov01_021FA22C(render) << 2;
    u32 count = ov01_021FA234(render);
    return ov01_021F9AD0((u32 *)((u8 *)ids + offset), id, count);
}

// 0: resident, 1: dynamic, 2: not loaded
int ov01_021F9EC4(MapObjectRender *render, u32 id) {
    if (ov01_021F9D88(render, id) == TRUE) {
        return 0;
    }
    if (ov01_021F9E9C(render, id) == TRUE) {
        return 1;
    }
    return 2;
}

int ov01_021F9EEC(MapObjectRender *render, u32 id) {
    if (ov01_021F9B38(render, id) == TRUE) {
        return 0;
    }
    if (ov01_021F9B84(render, id) == TRUE) {
        return 1;
    }
    return 2;
}

int ov01_021F9F14(MapObjectRender *render, u32 id) {
    if (ov01_021F9C5C(render, id) == TRUE) {
        return 0;
    }
    if (ov01_021F9CA8(render, id) == TRUE) {
        return 1;
    }
    return 2;
}

void ov01_021F9F3C(MapObjectRender *render, u32 spriteId) {
    if (ov01_021F9DA4(render, spriteId) == 0) {
        ov01_021F9974(render, spriteId);
    }
}

void ov01_021F9F54(MapObjectRender *render, u32 modelId) {
    if (ov01_021F9EEC(render, modelId) == 2) {
        ov01_021F99D0(render, modelId);
    }
}

void ov01_021F9F6C(MapObjectRender *render, u32 sequenceId) {
    if (ov01_021F9F14(render, sequenceId) == 2) {
        ov01_021F9A44(render, sequenceId);
    }
}

// Loads the model, gfx sequence and texture of the sprite
void ov01_021F9F84(MapObjectRender *render, u32 spriteId) {
    ov01_021F9F54(render, SpriteId_GetBillboardModelId(spriteId));
    ov01_021F9F6C(render, SpriteId_GetBillboardSequenceId(spriteId));
    ov01_021F9F3C(render, spriteId);
}

void MapObjectRender_ReleaseUnused(MapObjectManager *manager, MapObjectRender *render) {
    ov01_021F9BD4(manager, render);
    ov01_021F9CF8(manager, render);
    ov01_021F9E30(manager, render);
}

// TRUE if an active map object other than skip uses the sprite
BOOL ov01_021F9FCC(MapObjectManager *manager, u32 spriteId, LocalMapObject *skip) {
    s32 index = 0;
    LocalMapObject *object = NULL;
    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        do {
            if (skip == NULL || skip != object) {
                u32 objectSpriteId = MapObject_GetSpriteID(object);
                if (objectSpriteId == spriteId) {
                    return TRUE;
                }
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE);
    }
    return FALSE;
}

BOOL ov01_021FA01C(MapObjectManager *manager, u32 modelId, LocalMapObject *skip) {
    s32 index = 0;
    LocalMapObject *object = NULL;
    u32 spriteId;
    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        do {
            if ((skip == NULL || skip != object) && (spriteId = MapObject_GetSpriteID(object)) != 0xFFFF && MapObject_GetEffectFlags(object)->low == 1 && spriteId != 0xFFFF && SpriteId_GetBillboardModelId(spriteId) == modelId) {
                return TRUE;
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE);
    }
    return FALSE;
}

// Note: tests spriteId before it is first read (kept as built)
BOOL ov01_021FA094(MapObjectManager *manager, u32 sequenceId, LocalMapObject *skip) {
    s32 index = 0;
    LocalMapObject *object = NULL;
    u32 spriteId;
    if (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE) {
        do {
            if ((skip == NULL || object != skip) && spriteId != 0xFFFF && MapObject_GetEffectFlags(object)->low == 1 && (spriteId = MapObject_GetSpriteID(object)) != 0xFFFF && SpriteId_GetBillboardSequenceId(spriteId) == sequenceId) {
                return TRUE;
            }
        } while (MapObjectManager_GetNextObjectWithFlagFromIndex(manager, &object, &index, MAPOBJECTFLAG_ACTIVE) == TRUE);
    }
    return FALSE;
}

// Frees the dynamic model, sequence and texture of the sprite unless a map object other than skip uses them
void MapObjectRender_ReleaseSpriteResources(MapObjectManager *manager, u32 spriteId, LocalMapObject *skip) {
    MapObjectRender *render = sub_0205F1A0(manager);
    u32 modelId = SpriteId_GetBillboardModelId(spriteId);
    u32 sequenceId;
    if (ov01_021F9EEC(render, modelId) == 1 && !ov01_021FA01C(manager, modelId, skip)) {
        ResourceBank_Free(render->modelBank, modelId);
        ov01_021F9BAC(render, modelId);
    }
    sequenceId = SpriteId_GetBillboardSequenceId(spriteId);
    if (ov01_021F9F14(render, sequenceId) == 1 && !ov01_021FA094(manager, sequenceId, skip)) {
        ResourceBank_Free(render->sequenceBank, sequenceId);
        ov01_021F9CD0(render, sequenceId);
    }
    if (ov01_021F9EC4(render, spriteId) == 1 && !ov01_021F9FCC(manager, spriteId, skip)) {
        GF3dGfxRawResMan_FreeObjById(ov01_021FA1F4(render), spriteId);
        ov01_021F9E04(render, spriteId);
        ov01_021FA4F0(render, spriteId);
        ov01_021FA6A4(render, spriteId);
    }
}

void ov01_021FA1C8(MapObjectRender *render, void *billboards) {
    render->billboards = billboards;
}

void *MapObjectRender_GetBillboards(MapObjectRender *render) {
    return render->billboards;
}

void ov01_021FA1D8(MapObjectRender *render, u32 count) {
    render->objectCount = count;
}

void *ov01_021FA1DC(MapObjectRender *render) {
    return render->unkE8;
}

void *ov01_021FA1E4(MapObjectRender *render) {
    return render->unkEC;
}

void ov01_021FA1EC(MapObjectRender *render, GF_3DGfxRawResMan *textures) {
    render->textures = textures;
}

GF_3DGfxRawResMan *ov01_021FA1F4(MapObjectRender *render) {
    return render->textures;
}

u32 *ov01_021FA1FC(MapObjectRender *render) {
    return render->textureIds;
}

u32 *ov01_021FA200(MapObjectRender *render) {
    return render->modelIds;
}

u32 *ov01_021FA204(MapObjectRender *render) {
    return render->sequenceIds;
}

void ov01_021FA208(MapObjectRender *render, u32 count) {
    render->residentModelCount = count;
}

u32 ov01_021FA20C(MapObjectRender *render) {
    return render->residentModelCount;
}

void ov01_021FA210(MapObjectRender *render, u32 count) {
    render->dynamicModelCount = count;
}

u32 ov01_021FA214(MapObjectRender *render) {
    return render->dynamicModelCount;
}

void ov01_021FA218(MapObjectRender *render, u32 count) {
    render->residentSequenceCount = count;
}

u32 ov01_021FA21C(MapObjectRender *render) {
    return render->residentSequenceCount;
}

void ov01_021FA220(MapObjectRender *render, u32 count) {
    render->dynamicSequenceCount = count;
}

u32 ov01_021FA224(MapObjectRender *render) {
    return render->dynamicSequenceCount;
}

void ov01_021FA228(MapObjectRender *render, u32 count) {
    render->residentTextureCount = count;
}

u32 ov01_021FA22C(MapObjectRender *render) {
    return render->residentTextureCount;
}

void ov01_021FA230(MapObjectRender *render, u32 count) {
    render->dynamicTextureCount = count;
}

u32 ov01_021FA234(MapObjectRender *render) {
    return render->dynamicTextureCount;
}

void *FldObjSys_ReadMModelFromNarc(MapObjectRender *render, u32 memberIdx, BOOL atStart) {
    return ReadMModelFromNarcInternal(render->manager, memberIdx, atStart);
}

#pragma force_active on
const SpriteBillboardInfo *SpriteId_GetBillboardInfo(u32 spriteId) {
    const SpriteBillboardIndex *entry = ov01_022074A8;
    do {
        if (entry->spriteId == spriteId) {
            u32 index = entry->index;
            if (index == 0xFF) {
                GF_ASSERT(index != 0xFF);
                return (const SpriteBillboardInfo *)ov01_02207318;
            }
            return (const SpriteBillboardInfo *)(ov01_02207318 + index * 8);
        }
        entry++;
    } while (entry->spriteId != 0xFFFF);
    GF_AssertFail();
    return NULL;
}
#pragma force_active reset

u8 SpriteId_GetBillboardModelId(u32 spriteId) {
    return SpriteId_GetBillboardInfo(spriteId)->modelId;
}

u8 ov01_021FA298(u32 spriteId) {
    return SpriteId_GetBillboardModelId(spriteId);
}

u8 SpriteId_GetBillboardSequenceId(u32 spriteId) {
    return SpriteId_GetBillboardInfo(spriteId)->sequenceId;
}

u32 SpriteId_GetBillboardRanges(u32 spriteId) {
    return SpriteId_GetBillboardInfo(spriteId)->ranges;
}

void MapObject_SetBillboardPending(LocalMapObject *object, BOOL set) {
    if (set == TRUE) {
        MapObject_SetFlagsBits(object, MAPOBJECTFLAG_UNK22);
    } else {
        MapObject_ClearFlagsBits(object, MAPOBJECTFLAG_UNK22);
    }
}

BOOL MapObject_IsBillboardPending(LocalMapObject *object) {
    return MapObject_GetFlagsBitsMask(object, MAPOBJECTFLAG_UNK22) != 0;
}

void ov01_021FA2EC(MapObjectRender *render, u32 count) {
    BillboardCreateParams params;
    params.heapId = HEAP_ID_FIELD1;
    params.count = count;
    ov01_021FA1C8(render, Billboard_Create(&params));
    ov01_021FA1D8(render, count);
}

void ov01_021FA314(void *billboards) {
    Billboard_Delete(billboards);
}

#pragma force_active on
void *ov01_021FA31C(void *billboards, MapObjectBillboardResource *resource, VecFx32 *position) {
    MapObjectBillboardCreateParams params;
    void *billboard;
    params.billboards = billboards;
    params.resource = resource;
    params.position = *position;
    params.scale = ov01_02208B64;
    billboard = BillboardObject_Create(&params);
    if (billboard) {
        BillboardObject_SetVisible(billboard, 0);
        BillboardObject_SetSequence(billboard, 0);
        BillboardObject_SetSequenceFrame(billboard, 0);
        ov01_021EA3B0(BillboardObject_GetModel(billboard));
    }
    return billboard;
}

// Fills the cached resource of the sprite from the banks and the texture manager
void ov01_021FA370(MapObjectRender *render, u32 spriteId, MapObjectBillboardResource *resource) {
    GF_3DGfxRawResObj *texture;
    resource->modelFile = ResourceBank_Get(render->modelBank, SpriteId_GetBillboardModelId(spriteId));
    BillboardGfxSequence_Init(ResourceBank_Get(render->sequenceBank, SpriteId_GetBillboardSequenceId(spriteId)), resource->sequence);
    texture = GF3dGfxRawResMan_GetObjById(ov01_021FA1F4(render), spriteId);
    resource->texture = GF3dGfxRawResObj_GetTex(texture);
    resource->texKey = GF3dGfxRawResObj_GetTexKey(texture);
    resource->tex4x4Key = GF3dGfxRawResObj_GetTex4x4Key(texture);
    resource->plttKey = GF3dGfxRawResObj_GetPlttKey(texture);
    resource->ranges = SpriteId_GetBillboardRanges(spriteId);
}
#pragma force_active reset

MapObjectRender *MapObject_GetRender(LocalMapObject *object) {
    return sub_0205F1A0(MapObject_GetManager(object));
}

int MapObject_UpdateBillboardPosition(LocalMapObject *object, Sprite *billboard) {
    VecFx32 position;
    MapObject_GetDrawPosition(object, &position);
    position.z += FX32_CONST(6);
    BillboardObject_SetPosition(billboard, &position);
    return 0;
}

void ov01_021FA40C(LocalMapObject *object, void *billboard) {
    BOOL visible = TRUE;
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_VISIBLE) == TRUE) {
        visible = FALSE;
    }
    if (MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK12) == TRUE && MapObject_TestFlagsBits(object, MAPOBJECTFLAG_UNK13) == FALSE) {
        visible = FALSE;
    }
    BillboardObject_SetVisible(billboard, (u8)visible);
}

u32 Facing_GetBillboardStandSequence(u32 facing) {
    return ov01_02208B70[facing];
}

u32 Facing_GetBillboardWalkSequence(u32 facing) {
    return ov01_02208B80[facing];
}

u32 ov01_021FA464(u32 facing) {
    return ov01_02208B90[facing];
}

#pragma force_active on
// 1: the texture was loaded now; 0: queued for a later frame (the queue is full for this frame, or the
// billboards are busy)
int ov01_021FA470(MapObjectRender *render, u32 spriteId, u32 modelId, u32 dynamic) {
    MapObjectRenderQueue *queue = render->queue;
    if (queue->loadsThisFrame >= queue->loadsPerFrame || sub_020238F8(render->billboards) == TRUE) {
        int i;
        TextureLoadRequest *request = queue->loads;
        for (i = 0; i < queue->count; i++, request++) {
            if (!request->textures) {
                request->textures = render->textures;
                request->dynamic = dynamic;
                request->spriteId = spriteId;
                request->modelId = modelId;
                return 0;
            }
        }
        GF_AssertFail();
        return 0;
    } else {
        void *textureFile = FldObjSys_ReadMModelFromNarc(render, modelId, FALSE);
        ov01_021FA61C(render, spriteId, textureFile, dynamic);
        queue->loadsThisFrame++;
        return 1;
    }
}
#pragma force_active reset

void ov01_021FA4F0(MapObjectRender *render, u32 spriteId) {
    MapObjectRenderQueue *queue = render->queue;
    s16 count = queue->count;
    TextureLoadRequest *request = queue->loads;
    int i;
    for (i = 0; i < count; i++, request++) {
        if (request->spriteId == spriteId && request->textures != NULL) {
            request->textures = NULL;
            return;
        }
    }
}

// 3: a resident texture of the sprite is queued, 4: a dynamic one, 0: none
int ov01_021FA524(MapObjectRender *render, u32 spriteId) {
    MapObjectRenderQueue *queue = render->queue;
    s16 count = queue->count;
    TextureLoadRequest *request = queue->loads;
    int i;
    for (i = 0; i < count; i++, request++) {
        if (request->spriteId == spriteId && request->textures != NULL) {
            if (request->dynamic == 0) {
                return 3;
            }
            return 4;
        }
    }
    return 0;
}

#pragma force_active on
// Main task: loads up to loadsPerFrame queued textures, then compacts the queue
void ov01_021FA564(SysTask *task, void *data) {
    MapObjectRender *render = data;
    MapObjectRenderQueue *queue = render->queue;
    int i = 0;
    int j;
    TextureLoadRequest *request = queue->loads;
    TextureLoadRequest *requests;
    if (sub_020238F8(render->billboards) == TRUE) {
        return;
    }
    while (queue->loadsThisFrame < queue->loadsPerFrame && i < queue->count) {
        if (request->textures) {
            void *textureFile = FldObjSys_ReadMModelFromNarc(render, request->modelId, FALSE);
            ov01_021FA61C(render, request->spriteId, textureFile, request->dynamic);
            request->textures = NULL;
            queue->loadsThisFrame++;
        }
        request++;
        i++;
    }
    requests = queue->loads;
    for (i = 0; i < queue->count - 1; i++) {
        if (requests[i].textures == NULL) {
            for (j = i + 1; j < queue->count; j++) {
                if (requests[j].textures) {
                    requests[i] = requests[j];
                    requests[j].textures = NULL;
                    break;
                }
            }
        }
    }
    queue->loadsThisFrame = 0;
}
#pragma force_active reset

void ov01_021FA61C(MapObjectRender *render, u32 spriteId, void *textureFile, u32 dynamic) {
    if (GF3dGfxRawResMan_AllocObjAndKeys(render->textures, textureFile, spriteId, TRUE, HEAP_ID_FIELD1) == NULL) {
        GF_AssertFail();
    }
    ov01_021FA668(render, spriteId);
    if (dynamic == FALSE) {
        ov01_021F9D5C(render, spriteId);
    } else {
        ov01_021F9DD0(render, spriteId);
    }
}

#pragma force_active on
void ov01_021FA668(MapObjectRender *render, u32 spriteId) {
    int i = 0;
    MapObjectRenderQueue *queue = render->queue;
    TextureTransfer *transfer = queue->transfers;
    queue->busy = TRUE;
    for (; i < queue->count; i++) {
        if (transfer->textures == NULL) {
            transfer->textures = render->textures;
            transfer->spriteId = spriteId;
            transfer->loaded = FALSE;
            queue->busy = FALSE;
            return;
        }
        transfer++;
    }
    GF_AssertFail();
}

void ov01_021FA6A4(MapObjectRender *render, u32 spriteId) {
    MapObjectRenderQueue *queue = render->queue;
    int i;
    TextureTransfer *transfer = queue->transfers;
    int count;
    queue->busy = TRUE;
    count = queue->count;
    for (i = 0; i < count; i++, transfer++) {
        if (transfer->textures != NULL && transfer->spriteId == spriteId) {
            transfer->textures = NULL;
            transfer->loaded = FALSE;
            break;
        }
    }
    queue->busy = FALSE;
}
#pragma force_active reset

// VBlank task: transfers the queued textures to VRAM
void ov01_021FA6E0(SysTask *task, void *data) {
    int i;
    MapObjectRenderQueue *queue = ((MapObjectRender *)data)->queue;
    TextureTransfer *transfer;

    if (queue->busy != TRUE) {
        transfer = queue->transfers;
        for (i = 0; i < queue->count; i++) {
            if (transfer->loaded == FALSE && transfer->textures != NULL) {
                GF3dGfxRawResMan_LoadObjTexById(transfer->textures, transfer->spriteId);
                transfer->loaded = TRUE;
            }
            transfer++;
        }
    }
}

// VWait task: releases the transferred textures' VRAM headers
void ov01_021FA71C(SysTask *task, void *data) {
    int i;
    MapObjectRenderQueue *queue = ((MapObjectRender *)data)->queue;
    TextureTransfer *transfer;

    i = 0;
    transfer = queue->transfers;
    queue->busy = TRUE;
    if (queue->count > 0) {
        do {
            if (transfer->loaded == TRUE && transfer->textures != NULL) {
                GF3dGfxRawResMan_FreeObjVramAndSecondaryHeaderById(transfer->textures, transfer->spriteId);
                transfer->loaded = FALSE;
                transfer->textures = NULL;
            }
            i++;
            transfer++;
        } while (i < queue->count);
    }
    queue->busy = FALSE;
}

#pragma force_active on
#pragma opt_lifetimes off
// Queues the creation of the object's billboard until its texture is in VRAM
void ov01_021FA75C(MapObjectRender *render, LocalMapObject *object, void **billboard, u32 spriteId) {
    int i = 0;
    MapObjectRenderQueue *queue = render->queue;
    s16 count = queue->count;
    PendingBillboard *pending = queue->pending;
    for (; i < count; i++, pending++) {
        if (pending->object == NULL) {
            pending->object = object;
            pending->billboard = billboard;
            pending->spriteId = spriteId;
            *pending->billboard = NULL;
            MapObject_SetBillboardPending(object, TRUE);
            return;
        }
    }
    GF_AssertFail();
}
#pragma opt_lifetimes reset
#pragma force_active reset

void ov01_021FA798(MapObjectRender *render, LocalMapObject *object) {
    MapObjectRenderQueue *queue = render->queue;
    s16 count = queue->count;
    PendingBillboard *pending = queue->pending;
    int i;
    for (i = 0; i < count; i++, pending++) {
        if (pending->object == object) {
            pending->object = NULL;
            pending->billboard = NULL;
            return;
        }
    }
}

void ov01_021FA7C8(MapObjectRender *render) {
    MapObjectRenderQueue *queue = render->queue;
    int i;
    PendingBillboard *pending = queue->pending;
    for (i = 0; i < queue->count; i++) {
        if (pending->object != NULL) {
            ov01_021FA804(render, pending);
        }
        pending++;
    }
}

// Main task: creates the pending billboards
void ov01_021FA7F8(SysTask *task, void *data) {
    ov01_021FA7C8(data);
}

void ov01_021FA804(MapObjectRender *render, PendingBillboard *pending) {
    if (ov01_021F9EC4(render, pending->spriteId) == 2) {
        return;
    }
    *pending->billboard = ov01_021F9528(pending->object, pending->spriteId);
    if (*pending->billboard == NULL) {
        GF_AssertFail();
    }
    MapObject_SetBillboardPending(pending->object, FALSE);
    if (MapObject_TestFlagsBits(pending->object, MAPOBJECTFLAG_UNK21) == TRUE) {
        sub_0205F4C0(pending->object);
    } else {
        sub_0205F484(pending->object);
    }
    pending->object = NULL;
    pending->billboard = NULL;
}

#pragma force_active on
void ov01_021FA854(MapObjectRender *render, u32 count, u32 loadsPerFrame) {
    u32 size, loadsSize, pendingSize, transfersSize, priority;
    MapObjectRenderQueue *queue;
    pendingSize = count * sizeof(PendingBillboard);
    loadsSize = count * sizeof(TextureLoadRequest);
    transfersSize = pendingSize * 2;
    size = transfersSize + loadsSize + sizeof(MapObjectRenderQueue);
    queue = Heap_Alloc(HEAP_ID_FIELD1, size);
    if (!queue) {
        GF_AssertFail();
    }
    memset(queue, 0, size);
    queue->count = count;
    queue->loadsPerFrame = loadsPerFrame;
    queue->pending = (PendingBillboard *)((u8 *)queue + sizeof(MapObjectRenderQueue));
    queue->loads = (TextureLoadRequest *)((u8 *)queue->pending + pendingSize);
    queue->transfers = (TextureTransfer *)((u8 *)queue->loads + loadsSize);
    render->queue = queue;
    priority = MapObjectManager_GetPriority(render->manager);
    queue->loadTask = SysTask_CreateOnMainQueue(ov01_021FA564, render, priority + 1);
    queue->createTask = SysTask_CreateOnMainQueue(ov01_021FA7F8, render, priority + 2);
    queue->transferTask = SysTask_CreateOnVBlankQueue(ov01_021FA6E0, render, 255);
    queue->releaseTask = SysTask_CreateOnVWaitQueue(ov01_021FA71C, render, 255);
}
#pragma force_active reset

void ov01_021FA8F8(MapObjectRender *render) {
    MapObjectRenderQueue *queue = render->queue;
    queue->busy = TRUE;
    SysTask_Destroy(queue->loadTask);
    SysTask_Destroy(queue->createTask);
    SysTask_Destroy(queue->transferTask);
    SysTask_Destroy(queue->releaseTask);
    Heap_Free(queue);
    render->queue = NULL;
}

// Rebuilds the billboard for the new sprite; when the object already shows one, a copy of it stays on screen
// (ov01_021FA97C) until the new one is created
void MapObject_ChangeBillboardSprite(LocalMapObject *object, u32 spriteId) {
    if (MapObject_IsBillboardPending(object) == TRUE || MapObject_GetBillboard(object) == FALSE) {
        u32 oldSpriteId = MapObject_GetSpriteID(object);
        MapObjectManager *manager = MapObject_GetManager(object);
        sub_0205E420(object);
        MapObjectRender_ReleaseSpriteResources(manager, oldSpriteId, object);
        sub_0205E38C(object, spriteId);
    } else {
        ov01_021FA97C(object, spriteId);
    }
}

#pragma force_active on
void ov01_021FA97C(LocalMapObject *object, u32 spriteId) {
    int fog;
    u32 oldSpriteId;
    void *billboards;
    void *oldBillboard;
    int priority;
    VecFx32 position;
    VecFx32 reflectionPosition;
    SysTask *task;
    int id;
    int reflectionKind;
    const BillboardModelEntry *entry;
    NNSG3dResTex *texture;
    u8 behavior;
    int modelNo;
    SpriteChangeWork *work;
    MapObjectRender *render;

    render = MapObject_GetRender(object);
    billboards = MapObjectRender_GetBillboards(render);
    oldBillboard = MapObject_GetBillboard(object);
    work = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(SpriteChangeWork));
    memset(work, 0, sizeof(SpriteChangeWork));
    work->spriteId = spriteId;
    work->object = object;
    switch (MapObject_GetID(object)) {
    case 0xFF:
        fog = NNS_G3dMdlGetMdlFogEnableFlag(BillboardObject_GetModel(MapObject_GetBillboard(object)), 0);
        break;
    default:
        fog = 0;
        break;
    }
    oldSpriteId = MapObject_GetSpriteID(object);
    id = SpriteId_GetBillboardModelId(oldSpriteId);
    entry = ov01_02207294;
    do {
        if (entry->id == id) {
            break;
        }
        entry++;
    } while (entry->id != 0xFF);
    GF_ASSERT(entry->id != 0xFF);
    work->modelFile = FldObjSys_ReadMModelFromNarc(render, entry->member, FALSE);
    id = SpriteId_GetBillboardSequenceId(oldSpriteId);
    entry = ov01_022072CC;
    do {
        if (entry->id == id) {
            break;
        }
        entry++;
    } while (entry->id != 0xFF);
    GF_ASSERT(entry->id != 0xFF);
    work->sequenceFile = FldObjSys_ReadMModelFromNarc(render, entry->member, FALSE);
    BillboardGfxSequence_Init(work->sequenceFile, work->sequence);
    modelNo = GetMoveModelNoBySpriteId(oldSpriteId);
    if (modelNo < 0) {
        GF_AssertFail();
        modelNo = 0;
    }
    work->textureFile = FldObjSys_ReadMModelFromNarc(render, modelNo, FALSE);
    texture = NNS_G3dGetTex(work->textureFile);
    BillboardResource_Init(&work->resource, work->modelFile, texture, SpriteId_GetBillboardRanges(oldSpriteId), work->sequence);
    position = *BillboardObject_GetPosition(oldBillboard);
    work->billboard = ov01_021FA31C(billboards, &work->resource, &position);
    GF_ASSERT(work->billboard != NULL);
    if (fog == 0) {
        NNS_G3dMdlSetMdlFogEnableFlagAll(BillboardObject_GetModel(work->billboard), 0);
    }
    BillboardObject_SetSequence(work->billboard, BillboardObject_GetSequence(oldBillboard));
    BillboardObject_SetSequenceFrame(work->billboard, BillboardObject_GetSequenceFrame(oldBillboard));
    BillboardObject_SetFrame(work->billboard, BillboardObject_GetFrame(oldBillboard));
    BillboardObject_AdvanceFrame(work->billboard, 0);
    BillboardObject_SetVisible(work->billboard, 1);
    BillboardObject_UpdateTextureAnimation(work->billboard);
    if (MapObject_CheckFlag24(object) == TRUE) {
        priority = MapObject_GetPriorityPlusValue(object, 2);
        behavior = MapObject_GetMetatileBehaviorInDirection(object, 1);
        if (MetatileBehavior_IsReflective(behavior) == TRUE) {
            reflectionKind = 2;
        } else if (MetatileBehavior_IsPuddle(behavior) == TRUE) {
            reflectionKind = 0;
        } else {
            reflectionKind = 1;
        }
        MapObject_CopyPositionVector(object, &reflectionPosition);
        work->reflection = Reflection_SpawnForBillboard(FieldEffectManager_GetFromMapObject(object), &work->resource, work->billboard, &reflectionPosition, reflectionKind, priority);
    }
    sub_0205E420(object);
    MapObjectRender_ReleaseSpriteResources(render->manager, oldSpriteId, object);
    sub_0205E38C(object, spriteId);
    MapObjectManager_GetPriority(render->manager);
    task = SysTask_CreateOnVWaitQueue((SysTaskFunc)ov01_021FAB9C, work, 0xFF);
    GF_ASSERT(task != NULL);
    ov01_021FAB9C(task, work);
}
#pragma force_active reset

// Deletes the old billboard's copy once the object's new billboard exists (or the object is gone)
void ov01_021FAB9C(SysTask *task, SpriteChangeWork *work) {
    LocalMapObject *object = work->object;
    switch (work->state) {
    case 0:
        if (!MapObject_CheckActive(object) || !MapObject_IsBillboardPending(object)) {
            if (work->reflection != NULL) {
                FieldEffect_Destroy((int)work->reflection);
            }
            BillboardObject_Delete(work->billboard);
            Heap_Free(work->modelFile);
            Heap_Free(work->sequenceFile);
            Heap_Free(work->textureFile);
            Heap_Free(work);
            SysTask_Destroy(task);
            return;
        }
        if (work->spriteId != MapObject_GetSpriteID(object)) {
            work->state++;
        }
        break;
    case 1:
        if (!MapObject_IsBillboardPending(object)) {
            if (work->reflection != NULL) {
                FieldEffect_Destroy((int)work->reflection);
            }
            BillboardObject_Delete(work->billboard);
            Heap_Free(work->modelFile);
            Heap_Free(work->sequenceFile);
            Heap_Free(work->textureFile);
            Heap_Free(work);
            SysTask_Destroy(task);
        }
        break;
    }
}
