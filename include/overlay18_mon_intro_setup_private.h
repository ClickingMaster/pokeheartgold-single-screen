#ifndef POKEHEARTGOLD_OVERLAY18_MON_INTRO_SETUP_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_MON_INTRO_SETUP_PRIVATE_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "palette.h"
#include "pokemon.h"
#include "sound_system.h"
#include "sprite.h"
#include "sound_playback.h"

typedef struct Ov18State {
    u8 raw[0x258];
} Ov18State;

extern const BgTemplate sEntryPopupBg2Template;
extern const BgTemplate sEntryPopupBg1Template;
extern const BgTemplate sEntryPopupBg3Template;

void PokedexEntryPopup_Task(void *, Ov18State *);
void PokedexEntryPopup_ReadMonData(Ov18State *);
void PokedexEntryPopup_InitBgs(Ov18State *);
void PokedexEntryPopup_DrawWindows(Ov18State *);
void PokedexEntryPopup_InitSprites(Ov18State *);
void PokedexEntryPopup_CreatePokepic(Ov18State *);
void PokedexEntryPopup_StartFadeIn(Ov18State *);
BOOL PokedexEntryPopup_IsFadeDone(Ov18State *);
void PokedexEntryPopup_FlashBanner(Ov18State *);

#endif
