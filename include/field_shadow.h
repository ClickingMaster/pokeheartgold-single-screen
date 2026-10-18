#ifndef POKEHEARTGOLD_FIELD_SHADOW_H
#define POKEHEARTGOLD_FIELD_SHADOW_H

#include "field/field_effect_manager.h"

// Map object ground shadows (field effect renderer 0).

typedef struct FieldShadow FieldShadow;

FieldShadow *FieldShadow_New(FieldEffectManager *manager);
void FieldShadow_Free(FieldShadow *shadow);
void FieldShadow_CreateMapObjectShadow(LocalMapObject *object);
void FieldShadow_CreateFollowMonShadow(LocalMapObject *object);
void FieldShadow_OffsetByFacing(u32 direction, VecFx32 *pos);

#endif // POKEHEARTGOLD_FIELD_SHADOW_H
