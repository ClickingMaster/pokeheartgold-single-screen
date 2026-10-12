#ifndef POKEHEARTGOLD_FIELD_MAP_LOAD_BUFFERS_H
#define POKEHEARTGOLD_FIELD_MAP_LOAD_BUFFERS_H

#include "global.h"

#define MAP_LOAD_BUFFER_COUNT          4
#define MAP_LOAD_PRIMARY_BUFFER_SIZE   0xF000
#define MAP_LOAD_SECONDARY_BUFFER_SIZE 0x9000

// The map chunk loader's working buffers, one pair per loaded map: a static primary buffer and an optional
// heap-allocated secondary one.
typedef struct MapLoadBuffers {
    void *primary[MAP_LOAD_BUFFER_COUNT];
    void *secondary[MAP_LOAD_BUFFER_COUNT];
} MapLoadBuffers;

MapLoadBuffers *MapLoadBuffers_New(BOOL withSecondary);
void MapLoadBuffers_Free(MapLoadBuffers *buffers);
void MapLoadBuffers_GetPrimary(u32 index, MapLoadBuffers *buffers, void **out);
void MapLoadBuffers_GetSecondary(u32 index, MapLoadBuffers *buffers, void **out);
void MapLoadBuffers_Clear(int index, u32 primarySize, u32 secondarySize, MapLoadBuffers *buffers);

#endif // POKEHEARTGOLD_FIELD_MAP_LOAD_BUFFERS_H
