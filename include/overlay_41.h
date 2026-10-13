#ifndef POKEHEARTGOLD_OVY_41_H
#define POKEHEARTGOLD_OVY_41_H

#include "fashion_case.h"
#include "overlay_manager.h"

typedef struct FashionAppData {
    SaveFashionData *saveFashionData;
    int entryIndex;
    int mode;
} FashionAppData;

BOOL AccessoryPortrait_Init(OverlayManager *man, int *state);
BOOL AccessoryPortrait_Main(OverlayManager *man, int *state);
BOOL AccessoryPortrait_Exit(OverlayManager *man, int *state);

BOOL DressUp_Init(OverlayManager *man, int *state);
BOOL DressUp_Main(OverlayManager *man, int *state);
BOOL DressUp_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_41_H
