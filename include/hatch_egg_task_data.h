#ifndef POKEHEARTGOLD_HATCH_EGG_TASK_DATA_H
#define POKEHEARTGOLD_HATCH_EGG_TASK_DATA_H

#include "hatch_egg_task.h"
#include "naming_screen.h"

// The hatch egg task environment, also the args of the hatch egg app overlay.
typedef enum HatchEggTaskState {
    HATCHEGGTASKSTATE_LEAVE_OVERWORLD,
    HATCHEGGTASKSTATE_OPEN_HATCH_APP,
    HATCHEGGTASKSTATE_UPDATE_MON_INFO,
    HATCHEGGTASKSTATE_OPEN_NAMING_SCREEN,
    HATCHEGGTASKSTATE_SET_MON_NAME,
    HATCHEGGTASKSTATE_LOAD_OVERWORLD,
    HATCHEGGTASKSTATE_EXIT,
} HatchEggTaskState;

typedef struct HatchEggTaskData {
    HatchEggTaskState state;
    BOOL wantToNameNewMon;
    NamingScreenArgs *namingScreenArgs;
    HatchEggArgs args;
} HatchEggTaskData;

#endif // POKEHEARTGOLD_HATCH_EGG_TASK_DATA_H
