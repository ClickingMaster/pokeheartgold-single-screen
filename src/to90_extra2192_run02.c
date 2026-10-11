#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
#define C(p,o) (*(s8*)((u8*)(p)+(o)))
extern const u8 ov18_021FA610[];extern void PokedexApp_LoadAreaSpriteResources(void*),PokedexApp_SetPlayerMarkerAnim(void*,int),PokedexApp_GetSeenFormAndGender(void*,int,u8*,u8*),PokedexApp_SetMonIconSub(void*,u16,u8,int),PokedexApp_UpdatePlayerMarker(void*),PokedexApp_SetupAreaListScrollbar(void*),PokedexApp_CreateAreaMarkerSprites(void*);extern void* PokedexApp_NewSprite(void*,const void*);
void PokedexApp_SetupAreaSprites(void*);
#pragma force_active on
void PokedexApp_SetupAreaSprites(void*w){u8 x,y;u32 i;PokedexApp_LoadAreaSpriteResources(w);for(i=1;i<=8;i++)P((u8*)w+i*4,0x670)=PokedexApp_NewSprite(w,ov18_021FA610+(i-1)*52);
 PokedexApp_SetPlayerMarkerAnim(w,1);PokedexApp_GetSeenFormAndGender(w,0,&x,&y);PokedexApp_SetMonIconSub(w,H(w,0x18a2),x,1);PokedexApp_UpdatePlayerMarker(w);PokedexApp_SetupAreaListScrollbar(w);PokedexApp_CreateAreaMarkerSprites(w);}
