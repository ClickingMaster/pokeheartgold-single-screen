#ifndef POKEHEARTGOLD_OVY_18_H
#define POKEHEARTGOLD_OVY_18_H

#include "battle/battle.h"

#include "menu_input_state.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pokedex.h"
#include "unk_02092BB8.h"

typedef struct PokedexArgs {
    Pokedex *pokedex;
    PlayerProfile *playerProfile;
    UnkStruct_02092BB8 *unk_08;
    MenuInputStateMgr *menuInputStatePtr;
    int x;
    int y;
    u16 mapId;
    u16 mapMatrixId;
} PokedexArgs;

BOOL Pokedex_Init(OverlayManager *man, int *state);
BOOL Pokedex_Main(OverlayManager *man, int *state);
BOOL Pokedex_Exit(OverlayManager *man, int *state);

OverlayManager *PokedexEntryPopup_Create(UnkStruct_50C *unkStruct);
s32 PokedexEntryPopup_IsDone(OverlayManager *man);
void PokedexEntryPopup_Delete(OverlayManager *man);
Pokepic *PokedexEntryPopup_GetPokepic(OverlayManager *man);
void PokedexEntryPopup_HideSprites(OverlayManager *man);

#endif // POKEHEARTGOLD_OVY_18_H
