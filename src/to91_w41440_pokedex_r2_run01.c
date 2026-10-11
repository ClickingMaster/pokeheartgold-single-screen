#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#pragma force_active on
extern const ManagedSpriteTemplate ov18_021FAC28[];
extern const u16 ov18_021FA348[];
void PokedexApp_ShowFormMonPics(void*,int,int);void PokedexApp_SetScrollbarSizeAnim(void*,int,int,const u16*,u32);
void PokedexApp_CreateFormsSprites(PokedexAppData*);
void PokedexApp_CreateFormsSprites(PokedexAppData*w){u32 i;for(i=1;i<=4;i++)w->sprites[i]=SpriteSystem_NewSpriteWithYOffset(w->spriteSystem,w->unk_066C,&ov18_021FAC28[i-1],FX32_CONST(512));for(i=5;i<20;i++)w->sprites[i]=SpriteSystem_NewSprite(w->spriteSystem,w->unk_066C,&ov18_021FAC28[i-1]);PokedexApp_SetSpriteVisible(w,9,0);PokedexApp_SetSpriteVisible(w,10,0);PokedexApp_SetSpriteVisible(w,11,0);PokedexApp_SetSpriteVisible(w,12,0);PokedexApp_SetSpriteVisible(w,13,0);PokedexApp_ShowFormMonPics(w,0,0);PokedexApp_DrawFormListIcons(w);PokedexApp_SetScrollbarSizeAnim(w,5,w->numSeenForms,ov18_021FA348,9);PokedexApp_SyncFormScrollbar(w);PokedexApp_UpdateScrollArrows(w,w->unk_18C5,w->numSeenForms,6);}
