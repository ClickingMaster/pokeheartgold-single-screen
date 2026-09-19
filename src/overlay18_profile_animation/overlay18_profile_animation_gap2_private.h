#ifndef TO53_OVERLAY18_PROFILE_ANIMATION_GAP2_PRIVATE_H
#define TO53_OVERLAY18_PROFILE_ANIMATION_GAP2_PRIVATE_H

#include "application/pokedex/pokedex_internal.h"
#include "global.h"
#include "sprite_system.h"

void ov18_021F14FC(PokedexAppData *, u32, u32, int);
void ov18_021F1534(PokedexAppData *, u32, u32, int);
void ov18_021F1294(PokedexAppData *, int, s16, s16, int);
void ov18_021F12C8(PokedexAppData *, int, s16 *, s16 *, int);
void ov18_021F3CA8(PokedexAppData *, int, u8 *, u8 *);
void ov18_021F69E8(PokedexAppData *, u16, u8, u8, int);
void ov18_021F6AB0(PokedexAppData *, s16, s16);

extern const s16 FX_SinCosTable_[];
fx32 FX_Div(fx32, fx32);

void ov18_021F4D64(PokedexAppData *);
void ov18_021F4DDC(PokedexAppData *);
void ov18_021F4E28(PokedexAppData *);
void ov18_021F4EB0(u16, s16 *, s16 *);

#endif
