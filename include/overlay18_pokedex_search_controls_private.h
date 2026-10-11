#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_SEARCH_CONTROLS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_SEARCH_CONTROLS_PRIVATE_H

#include "global.h"

#include "menu_input_state.h"
#include "sprite_system.h"
#include "system.h"
#include "sound_playback.h"
#include "unk_02019BA4.h"
#include "unk_02020A0C.h"

typedef struct Ov18MenuState {
    u8 raw[0x18a0];
} Ov18MenuState;

extern const TouchscreenHitbox sSearchFormHitboxes[];
extern const DpadMenuBox sSearchFormDpadBoxes[];
extern const GridCallbacks sSearchFormGridCallbacks[];
extern const u8 sSearchFormCursorAnims[];

void PokedexApp_SetSpriteVisible(Ov18MenuState *, int, int);
void PokedexApp_SetSpriteAnim(Ov18MenuState *, int, int);
void PokedexApp_UpdateSearchFormCursor(Ov18MenuState *, int);
void PokedexApp_CreateSearchFormGrid(Ov18MenuState *);
int PokedexApp_HandleSearchFormInput(Ov18MenuState *);
void PokedexApp_OnSearchFormGridMove(Ov18MenuState *, int, int);

#endif
