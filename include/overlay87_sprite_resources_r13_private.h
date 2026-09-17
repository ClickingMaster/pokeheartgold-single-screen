#ifndef POKEHEARTGOLD_OVERLAY87_SPRITE_RESOURCES_R13_PRIVATE_H
#define POKEHEARTGOLD_OVERLAY87_SPRITE_RESOURCES_R13_PRIVATE_H
#include "global.h"
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))
void *AddCharResObjFromNarc(void *, u32, u32, u32, u32, u32, u32);
void *AddPlttResObjFromNarc(void *, u32, u32, u32, u32, u32, u32, u32);
void *AddCellOrAnimResObjFromNarc(void *, u32, u32, u32, u32, u32, u32);
void ov87_021E7AB0(void *, s32, s32);
void ov87_021E7BC4(void *, u32);
void ov87_021E7C50(void *, u32, u32, u32);
void ov87_021E7CE0(void *, u32);
void ov87_021E7D6C(void *, u32);
#endif
