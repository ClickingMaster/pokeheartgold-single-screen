#ifndef POKEHEARTGOLD_FIELD_HIDDEN_OBJECT_COVER_H
#define POKEHEARTGOLD_FIELD_HIDDEN_OBJECT_COVER_H

#include "field/field_effect_manager.h"

typedef struct HiddenObjectCover HiddenObjectCover;

HiddenObjectCover *HiddenObjectCover_New(FieldEffectManager *manager);
void HiddenObjectCover_Free(HiddenObjectCover *a0);
void HiddenObjectCover_Spawn(LocalMapObject *mapObject, int a1);

#endif // POKEHEARTGOLD_FIELD_HIDDEN_OBJECT_COVER_H
