#ifndef POKEHEARTGOLD_BALL_CAPSULE_EDITOR_H
#define POKEHEARTGOLD_BALL_CAPSULE_EDITOR_H

#include "global.h"

#include "menu_input_state.h"
#include "options.h"
#include "overlay_manager.h"
#include "pokemon.h"
#include "save.h"
#include "seal_case.h"

typedef struct BallCapsuleEditorArgs {
    int partyCount;
    Pokemon *mons[PARTY_SIZE];
    Party *party;
    SealCase *sealCase;
    Options *options;
    SaveData *saveData;
    MenuInputStateMgr *menuInputStatePtr;
    u8 capsuleIndex;    // the selected capsule (kept between visits)
    u8 setCapsuleOnMon; // TRUE: open the party menu to put the capsule on a Pokémon
} BallCapsuleEditorArgs;

BOOL BallCapsuleEditor_Init(OverlayManager *manager, int *state);
BOOL BallCapsuleEditor_Main(OverlayManager *manager, int *state);
BOOL BallCapsuleEditor_Exit(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_BALL_CAPSULE_EDITOR_H
