#include "field/dynamic_terrain_height.h"

#include "global.h"

#include "assert.h"
#include "heap.h"

BOOL DynamicTerrainHeightPlate_Contains(s32 x, s32 z, const DynamicTerrainHeightPlate *plate) {
    s32 minX;
    s32 maxX;
    s32 minZ;
    s32 maxZ;

    if (plate->active == FALSE) {
        return FALSE;
    }
    minX = plate->x;
    maxX = plate->width;
    minZ = plate->z;
    maxZ = plate->depth;
    maxX = minX + maxX - 1;
    maxZ = minZ + maxZ - 1;
    if (minX <= x && x <= maxX && minZ <= z && z <= maxZ) {
        return TRUE;
    }
    return FALSE;
}

DynamicTerrainHeightManager *DynamicTerrainHeightManager_New(u8 platesCount, enum HeapID heapID) {
    DynamicTerrainHeightManager *manager = Heap_Alloc(heapID, sizeof(DynamicTerrainHeightManager));
    u8 i;

    manager->plates = Heap_Alloc(heapID, platesCount * sizeof(DynamicTerrainHeightPlate));
    manager->count = platesCount;
    for (i = 0; i < platesCount; i++) {
        manager->plates[i].active = FALSE;
    }
    return manager;
}

void DynamicTerrainHeightManager_SetPlate(u32 index, s32 x, s32 z, s32 width, s32 depth, fx32 height, DynamicTerrainHeightManager *manager) {
    manager->plates[index].x = x;
    manager->plates[index].z = z;
    manager->plates[index].width = width;
    manager->plates[index].depth = depth;
    manager->plates[index].height = height;
    manager->plates[index].active = TRUE;
}

void DynamicTerrainHeightManager_Free(DynamicTerrainHeightManager *manager) {
    Heap_Free(manager->plates);
    Heap_Free(manager);
}

// Tile (x, z) -> the first active plate covering it.
BOOL DynamicTerrainHeightManager_FindPlate(s32 x, s32 z, DynamicTerrainHeightManager *manager, u8 *index) {
    u8 i;

    GF_ASSERT(index != NULL);
    for (i = 0; i < manager->count; i++) {
        if (DynamicTerrainHeightPlate_Contains(x, z, &manager->plates[i])) {
            *index = i;
            return TRUE;
        }
    }
    return FALSE;
}

fx32 DynamicTerrainHeightManager_GetPlateHeight(s32 index, DynamicTerrainHeightManager *manager) {
    GF_ASSERT(index < manager->count);
    GF_ASSERT(manager->plates[index].active != FALSE);
    return manager->plates[index].height;
}

void DynamicTerrainHeightManager_SetPlateHeight(s32 index, fx32 height, DynamicTerrainHeightManager *manager) {
    GF_ASSERT(index < manager->count);
    manager->plates[index].height = height;
}
