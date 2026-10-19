#ifndef POKEHEARTGOLD_FIELD_GROUND_DUST_H
#define POKEHEARTGOLD_FIELD_GROUND_DUST_H

#include "field/field_effect_manager.h"

// Dust where a map object lands on plain ground (field effect renderer 11).

typedef struct GroundDust GroundDust;

GroundDust *GroundDust_New(FieldEffectManager *manager);
void GroundDust_Free(GroundDust *dust);
void GroundDust_Spawn(LocalMapObject *mapObject);

#endif // POKEHEARTGOLD_FIELD_GROUND_DUST_H
