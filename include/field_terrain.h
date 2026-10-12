#ifndef POKEHEARTGOLD_FIELD_TERRAIN_H
#define POKEHEARTGOLD_FIELD_TERRAIN_H

#include "field/field_control.h"

#include "script.h"

struct MapProp;

SoundplateStruct *FieldSystem_GetSoundplateAt(FieldSystem *fieldSystem, int x, int z);
void FieldSystem_SetTerrainQueries(void **a0, int a1);
BOOL FieldSystem_CheckTileCollision(FieldSystem *fieldSystem, int x, int z);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
BOOL FieldSystem_FindMapProp(FieldSystem *fieldSystem, int buildModel, struct MapProp **outProp, int *outIdx);
BOOL FieldSystem_FindMapPropOfModels(FieldSystem *fieldSystem, int *models, u32 count, struct MapProp **outProp, int *outModel);
void Terrain_GetChunkOrigin(int idx, int width, VecFx32 *pos);
BOOL FieldSystem_CheckCollisionAndHeight(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int a4);
u32 FieldSystem_CheckCollisionHeightAndGimmick(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *a4);
BOOL sub_02060BFC(LocalMapObject *playerObj, int xInFront, int playerElev, int yInFront);
void FollowPlayerMovement_Reset(LocalMapObject *object);

#endif // POKEHEARTGOLD_FIELD_TERRAIN_H
