#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void ManagedSprite_SetPositionXY(void*,s16,s16);void ManagedSprite_SetDrawFlag(void*,int);
void PokedexApp_MoveSpriteToListCursor(u8*w,int index);
void PokedexApp_MoveSpriteToListCursor(u8*w,int index){int n=B(w,0x185a);void**sprites=(void**)(w+0x670);int rem=n%5;int row=n/5;ManagedSprite_SetPositionXY(sprites[index],rem*40+48,row*40+24);ManagedSprite_SetDrawFlag(sprites[index],1);}
