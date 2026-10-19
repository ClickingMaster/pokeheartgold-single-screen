#ifndef POKEHEARTGOLD_FIELD_PUDDLE_SPLASH_H
#define POKEHEARTGOLD_FIELD_PUDDLE_SPLASH_H

#include "field/field_effect_manager.h"

// Splashes where a map object steps into a puddle (field effect renderer 6).

typedef struct PuddleSplash PuddleSplash;

PuddleSplash *PuddleSplash_New(FieldEffectManager *manager);
void PuddleSplash_Free(PuddleSplash *splash);
void PuddleSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z);

#endif // POKEHEARTGOLD_FIELD_PUDDLE_SPLASH_H
