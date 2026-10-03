#pragma force_active on
#include "global.h"
#include "party.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "unk_02013FDC.h"
#include "heap.h"
typedef struct{u8 pad[16];ManagedSprite*sprite;}Sub;
extern int ov106_021E66B0(int,int,int);extern void ov106_021E6678(void*,u32,int);extern void ov106_021E6694(void*,u32,int);

void ov106_021E66FC(void*);
void ov106_021E66FC(void*w){Sub*s=*(Sub**)((u8*)w+0x418);Party*party=SaveArray_Party_Get(**(SaveData***)w);u16 gender,form;int shiny;NNSG3dResTex*tex;void*file;u8*src;void*buffer;u32 dest;u32 i;
 if(!Party_GetCount(party)){gender=2;form=0;shiny=0;}else{Pokemon*mon=Party_GetMonByIndex(party,0);if(GetMonData(mon,5,0)==493){gender=(u16)GetMonData(mon,111,0);form=(u16)GetMonData(mon,112,0);shiny=MonIsShiny(mon);}else{gender=2;form=0;shiny=0;}}
 file=AllocAndReadWholeNarcMemberByIdPair((NarcId)81,ov106_021E66B0(493,form,gender),(enum HeapID)153);tex=NNS_G3dGetTex(file);dest=NNS_G2dGetImageLocation(Sprite_GetImageProxy(s->sprite->sprite),NNS_G2D_VRAM_TYPE_2DMAIN);src=(u8*)tex+*(u32*)((u8*)tex+20);buffer=Heap_Alloc((enum HeapID)153,2048);
 for(i=0;i<8;i++){sub_020145B4(src,8,0,0,8,8,buffer);ov106_021E6678(buffer,dest,2048);src+=2048;dest+=2048;}Heap_Free(buffer);
 dest=NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(s->sprite->sprite),NNS_G2D_VRAM_TYPE_2DMAIN);src=(u8*)tex+*(u32*)((u8*)tex+56);if(shiny==1)src+=32;ov106_021E6694(src,dest,32);Heap_Free(file);}
