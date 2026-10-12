#ifndef POKEHEARTGOLD_UNK_02054648_H
#define POKEHEARTGOLD_UNK_02054648_H

#include "field/field_control.h"

#include "script.h"

struct MapProp;

SoundplateStruct *sub_02054874(FieldSystem *fieldSystem, int x, int z);
void sub_0205489C(void **a0, int a1);
BOOL sub_020548C0(FieldSystem *fieldSystem, int x, int z);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
BOOL sub_02054C20(FieldSystem *fieldSystem, int buildModel, struct MapProp **outProp, int *outIdx);
BOOL sub_02054C90(FieldSystem *fieldSystem, int *models, u32 count, struct MapProp **outProp, int *outModel);
void sub_02054DC8(int idx, int width, VecFx32 *pos);
BOOL sub_020549A8(FieldSystem *fieldSystem, VecFx32 *playerPos, int xInFront, int yInFront, int a4);
u32 sub_020549F4(FieldSystem *fieldSystem, VecFx32 *playerPos, u32 x, u32 y, u32 *a4);
BOOL sub_02060BFC(LocalMapObject *playerObj, int xInFront, int playerElev, int yInFront);
void FollowPlayerMovement_Reset(LocalMapObject *object);

#endif // POKEHEARTGOLD_UNK_02054648_H
