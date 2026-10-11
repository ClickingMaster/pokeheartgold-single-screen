#ifndef POKEHEARTGOLD_FIELD_MAP_OBJECT_MODEL_LIST_H
#define POKEHEARTGOLD_FIELD_MAP_OBJECT_MODEL_LIST_H

#include "field/field_effect_manager.h"

typedef struct MapObjectModelList MapObjectModelList;

MapObjectModelList *MapObjectModelList_New(FieldEffectManager *manager);
void MapObjectModelList_Free(MapObjectModelList *list);
FieldEffect *MapObjectModel_CreateEffect(LocalMapObject *mapObject, const VecFx32 *offset);
void MapObjectModelList_Init(FieldEffectManager *manager);

#endif // POKEHEARTGOLD_FIELD_MAP_OBJECT_MODEL_LIST_H
