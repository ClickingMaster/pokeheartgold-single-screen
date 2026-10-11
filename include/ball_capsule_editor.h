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
    int partyCount;                       // 0x00
    Pokemon *mons[PARTY_SIZE];            // 0x04
    Party *party;                         // 0x1C
    SealCase *sealCase;                   // 0x20
    Options *options;                     // 0x24
    SaveData *saveData;                   // 0x28
    MenuInputStateMgr *menuInputStatePtr; // 0x2C
    u8 capsuleIndex;                      // 0x30, the selected capsule (kept between visits)
    u8 setCapsuleOnMon;                   // 0x31, TRUE: open the party menu to put the capsule on a Pokémon
} BallCapsuleEditorArgs;

BOOL BallCapsuleEditor_Init(OverlayManager *manager, int *state);
BOOL BallCapsuleEditor_Main(OverlayManager *manager, int *state);
BOOL BallCapsuleEditor_Exit(OverlayManager *manager, int *state);

#endif // POKEHEARTGOLD_BALL_CAPSULE_EDITOR_H
