#ifndef POKEHEARTGOLD_FIELD_VERY_TALL_GRASS_H
#define POKEHEARTGOLD_FIELD_VERY_TALL_GRASS_H

#include "field/field_effect_manager.h"

// Very tall grass around map objects (field effect renderer 12).

typedef struct VeryTallGrass VeryTallGrass;

VeryTallGrass *VeryTallGrass_New(FieldEffectManager *manager);
void VeryTallGrass_Free(VeryTallGrass *grass);
void VeryTallGrass_Spawn(LocalMapObject *mapObject, BOOL animate);

#endif // POKEHEARTGOLD_FIELD_VERY_TALL_GRASS_H
