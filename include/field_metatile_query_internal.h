#ifndef POKEHEARTGOLD_FIELD_METATILE_QUERY_INTERNAL_H
#define POKEHEARTGOLD_FIELD_METATILE_QUERY_INTERNAL_H
#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_prop.h"

#include "field_system.h"
#include "heap.h"
#include "map_matrix.h"
#include "terrain_attributes.h"
#include "field_terrain.h"
#include "gymmick_collision.h"

// Inlined from the external worker's private.h. The original assembly remains
// the owner of the four tables below, so this snapshot only declares them.
typedef struct FieldAttrVTable {
    int (*getHeight)(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
    BOOL (*getAttr)(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
} FieldAttrVTable;

#define FS_VT(fieldSystem) ((const FieldAttrVTable *)(fieldSystem)->terrainQueries)

extern int TileIndex_GetQuadrant(int idx, int stride);
u8 MapLoadManager_GetMapSlotOfMatrixIndex(int chunkIdx, int unused, MapLoadManager *mapLoadManager);
extern void *MapLoadManager_GetBdhc(MapLoadManager *mapLoadManager, u8 idx);

extern BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *mapLoadManager, int x, int z, u8 *outIdx);
extern SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *mapLoadManager, u8 idx);
extern int MapLoadManager_GetMapMatrixIndex(MapLoadManager *mapLoadManager, u8 idx);

#endif
