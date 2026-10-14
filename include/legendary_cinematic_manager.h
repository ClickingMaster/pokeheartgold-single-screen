#ifndef POKEHEARTGOLD_LEGENDARY_CINEMATIC_MANAGER_H
#define POKEHEARTGOLD_LEGENDARY_CINEMATIC_MANAGER_H

#include "overlay_manager.h"
#include "save.h"

typedef struct ModelAttributesStorage {
    int unk0[0x12];
} ModelAttributesStorage;

typedef struct LegendaryCinematicArgs {
    SaveData *saveData;
    ModelAttributesStorage modelAttributes;
    u16 scene;
    u16 timeOfDay;
} LegendaryCinematicArgs;

BOOL LegendaryCinematic_Init(OverlayManager *man, int *state);
BOOL LegendaryCinematic_Main(OverlayManager *man, int *state);
BOOL LegendaryCinematic_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_LEGENDARY_CINEMATIC_MANAGER_H
