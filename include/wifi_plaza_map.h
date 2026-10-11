#ifndef POKEHEARTGOLD_WIFI_PLAZA_MAP_H
#define POKEHEARTGOLD_WIFI_PLAZA_MAP_H

#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "model_animation.h"
#include "overlay_42.h"

// The Wi-Fi Plaza room's 35 x 42 cell grid (OVY_42 grid, cells from NARC a/2/0/0 member 0).
// Each cell holds a collision bit (bit 0) and a 15-bit tile attribute plus a tag above it.
typedef struct WiFiPlazaMap {
    UnkStruct_ov42_02227F68 *grid;
} WiFiPlazaMap;

void WiFiPlazaMap_PosToVecFx32(const s16 *pos, s32 *vec);
void WiFiPlazaMap_VecFx32ToPos(const s32 *vec, s16 *pos);
void WiFiPlazaMap_LoadModelFile(NNSG3dResFileHeader **dest, NARC *narc, int fileId, enum HeapID heapID);
BOOL WiFiPlazaMap_IsModelInstanceInView(ModelResource *model, ModelInstance *instance);
WiFiPlazaMap *WiFiPlazaMap_Create(enum HeapID heapID);
void WiFiPlazaMap_Delete(WiFiPlazaMap *map);
u32 WiFiPlazaMap_GetWidth(void);
s32 WiFiPlazaMap_GetCollision(WiFiPlazaMap *map, u16 x, u16 y);
u32 WiFiPlazaMap_GetTileTag(WiFiPlazaMap *map, u16 x, u16 y);
u32 WiFiPlazaMap_GetTileAttr(WiFiPlazaMap *map, u16 x, u16 y);
BOOL WiFiPlazaMap_FindTileTag(WiFiPlazaMap *map, u16 tag, u16 *outX, u16 *outY, u32 ordinal);
BOOL ov49_02258A30(u8 attr);
BOOL ov49_02258A50(u8 attr);
BOOL ov49_02258A70(u8 attr);
BOOL ov49_02258A90(u8 attr);
UnkStruct_ov42_02227F68 *WiFiPlazaMap_GetGrid(WiFiPlazaMap *map);

#endif // POKEHEARTGOLD_WIFI_PLAZA_MAP_H
