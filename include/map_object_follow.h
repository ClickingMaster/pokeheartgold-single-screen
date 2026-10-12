#ifndef POKEHEARTGOLD_MAP_OBJECT_FOLLOW_H
#define POKEHEARTGOLD_MAP_OBJECT_FOLLOW_H

#include "map_object.h"

LocalMapObject *MapObject_FindTrainerPartner(LocalMapObject *object);
int HiddenObjectMovement_SetCoverEffect(LocalMapObject *object, int effect);

#endif // POKEHEARTGOLD_MAP_OBJECT_FOLLOW_H
