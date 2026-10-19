#ifndef POKEHEARTGOLD_FIELD_TRAINER_REVEAL_H
#define POKEHEARTGOLD_FIELD_TRAINER_REVEAL_H

#include "field/field_effect_manager.h"

// The burst of a hidden trainer jumping out of its hiding place (field effect renderer 15).

typedef struct TrainerReveal TrainerReveal;

TrainerReveal *TrainerReveal_New(FieldEffectManager *manager);
void TrainerReveal_Free(TrainerReveal *reveal);
void TrainerReveal_Spawn(LocalMapObject *mapObject);

#endif // POKEHEARTGOLD_FIELD_TRAINER_REVEAL_H
