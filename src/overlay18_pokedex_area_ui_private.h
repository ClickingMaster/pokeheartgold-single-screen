#ifndef POKEHEARTGOLD_OVERLAY18_POKEDEX_AREA_UI_H
#define POKEHEARTGOLD_OVERLAY18_POKEDEX_AREA_UI_H

#include "global.h"

#include "sprite_system.h"

void PlaySE(u16 seqNo);
void Heap_Free(void *ptr);
int Pokedex_GetInternationalViewFlag(void *pokedex);
int Pokedex_GetSeenFormByIdx(void *pokedex, int species, int idx);
int LanguageToDexFlag(u32 language);

int sub_020912AC(int a0);
int sub_02091294(int a0);

extern const ManagedSpriteTemplate sTypeIconSpriteTemplate;
extern const ManagedSpriteTemplate sFootprintSpriteTemplate;
extern const ManagedSpriteTemplate sLanguageIconSpriteTemplate;
extern const ManagedSpriteTemplate ov18_021FA984[];
extern const ManagedSpriteTemplate ov18_021FAB24;
extern const ManagedSpriteTemplate ov18_021FAB58;
extern const ManagedSpriteTemplate ov18_021FAB8C;

void ov18_021E5900(void);
void ov18_021E5908(void);
int ov18_021E6D10(void *p, int a1, u16 a2);
void PokedexApp_LoadSpriteCharData(void *p, int idx, void *str, int a3, int a4);
void PokedexApp_SetSpriteAnim(void *p, int idx, int anim);
void PokedexApp_LoadMonIconResources(void *p, int a1);
void PokedexApp_CreateMonIconSprites(void *p, int a1);
void PokedexApp_DrawListIconGrid(void *p, int a1);
void PokedexApp_LoadMonPicResources(void *p);
void PokedexApp_CreateMonPicSprites(void *p, int a1);
void PokedexApp_ShowMonPic(void *p, int a1, int a2, int a3);
void PokedexApp_MoveSpriteToListCursor(void *p, int a1);
void PokedexApp_UpdateListPageArrows(void *p, int a1, int a2);
void PokedexApp_UpdateListScrollbarSize(void *p, int a1, int a2);
void PokedexApp_SyncListScrollbarToPage(void *p, int a1, int a2);
void PokedexApp_UpdateCaughtMarkSprite(void *p, int a1, int a2);
int PokedexApp_GetListCursorIndex(void *p);
u16 PokedexApp_GetCursorSpecies(void *p);
void *PokedexApp_LoadFootprintGraphics(int species, int a1);

#endif
