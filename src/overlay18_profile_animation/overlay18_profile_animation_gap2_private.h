#ifndef TO53_OVERLAY18_PROFILE_ANIMATION_GAP2_PRIVATE_H
#define TO53_OVERLAY18_PROFILE_ANIMATION_GAP2_PRIVATE_H

#include "application/pokedex/pokedex_internal.h"
#include "global.h"
#include "sprite_system.h"

void PokedexApp_SetMonIcon(PokedexAppData *, u32, u32, int);
void PokedexApp_SetMonIconSub(PokedexAppData *, u32, u32, int);
void PokedexApp_SetSpritePos(PokedexAppData *, int, s16, s16, int);
void PokedexApp_GetSpritePos(PokedexAppData *, int, s16 *, s16 *, int);
void PokedexApp_GetSeenFormAndGender(PokedexAppData *, int, u8 *, u8 *);
void PokedexApp_LoadMonPicToBg(PokedexAppData *, u16, u8, u8, int);
void PokedexApp_SetMonPicBgAffine(PokedexAppData *, s16, s16);

extern const s16 FX_SinCosTable_[];
fx32 FX_Div(fx32, fx32);

void PokedexApp_SetSizeCheckMonIcons(PokedexAppData *);
void PokedexApp_DrawSizeCheckMonPic(PokedexAppData *);
void PokedexApp_ScaleSizeCheckTrainer(PokedexAppData *);
void ov18_021F4EB0(u16, s16 *, s16 *);

#endif
