#ifndef POKEHEARTGOLD_FRONTIER_FRONTIER_BRAIN_INTRO_H
#define POKEHEARTGOLD_FRONTIER_FRONTIER_BRAIN_INTRO_H

#include "bg_window.h"
#include "palette.h"
#include "sprite_system.h"

// Starts the cutscene shown before a Frontier Brain battle. brain: 1-based index into the Brain table; *done is set
// to 1 when the cutscene ends.
void FrontierBrainIntro_Start(int brain, BgConfig *bgConfig, SpriteSystem *spriteSystem, SpriteManager *spriteManager, PaletteData *paletteData, u16 *done, s16 x, s16 y);

#endif // POKEHEARTGOLD_FRONTIER_FRONTIER_BRAIN_INTRO_H
