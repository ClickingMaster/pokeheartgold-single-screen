#ifndef POKEHEARTGOLD_HATCH_EGG_TASK_H
#define POKEHEARTGOLD_HATCH_EGG_TASK_H

#include "options.h"
#include "player_data.h"
#include "pokemon.h"
#include "task.h"

typedef struct HatchEggArgs {
    Pokemon *mon;
    Options *options;
    PlayerProfile *profile;
    u16 unkC;
} HatchEggArgs;

void CallTask_HatchEggInParty(TaskManager *taskman, HatchEggArgs *args);

#endif // POKEHEARTGOLD_HATCH_EGG_TASK_H
