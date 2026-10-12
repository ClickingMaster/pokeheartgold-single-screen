#ifndef FIELD_FOLLOW_TERRAIN_R40_PRIVATE_H
#define FIELD_FOLLOW_TERRAIN_R40_PRIVATE_H
#include "global.h"

#include "map_object.h"
#include "metatile_behavior.h"

typedef struct FourValues {
    u32 values[4];
} FourValues;
extern const FourValues _020FE3F4;
extern const FourValues _020FE434;
extern const FourValues _020FE444;

void GF_AssertFail(void);
u8 GetMetatileBehavior(FieldSystem *fieldSystem, int x, int z);
void TallGrass_Spawn(LocalMapObject *object, int kind);
void VeryTallGrass_Spawn(LocalMapObject *object, int kind);
BOOL FollowMonObject_IsLarge(LocalMapObject *object);
void MapObject_GetCoordsBehind(LocalMapObject *object, int *x, int *z);
void TallGrass_SpawnAt(LocalMapObject *object, int kind, int x, int z, int flag);
void VeryTallGrass_SpawnAt(LocalMapObject *object, int kind, int x, int z, int flag);
int sub_02066444(int value);
void sub_020664D8(LocalMapObject *object);
#endif
