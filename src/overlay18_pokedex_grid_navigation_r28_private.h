#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_GRID_NAVIGATION_R28_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_GRID_NAVIGATION_R28_PRIVATE_H

#include "global.h"

#include "constants/heap.h"
#include "constants/sndseq.h"

#include "application/pokedex/pokedex_internal.h"

#include "menu_input_state.h"
#include "system.h"
#include "unk_02020A0C.h"

extern const TouchscreenHitbox sSearchTypeHitboxes[];
extern const DpadMenuBox sSearchTypeDpadBoxes[];
extern const GridCallbacks sSearchTypeGridCallbacks[];
extern const u8 sSearchTypeCursorAnims[];

void PlaySE(u32 seqNo);

void PokedexApp_UpdateSearchTypeCursor(PokedexAppData *pokedexApp, int index);
void PokedexApp_OnSearchTypeGridMove(PokedexAppData *pokedexApp, int currentInput, int lastInput);

#endif // POKEHEARTGOLD_OVERLAY18_POKEDEX_GRID_NAVIGATION_R28_PRIVATE_H
