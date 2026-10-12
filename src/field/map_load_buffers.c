#include "global.h"

#include "constants/heap.h"

#include "field/map_load_buffers.h"

#include "heap.h"

u8 gMapLoadPrimaryBuffers[MAP_LOAD_BUFFER_COUNT][MAP_LOAD_PRIMARY_BUFFER_SIZE];

MapLoadBuffers *MapLoadBuffers_New(BOOL withSecondary) {
    u8 i;
    MapLoadBuffers *buffers = Heap_Alloc(HEAP_ID_FIELD1, sizeof(MapLoadBuffers));
    u32 secondarySize;
    i = 0;
    do {
        buffers->primary[i] = gMapLoadPrimaryBuffers[i];
        if (buffers->primary[i] == NULL) {
            return NULL;
        }
        if (withSecondary) {
            buffers->secondary[i] = Heap_Alloc(HEAP_ID_FIELD1, MAP_LOAD_SECONDARY_BUFFER_SIZE);
            if (buffers->secondary[i] == NULL) {
                return NULL;
            }
            secondarySize = MAP_LOAD_SECONDARY_BUFFER_SIZE;
        } else {
            buffers->secondary[i] = NULL;
            secondarySize = 0;
        }
        MapLoadBuffers_Clear(i, MAP_LOAD_PRIMARY_BUFFER_SIZE, secondarySize, buffers);
        i++;
    } while (i < MAP_LOAD_BUFFER_COUNT);
    return buffers;
}

void MapLoadBuffers_Free(MapLoadBuffers *buffers) {
    u8 i;
    for (i = 0; i < MAP_LOAD_BUFFER_COUNT; i++) {
        buffers->primary[i] = NULL;
        if (buffers->secondary[i] != NULL) {
            Heap_Free(buffers->secondary[i]);
            buffers->secondary[i] = NULL;
        }
    }
    Heap_Free(buffers);
}

void MapLoadBuffers_GetPrimary(u32 index, MapLoadBuffers *buffers, void **out) {
    *out = buffers->primary[index];
}

void MapLoadBuffers_GetSecondary(u32 index, MapLoadBuffers *buffers, void **out) {
    *out = buffers->secondary[index];
}

void MapLoadBuffers_Clear(int index, u32 primarySize, u32 secondarySize, MapLoadBuffers *buffers) {
    MI_CpuFill8(buffers->primary[index], 0, primarySize);
    MI_CpuFill8(buffers->secondary[index], 0, secondarySize);
}
