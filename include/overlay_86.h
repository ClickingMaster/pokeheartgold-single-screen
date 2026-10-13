#ifndef POKEHEARTGOLD_OVY_86_H
#define POKEHEARTGOLD_OVY_86_H

#include "overlay_manager.h"
#include "save.h"

typedef struct FrontierRecordsAppArgs {
    SaveData *saveData;
    u8 mode;
    u8 facility;
    u16 species;
} FrontierRecordsAppArgs;

BOOL FrontierRecordsApp_Init(OverlayManager *man, int *state);
BOOL FrontierRecordsApp_Main(OverlayManager *man, int *state);
BOOL FrontierRecordsApp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_86_H
