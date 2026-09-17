#ifndef POKEHEARTGOLD_TO50_MAP_OBJECT_HEIGHT_ADJUST_INTERNAL_H
#define POKEHEARTGOLD_TO50_MAP_OBJECT_HEIGHT_ADJUST_INTERNAL_H

#include "map_object_movement_utilities_internal.h"

BOOL MapObject_CheckIgnoreHeights(LocalMapObject *object);
BOOL MapObject_CheckFlag29(LocalMapObject *object);
void MapObject_SetCurrentY(LocalMapObject *object, s32 currentY);
int sub_02061070(LocalMapObject *object);

#endif
