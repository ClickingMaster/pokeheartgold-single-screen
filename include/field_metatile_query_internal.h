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

int Terrain_AbsDiff(int a0, int a1);
int FieldSystem_GetHeightStep(FieldSystem *fieldSystem, VecFx32 *playerPos, int x, int z, u8 *outKind);

extern int TileIndex_GetQuadrant(int idx, int stride);
extern int MapLoadManager_GetMapSlotOfMatrixIndex(int chunkIdx, int a1, MapLoadManager *mapLoadManager);
extern void *MapLoadManager_GetBdhc(MapLoadManager *mapLoadManager, u8 idx);
int Terrain_CalcHeight(FieldSystem *fieldSystem, int a1, int a2, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Bdhc(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
int Terrain_CalcHeightWithMode(FieldSystem *fieldSystem, int a1, int a2, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Simple(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
BOOL Terrain_GetAttributes_Loaded(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
BOOL Terrain_GetAttributes_Simple(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
u8 sub_020548EC(FieldSystem *fieldSystem, int x, int z);
int FieldSystem_GetTerrainHeight(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);

extern BOOL MapLoadManager_GetMapSlotAt(MapLoadManager *mapLoadManager, int x, int z, u8 *outIdx);
extern SoundplateStruct *MapLoadManager_GetSoundplates(MapLoadManager *mapLoadManager, u8 idx);
extern int MapLoadManager_GetMapMatrixIndex(MapLoadManager *mapLoadManager, u8 idx);
u32 FieldSystem_CheckCollisionHeightAndGimmick(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *a4);
void Terrain_MakeTileRect(int a0, int a1, int a2, int a3, int a4, int a5, fx32 *out);
BOOL MapProp_IsInRect(MapProp *mapProp, fx32 *rect, VecFx32 *offset);
BOOL FieldSystem_FindMapPropInRect(FieldSystem *fieldSystem, int buildModel, fx32 *rect, MapProp **outProp);
BOOL FieldSystem_FindMapPropOfModelsInRect(FieldSystem *fieldSystem, int *models, u32 count, fx32 *rect, MapProp **outProp, int *outModel);
BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, MapProp **outProp, int *outIdx);
BOOL FieldSystem_FindMapPropOfModels(FieldSystem *fieldSystem, int *models, u32 count, MapProp **outProp, int *outModel);
int *FieldSystem_CollectMapPropModelsInRect(FieldSystem *fieldSystem, u32 heapId, int count, fx32 *rect, int fill);
void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos);
BOOL sub_02054E00(u16 a0);
BOOL sub_02054E20(u16 a0);

extern const u16 _020FC5FC[];
extern const FieldAttrVTable gTerrainQueries_Bdhc;
extern const u16 _020FC60C[];
extern const FieldAttrVTable gTerrainQueries_Simple;

SoundplateStruct *FieldSystem_GetSoundplateAt(FieldSystem *fieldSystem, int x, int z);
void FieldSystem_SetTerrainQueries(void **a0, int a1);
BOOL FieldSystem_CheckTileCollision(FieldSystem *fieldSystem, int x, int z);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
BOOL FieldSystem_CheckCollisionAndHeight(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int a4);

#endif
