#ifndef POKEHEARTGOLD_FIELD_TALL_GRASS_H
#define POKEHEARTGOLD_FIELD_TALL_GRASS_H

#include "field/field_effect_manager.h"

// Tall grass around map objects (field effect renderer 8).

typedef struct TallGrass TallGrass;

TallGrass *TallGrass_New(FieldEffectManager *manager);
void TallGrass_Free(TallGrass *grass);

#endif // POKEHEARTGOLD_FIELD_TALL_GRASS_H
