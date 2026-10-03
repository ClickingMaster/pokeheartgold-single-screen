#pragma force_active on
#pragma force_active on
#include "global.h"
#define I(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*((u8*)(p)+(o)))
#define V(p,o) (*(void**)((u8*)(p)+(o)))
extern void GetPokemonSpriteCharAndPlttNarcIds(void*,void*,int);extern void*PokepicManager_CreatePokepic(void*,void*,int,int,int,int,int,int);void ov59_0223C380(u8*,void**);
void ov59_0223C380(u8*p,void**mon){u32 info[4];GetPokemonSpriteCharAndPlttNarcIds(info,*mon,2);V(p,32)=PokepicManager_CreatePokepic(V(p,12),info,204,112,0,0,0,0);}
