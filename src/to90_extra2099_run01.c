#pragma force_active on
#include "global.h"
#pragma force_active on
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
extern u8 ov74_0223D454[];extern void*AddCharResObjFromNarc(void*,int,int,int,int,int,int),*AddPlttResObjFromNarc(void*,int,int,int,int,int,int,int),*AddCellOrAnimResObjFromNarc(void*,int,int,int,int,int,int);extern void SpriteTransfer_CreateCharTransferTask(void*),SpriteTransfer_CreatePlttTransferTask(void*),CreateSpriteResourcesHeader(void*,int,int,int,int,int,int,int,int,void*,void*,void*,void*,void*,void*),GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int),ov74_02235A74(void*),Main_SetVBlankIntrCB(void*,void*);void ov74_02235728(int,int,int,int,int,int);
#pragma opt_common_subs off
void ov74_02235728(int narc,int ch,int pal,int cell,int anim,int id){u8*w=ov74_0223D454;int screen=id==0?1:2;int compressed=1;if(narc==18)compressed=0;if(ch!=-1)P(w+id*24,0x178)=AddCharResObjFromNarc(P(w,0x160),narc,ch,compressed,id,screen,W(w,8));if(pal!=-1)P(w+id*24,0x17c)=AddPlttResObjFromNarc(P(w,0x164),narc,pal,0,id,screen,3,W(w,8));if(cell!=-1)P(w+id*24,0x180)=AddCellOrAnimResObjFromNarc(P(w,0x168),narc,cell,compressed,id,2,W(w,8));if(anim!=-1)P(w+id*24,0x184)=AddCellOrAnimResObjFromNarc(P(w,0x16c),narc,anim,compressed,id,3,W(w,8));int offset=id*24;SpriteTransfer_CreateCharTransferTask(P(w+offset,0x178));SpriteTransfer_CreatePlttTransferTask(P(w+offset,0x17c));CreateSpriteResourcesHeader(w+0x1a8+id*36,id,id,id,id,-1,-1,0,0,P(w,0x160),P(w,0x164),P(w,0x168),P(w,0x16c),0,0);if(id==0)GfGfx_EngineATogglePlanes(16,1);else GfGfx_EngineBTogglePlanes(16,1);Main_SetVBlankIntrCB(ov74_02235A74,0);}
