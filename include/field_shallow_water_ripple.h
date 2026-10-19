#ifndef POKEHEARTGOLD_FIELD_SHALLOW_WATER_RIPPLE_H
#define POKEHEARTGOLD_FIELD_SHALLOW_WATER_RIPPLE_H

#include "field/field_effect_manager.h"

// Ripples around a map object standing in shallow water (field effect renderer 10).

typedef struct ShallowWaterRipple ShallowWaterRipple;

ShallowWaterRipple *ShallowWaterRipple_New(FieldEffectManager *manager);
void ShallowWaterRipple_Free(ShallowWaterRipple *ripple);
void ShallowWaterRipple_Spawn(LocalMapObject *mapObject, BOOL loop);

#endif // POKEHEARTGOLD_FIELD_SHALLOW_WATER_RIPPLE_H
