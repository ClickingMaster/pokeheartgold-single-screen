#ifndef POKEHEARTGOLD_FIELD_MUD_SPLASH_H
#define POKEHEARTGOLD_FIELD_MUD_SPLASH_H

#include "field/field_effect_manager.h"

// Mud splashes where a map object steps into mud (field effect renderer 7).

typedef struct MudSplash MudSplash;

MudSplash *MudSplash_New(FieldEffectManager *manager);
void MudSplash_Free(MudSplash *splash);
void MudSplash_Spawn(LocalMapObject *mapObject, int x, int y, int z);

#endif // POKEHEARTGOLD_FIELD_MUD_SPLASH_H
