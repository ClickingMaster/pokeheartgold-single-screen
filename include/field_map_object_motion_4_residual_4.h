#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_MOTION_4_RESIDUAL_4_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_MOTION_4_RESIDUAL_4_H

#include "global.h"

#include "field_object_models_internal.h"

BOOL MapObject_IsShinyPokemon(LocalMapObject *object);
BOOL MapObject_HasFollowMonSprite(LocalMapObject *object);
void MapObject_SetBillboardSepia(void *select, LocalMapObject *object, Sprite *sprite);
void ov01_021F8D80(LocalMapObject *object, Sprite *sprite, UnkObjExtra *extra, u32 direction, BOOL update);
void ov01_021F8C88(LocalMapObject *object, Sprite *sprite);

void ov01_021F7810(LocalMapObject *object);
void ov01_021F7894(LocalMapObject *object);
void ov01_021F7918(LocalMapObject *object);

#endif
