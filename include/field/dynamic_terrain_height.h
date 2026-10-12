#ifndef POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H
#define POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H

#include "global.h"

#include "constants/heap.h"

// Rectangles of map tiles whose terrain height is set by script code instead of the map's height data
// (e.g. the Violet City gym elevator). They take precedence over the map's own heights.
typedef struct DynamicTerrainHeightPlate {
    s32 x; // in tiles
    s32 z;
    s32 width;
    s32 depth;
    fx32 height;
    BOOL active;
} DynamicTerrainHeightPlate;

typedef struct DynamicTerrainHeightManager {
    s32 count;
    DynamicTerrainHeightPlate *plates;
} DynamicTerrainHeightManager;

BOOL DynamicTerrainHeightPlate_Contains(s32 x, s32 z, const DynamicTerrainHeightPlate *plate);
DynamicTerrainHeightManager *DynamicTerrainHeightManager_New(u8 platesCount, enum HeapID heapID);
void DynamicTerrainHeightManager_SetPlate(u32 index, s32 x, s32 z, s32 width, s32 depth, fx32 height, DynamicTerrainHeightManager *manager);
void DynamicTerrainHeightManager_Free(DynamicTerrainHeightManager *manager);
BOOL DynamicTerrainHeightManager_FindPlate(s32 x, s32 z, DynamicTerrainHeightManager *manager, u8 *index);
fx32 DynamicTerrainHeightManager_GetPlateHeight(s32 index, DynamicTerrainHeightManager *manager);
void DynamicTerrainHeightManager_SetPlateHeight(s32 index, fx32 height, DynamicTerrainHeightManager *manager);

#endif // POKEHEARTGOLD_FIELD_DYNAMIC_TERRAIN_HEIGHT_H
