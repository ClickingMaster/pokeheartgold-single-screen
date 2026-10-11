#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_RESULT_HELPERS_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_RESULT_HELPERS_PRIVATE_H

#include "global.h"

void PokedexApp_SetSpriteAnim(void *, int, int);
void PokedexApp_MoveSprite(void *, int, int, int, int);
void PokedexApp_ShowFormMonPics(void *, int, int);

void ov18_021F5EF0(void *, int);
void PokedexApp_MoveFormListIcons(void *, int);
void PokedexApp_UpdateScrollArrows(void *, int, int, int);
void PokedexApp_SetScrollbarSizeAnim(void *, int, int, const u16 *, u32);
int PokedexApp_GetScrollbarSizeAnim(void *, int, const u16 *, u32);

#endif
