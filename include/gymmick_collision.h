#ifndef POKEHEARTGOLD_GYMMICK_COLLISION_H
#define POKEHEARTGOLD_GYMMICK_COLLISION_H

#include "field_system.h"

void Gymmick_Init(FieldSystem *fieldSystem);
void Gymmick_Free(FieldSystem *fieldSystem);
BOOL Gymmick_CheckCollision(FieldSystem *fieldSystem, u32 tileX, u32 tileZ, u32 height, BOOL *isColliding);

#endif // POKEHEARTGOLD_GYMMICK_COLLISION_H
