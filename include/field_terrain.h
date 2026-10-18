#ifndef POKEHEARTGOLD_FIELD_TERRAIN_H
#define POKEHEARTGOLD_FIELD_TERRAIN_H

#include "field/field_control.h"

#include "script.h"

struct MapProp;

SoundplateStruct *FieldSystem_GetSoundplateAt(FieldSystem *fieldSystem, int x, int z);
void FieldSystem_SetTerrainQueries(void **queries, int mode);
BOOL FieldSystem_CheckTileCollision(FieldSystem *fieldSystem, int x, int z);
u8 FieldSystem_GetTileAttrBits8To14(FieldSystem *, int, int);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
int FieldSystem_GetTerrainHeight(FieldSystem *, int, fx32, fx32, u8 *);
BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, struct MapProp **outProp, int *outIdx);
BOOL FieldSystem_FindMapPropOfModels(FieldSystem *fieldSystem, int *models, u32 count, struct MapProp **outProp, int *outModel);
int *FieldSystem_CollectMapPropModelsInRect(FieldSystem *, u32, int, fx32 *, u32);
void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos);
BOOL MapModelNo_HasNoAreaMapAnimation(u16);
BOOL FieldSystem_CheckCollisionAndHeight(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int outSlope);
u32 FieldSystem_CheckCollisionHeightAndGimmick(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *outSlope);
void Terrain_MakeTileRect(int, int, int, int, int, int, fx32 *);
BOOL FieldSystem_FindMapPropInRect(FieldSystem *, int, fx32 *, struct MapProp **);
BOOL FieldSystem_FindMapPropOfModelsInRect(FieldSystem *, int *, u32, fx32 *, struct MapProp **, int *);
BOOL MapObject_IsTileOccupiedInLink(LocalMapObject *playerObj, int xInFront, int playerElev, int yInFront);
void FollowPlayerMovement_Reset(LocalMapObject *object);
int Terrain_GetHeight_Bdhc(FieldSystem *, int, fx32, fx32, u8 *);
int Terrain_CalcHeightWithMode(FieldSystem *, int, int, fx32, fx32, u8 *);

#endif // POKEHEARTGOLD_FIELD_TERRAIN_H
