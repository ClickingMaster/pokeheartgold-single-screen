#ifndef POKEHEARTGOLD_LEGENDARY_CINEMATIC_MANAGER_PRIVATE_H
#define POKEHEARTGOLD_LEGENDARY_CINEMATIC_MANAGER_PRIVATE_H

#include "global.h"

#include "heap.h"
#include "legendary_cinematic.h"
#include "overlay_manager.h"
#include "legendary_cinematic_manager.h"

typedef struct LegendaryCinematicData {
    LegendaryCinematicArgs *args;
    u8 rest[0x418];
} LegendaryCinematicData;

#endif
