#ifndef TO45_OVERLAY_18_R20_PRIVATE_H
#define TO45_OVERLAY_18_R20_PRIVATE_H

#include "global.h"

#include "application/pokedex/pokedex_internal.h"

#pragma require_prototypes off

#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void PokedexApp_SetSpritePos(PokedexAppData *, int, int, int, int);
void PokedexApp_GetSpritePos(PokedexAppData *, int, s16 *, s16 *, int);
void *PokedexApp_NewSprite(PokedexAppData *, const void *);
void PokedexApp_LoadSizeCheckResources(PokedexAppData *);
void PokedexApp_SetSizeCheckMonIcons(PokedexAppData *);
void PokedexApp_DrawSizeCheckMonPic(PokedexAppData *);
void PokedexApp_ScaleSizeCheckTrainer(PokedexAppData *);

extern u8 ov18_021FA3B0[];
typedef struct Ov18SpriteTemplate {
    u8 raw[0x34];
} Ov18SpriteTemplate;

extern Ov18SpriteTemplate ov18_021FA7B0[];

#endif
