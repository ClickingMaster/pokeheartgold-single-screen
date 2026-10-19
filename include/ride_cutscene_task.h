#ifndef POKEHEARTGOLD_RIDE_CUTSCENE_TASK_H
#define POKEHEARTGOLD_RIDE_CUTSCENE_TASK_H

#include "field/model_attributes.h"

// The task environment of the Magnet Train ride and the ship voyage: leave the overworld, play the cutscene
// lit with the area light, then change map to the destination.
typedef struct AreaLightTaskEnv {
    u8 state;
    u8 param1;
    u8 param2;
    u8 pad03;
    u32 mapId;
    u32 x;
    u32 y;
    ModelAttributes light;
} AreaLightTaskEnv;

#endif // POKEHEARTGOLD_RIDE_CUTSCENE_TASK_H
