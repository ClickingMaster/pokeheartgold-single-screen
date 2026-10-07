#pragma force_active on
#include "global.h"
#include "application/pokedex/pokedex_internal.h"
#pragma force_active on
extern const ManagedSpriteTemplate ov18_021FAC28[];
extern const u16 ov18_021FA348[];
void ov18_021F5EFC(void*,int,int);void ov18_021F61DC(void*,int,int,const u16*,u32);
void ov18_021F5E0C(PokedexAppData*);
void ov18_021F5E0C(PokedexAppData*w){u32 i;for(i=1;i<=4;i++)w->sprites[i]=SpriteSystem_NewSpriteWithYOffset(w->spriteSystem,w->unk_066C,&ov18_021FAC28[i-1],FX32_CONST(512));for(i=5;i<20;i++)w->sprites[i]=SpriteSystem_NewSprite(w->spriteSystem,w->unk_066C,&ov18_021FAC28[i-1]);ov18_021F11C0(w,9,0);ov18_021F11C0(w,10,0);ov18_021F11C0(w,11,0);ov18_021F11C0(w,12,0);ov18_021F11C0(w,13,0);ov18_021F5EFC(w,0,0);ov18_021F6038(w);ov18_021F61DC(w,5,w->numSeenForms,ov18_021FA348,9);ov18_021F65AC(w);ov18_021F619C(w,w->unk_18C5,w->numSeenForms,6);}
