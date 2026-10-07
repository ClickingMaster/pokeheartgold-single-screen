#pragma force_active on
#include "global.h"
#pragma force_active on
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
typedef struct{u8 pad[20];void*raw;} CharData;u32 GetMonIconNaixEx(u16,int,u16),GetMonIconPaletteEx(u16,u16,int);void*GfGfxLoader_GetCharDataFromOpenNarc(void*,u32,int,CharData**,u32);void ov96_021EED14(void*,void*,u32,int),Heap_Free(void*),ManagedSprite_SetPaletteOverride(void*,int),ov96_021EECB8(void*,const u16*,int,void*,int,u32);
void ov96_021EECB8(void*sprite,const u16*species,int palette,void*narc,int index,u32 heap){CharData*chars;void*raw=GfGfxLoader_GetCharDataFromOpenNarc(narc,GetMonIconNaixEx(species[0],0,species[1]),0,&chars,heap);GF_ASSERT(raw);ov96_021EED14(P(sprite,0),chars->raw,512,index);Heap_Free(raw);ManagedSprite_SetPaletteOverride(sprite,palette+GetMonIconPaletteEx(species[0],species[1],0));}
