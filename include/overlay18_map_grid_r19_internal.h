#ifndef OVERLAY18_MAP_GRID_R19_INTERNAL_H
#define OVERLAY18_MAP_GRID_R19_INTERNAL_H

#include "global.h"

#define S8_AT(p, n)  (*(s8 *)((u8 *)(p) + (n)))
#define U32_AT(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define PTR_AT(p, n) (*(void **)((u8 *)(p) + (n)))

#pragma require_prototypes off

extern const u8 ov18_021FA35A[];
extern const u8 sAreaMarkerSpriteTemplate[];

void PokedexApp_SetSpriteVisible(void *, u32, u32);
void *PokedexApp_NewSprite(void *, const void *);
void PokedexApp_SetSpriteTranslucent(void *, u32, u32);
void PokedexApp_SetSpritePos(void *, u32, s16, s16, u32);
void PokedexApp_SetScrollbarSizeAnim(void *, u32, s8, const void *, u32);
void PokedexApp_UpdateScrollArrows(void *, s8, s8, u32);
void PokedexApp_SyncAreaScrollbar(void *);
void PokedexApp_UpdatePlayerMarkerVisibility(void *);
u32 ov18_021E8B18(u32);

#endif
