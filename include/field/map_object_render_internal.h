#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_INTERNAL_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_INTERNAL_H

#include "global.h"

#include "field/map_object_render.h"
#include "field/resource_bank.h"

#include "billboard_internal.h"
#include "gf_3d_loader.h"
#include "map_object.h"

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
    Billboard *billboards;
    BillboardResource *resources;
    void *unk_E8;
    void *unk_EC;
    GF_3DGfxRawResMan *textures;
    struct BillboardResourceSlot *resourceSlots;
    ResourceBank *modelBank;
    ResourceBank *sequenceBank;
    struct MapObjectRenderQueue *queue;
    MapObjectManager *manager;
};

BOOL MapObjectRender_GetBillboardResource(MapObjectManager *manager, u32 spriteId, BillboardResource *dest);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_RENDER_INTERNAL_H
