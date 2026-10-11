#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void PokedexApp_DeleteSprite(void*,int);void SpriteManager_UnloadCharObjById(void*,int);void SpriteManager_UnloadPlttObjById(void*,int);void SpriteManager_UnloadCellObjById(void*,int);void SpriteManager_UnloadAnimObjById(void*,int);
void ov18_021F3448(u8*w);
void ov18_021F3448(u8*w){PokedexApp_DeleteSprite(w,26);PokedexApp_DeleteSprite(w,27);SpriteManager_UnloadCharObjById(P(w,0x66c),0xc59e);SpriteManager_UnloadPlttObjById(P(w,0x66c),0xc55f);SpriteManager_UnloadCellObjById(P(w,0x66c),0xc55c);SpriteManager_UnloadAnimObjById(P(w,0x66c),0xc55c);}
