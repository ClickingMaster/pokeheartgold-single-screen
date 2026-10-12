#ifndef POKEHEARTGOLD_FIELD_TERRAIN_QUERIES_INTERNAL_H
#define POKEHEARTGOLD_FIELD_TERRAIN_QUERIES_INTERNAL_H
#include "global.h"

#include "field_system.h"
#include "field_terrain.h"

// FieldSystem::unk60 is `void *` in the tracked header. Callers here treat it as a
// pointer to a 2-entry dispatch table (offsets 0x00 and 0x04).
typedef struct FieldAttrVTable {
    int (*getHeight)(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
    BOOL (*getAttr)(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
} FieldAttrVTable;

#define FS_VT(fieldSystem) ((const FieldAttrVTable *)(fieldSystem)->unk60)

#include "field/map_load_manager.h"

#include "map_matrix.h"
#include "terrain_attributes.h"

int Terrain_AbsDiff(int a0, int a1);
int FieldSystem_GetHeightStep(FieldSystem *fieldSystem, VecFx32 *playerPos, int x, int z, u8 *outKind);

extern int ov01_021F6328(int idx, int stride);
extern int ov01_021F635C(int chunkIdx, int a1, MapLoadManager *mapLoadManager);
extern void *ov01_021F65D0(MapLoadManager *mapLoadManager, u8 idx);
extern int Bdhc_GetHeight(int a0, int a1, fx32 x, fx32 z, void *a4, fx32 *outY);
int Terrain_CalcHeight(FieldSystem *fieldSystem, int a1, int a2, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Bdhc(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
int Terrain_CalcHeightWithMode(FieldSystem *fieldSystem, int a1, int a2, fx32 x, fx32 z, u8 *outKind);
int Terrain_GetHeight_Simple(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);
BOOL Terrain_GetAttributes_Loaded(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
BOOL Terrain_GetAttributes_Simple(FieldSystem *fieldSystem, int x, int z, u16 *outAttr);
u8 sub_020548EC(FieldSystem *fieldSystem, int x, int z);
int FieldSystem_GetTerrainHeight(FieldSystem *fieldSystem, int a1, fx32 x, fx32 z, u8 *outKind);

extern BOOL ov01_021F654C(MapLoadManager *mapLoadManager, int x, int z, u8 *outIdx);
extern SoundplateStruct *ov01_021F6600(MapLoadManager *mapLoadManager, u8 idx);

extern const FieldAttrVTable gTerrainQueries_Bdhc;
extern const FieldAttrVTable gTerrainQueries_Simple;

SoundplateStruct *FieldSystem_GetSoundplateAt(FieldSystem *fieldSystem, int x, int z);
void FieldSystem_SetTerrainQueries(void **a0, int a1);
BOOL FieldSystem_CheckTileCollision(FieldSystem *fieldSystem, int x, int z);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
BOOL FieldSystem_CheckCollisionAndHeight(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int a4);

#endif
