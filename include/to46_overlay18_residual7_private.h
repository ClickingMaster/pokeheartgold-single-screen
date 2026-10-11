#ifndef TO46_OVERLAY18_RESIDUAL7_PRIVATE_H
#define TO46_OVERLAY18_RESIDUAL7_PRIVATE_H

#include "global.h"

#include "sprite_system.h"

void PokedexApp_SetSpriteVisible(void *, int, int);
ManagedSprite *PokedexApp_NewSprite(void *, const ManagedSpriteTemplate *);
void PokedexApp_MoveSprite(void *, int, int, int, int);
void PokedexApp_SetSpritePos(void *, int, s16, s16, int);
void PokedexApp_GetSpritePos(void *, int, s16 *, s16 *, int);

#endif
