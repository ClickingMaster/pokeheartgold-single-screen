#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8 pad[0x185f];u8 flip:4,other:4;} Bits;
void ManagedSprite_SetDrawFlag(void*,int);int Pokedex_SpeciesGetLastSeenGender(void*,int,int);int Pokedex_GetSeenFormByIdx(void*,int,int);void PokedexApp_LoadMonPicToSprite(void*,int,u8,u8,int,int,int);
void PokedexApp_SwapMonPic(u8*w,int species,int index,int old);
void PokedexApp_SwapMonPic(u8*w,int species,int index,int old){ManagedSprite_SetDrawFlag(P(w+4*(index+((Bits*)w)->flip),0x670),0);((Bits*)w)->flip=(u8)(((Bits*)w)->flip^1);index+=((Bits*)w)->flip;
 if(!species){ManagedSprite_SetDrawFlag(P(w+4*old,0x670),1);ManagedSprite_SetDrawFlag(P(w+4*index,0x670),0);return;}
 ManagedSprite_SetDrawFlag(P(w+4*old,0x670),0);ManagedSprite_SetDrawFlag(P(w+4*index,0x670),1);
 int gender=Pokedex_SpeciesGetLastSeenGender(P(P(w,0),0),species,0);int form=Pokedex_GetSeenFormByIdx(P(P(w,0),0),species,0);if(species==172){if(form==2){form=1;gender=form;}else form=0;}
 PokedexApp_LoadMonPicToSprite(w,species,(u8)form,(u8)gender,2,index,0);
}
