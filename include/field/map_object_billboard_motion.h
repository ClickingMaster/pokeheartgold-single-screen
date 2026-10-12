#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H

#include "map_object.h"
#include "sprite.h"

Sprite *MapObject_GetBillboard(LocalMapObject *object);
void FollowMonDraw_UpdatePositionOnly(LocalMapObject *object);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_BILLBOARD_MOTION_H
