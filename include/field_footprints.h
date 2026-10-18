#ifndef POKEHEARTGOLD_FIELD_FOOTPRINTS_H
#define POKEHEARTGOLD_FIELD_FOOTPRINTS_H

#include "field/field_effect_manager.h"

// Footprints and bike tracks in sand and snow (field effect renderer 2).

typedef struct Footprints Footprints;

Footprints *Footprints_New(FieldEffectManager *manager);
void Footprints_Free(Footprints *footprints);
BOOL Footprints_SpawnInSand(LocalMapObject *mapObject);
BOOL Footprints_SpawnTracksInSand(LocalMapObject *mapObject);
BOOL Footprints_SpawnInSnow(LocalMapObject *mapObject);
BOOL Footprints_SpawnOnBehavior169(LocalMapObject *mapObject);
BOOL Footprints_SpawnTracksOnBehavior169(LocalMapObject *mapObject);

#endif // POKEHEARTGOLD_FIELD_FOOTPRINTS_H
