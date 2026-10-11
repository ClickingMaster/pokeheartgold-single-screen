#ifndef POKEHEARTGOLD_SAFARI_AREA_CUSTOMIZER_H
#define POKEHEARTGOLD_SAFARI_AREA_CUSTOMIZER_H

#include "menu_input_state.h"
#include "overlay_manager.h"
#include "save.h"

// Overlay 108 holds two Safari Zone apps: the area customizer (choose and arrange the six areas of the Safari
// Zone) and the area decoration (place objects in an area).
typedef struct SafariAreaCustomizerArgs {
    SaveData *saveData;
    MenuInputStateMgr *menuInputStatePtr;
    u8 unk8[0x1c];
} SafariAreaCustomizerArgs;

BOOL SafariDecoration_Init(OverlayManager *man, int *state);
BOOL SafariDecoration_Main(OverlayManager *man, int *state);
BOOL SafariDecoration_Exit(OverlayManager *man, int *state);

BOOL SafariAreaCustomizer_Init(OverlayManager *man, int *state);
BOOL SafariAreaCustomizer_Main(OverlayManager *man, int *state);
BOOL SafariAreaCustomizer_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_SAFARI_AREA_CUSTOMIZER_H
