#ifndef POKEHEARTGOLD_RECORD_MIXING_H
#define POKEHEARTGOLD_RECORD_MIXING_H

#include "game_stats.h"
#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "save.h"
#include "script.h"

typedef struct RecordMixingArgs {
    SaveData *saveData;
    struct UnkStruct_0205AC88 *unk4;
    Options *options;
    GameStats *gameStats;
    MenuInputStateMgr *menuInputStatePtr;
} RecordMixingArgs;

BOOL RecordMixing_Init(OverlayManager *man, int *state);
BOOL RecordMixing_Main(OverlayManager *man, int *state);
BOOL RecordMixing_Exit(OverlayManager *man, int *state);

// The other app of this overlay: a Nintendo WFC friend lobby, run with OVY_72 loaded.
BOOL NintendoWfcUnused_Init(OverlayManager *man, int *state);
BOOL NintendoWfcUnused_Main(OverlayManager *man, int *state);
BOOL NintendoWfcUnused_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_RECORD_MIXING_H
