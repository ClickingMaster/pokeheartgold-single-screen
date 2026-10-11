#ifndef POKEHEARTGOLD_OVERLAY18_PROFILE_ANIMATION_INTERNAL_H
#define POKEHEARTGOLD_OVERLAY18_PROFILE_ANIMATION_INTERNAL_H
#include "global.h"

#include "sprite_system.h"

#define PTR(p, off, T) (*(T **)((u8 *)(p) + (off)))
typedef struct R21State {
    void *root;
    u32 velocity;
    u16 state;
    u8 direction;
    u8 pad;
    s32 x;
    s32 y;
    s32 z;
} R21State;

void PokedexApp_UnloadMonIconResources(void *, u32);
void PokedexApp_SetMonIcon(void *, u32, u32, int);
void PokedexApp_SetMonIconSub(void *, u32, u32, int);
void PokedexApp_SetSpritePos(void *, int, s16, s16, int);
void PokedexApp_GetSpritePos(void *, int, s16 *, s16 *, int);
void ov18_021F4EB0(u16, s16 *, s16 *);
void ov18_021F4F98(void *, int, u16, s16, s16);
void ov18_021F4FC8(void *, int, int);
void PokedexApp_GetSeenFormAndGender(void *, int, u8 *, u8 *);
void PokedexApp_LoadMonPicToBg(void *, u16, u8, u8, int);
void PokedexApp_SetMonPicBgAffine(void *, s16, s16);

void PokedexApp_UnloadSizeCheckResources(void *);
void PokedexApp_SetSizeCheckMonIcons(void *);
void PokedexApp_DrawSizeCheckMonPic(void *);
void ov18_021F4F98(void *, int, u16, s16, s16);
void ov18_021F4FC8(void *, int, int);
void PokedexApp_UpdateScalePans(void *, u16);
void PokedexApp_RunWeightCheckAnim(R21State *);
void ov18_021F5180(R21State *, s32, s32);
void ov18_021F5198(R21State *, s32, s32, s32);
extern void (*const sWeightCheckAnimFuncs[])(R21State *);
#endif
