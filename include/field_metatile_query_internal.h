#ifndef POKEHEARTGOLD_FIELD_METATILE_QUERY_INTERNAL_H
#define POKEHEARTGOLD_FIELD_METATILE_QUERY_INTERNAL_H
#include "global.h"

#include "field/map_load_manager.h"
#include "field/map_load_manager_internal.h"
#include "field/map_prop.h"

#include "field_system.h"
#include "heap.h"
#include "map_matrix.h"
#include "terrain_attributes.h"
#include "field_terrain.h"
#include "gymmick_collision.h"

// The terrain query functions of the current map kind (FieldSystem.terrainQueries).
typedef struct FieldAttrVTable {
    int (*getHeight)(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
    BOOL (*getAttr)(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
} FieldAttrVTable;

#define FS_VT(fieldSystem) ((const FieldAttrVTable *)(fieldSystem)->terrainQueries)

#endif
