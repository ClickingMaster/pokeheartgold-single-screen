#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*dummy,*bg,*narc;u32 heap;u8 pad[8];void*sprites[1];}Work;
typedef struct{int id,x,y;} Row;
extern int ov99_021E95D8[][2],ov99_021E95DC[][2];
extern Row ov99_021E9690[];
extern void GfGfx_EngineATogglePlanes(int,int),GfGfx_EngineBTogglePlanes(int,int);
extern void GfGfxLoader_LoadScrnDataFromOpenNarc(void*,int,void*,int,int,int,int,u32);
extern void BgSetPosTextAndCommit(void*,int,int,int);
extern void ManagedSprite_OffsetPositionXY(void*,int,int);

void ov99_021E5EB4(Work*w,int index,int mode);

void ov99_021E5EB4(Work*w,int index,int mode){int indexCopy=index;s16 offset;u8 i;if(mode)offset=-256;else offset=256;GfGfx_EngineATogglePlanes(4,0);GfGfx_EngineBTogglePlanes(2,0);GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc,ov99_021E95D8[indexCopy][0],w->bg,2,0,0,1,w->heap);GfGfxLoader_LoadScrnDataFromOpenNarc(w->narc,ov99_021E95DC[indexCopy][0],w->bg,5,0,0,1,w->heap);BgSetPosTextAndCommit(w->bg,2,0,offset);BgSetPosTextAndCommit(w->bg,5,0,offset);GfGfx_EngineATogglePlanes(4,1);GfGfx_EngineBTogglePlanes(2,1);offset=-offset;for(i=0;i<6;i++)ManagedSprite_OffsetPositionXY(w->sprites[ov99_021E9690[i].id],offset,0);}
