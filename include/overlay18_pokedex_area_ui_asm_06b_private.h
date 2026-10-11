#ifndef OVERLAY18_POKEDEX_AREA_UI_ASM_06B_PRIVATE_H
#define OVERLAY18_POKEDEX_AREA_UI_ASM_06B_PRIVATE_H

#include "global.h"

void ManagedSprite_SetDrawFlag(void *, int);
int Pokedex_GetInternationalViewFlag(void *);
void PokedexApp_SetSpritePairVisibleIfInternationalView(void *, int, int);

#endif
