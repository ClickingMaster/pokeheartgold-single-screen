#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_STATUS_SPRITES_R11_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_STATUS_SPRITES_R11_PRIVATE_H

#include "application/pokedex/pokedex_internal.h"

extern const u16 sListScrollbarSizeThresholds[];
extern const u8 ov18_021FA310[];

int PokedexApp_GetListScrollbarSizeAnim(PokedexAppData *pokedexApp, BOOL mode);
u32 PokedexApp_GetListScrollbarLength(PokedexAppData *pokedexApp, int index);

#endif // POKEHEARTGOLD_OVERLAY18_POKEDEX_STATUS_SPRITES_R11_PRIVATE_H
