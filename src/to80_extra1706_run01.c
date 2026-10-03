#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
extern void *NARC_New(int,int);extern void NARC_Delete(void*),NNS_G2dInitOamManagerModule(void),OamManager_Create(int,int,int,int,int,int,int,int,int);extern void *G2dRenderer_Init(int,void*,int),*Create2DGfxResObjMan(int,int,int);extern void G2dRenderer_SetSubSurfaceCoords(void*,fx32,fx32);extern void *AddCharResObjFromOpenNarc(void*,void*,int,int,int,int,int),*AddPlttResObjFromOpenNarc(void*,void*,int,int,int,int,int,int),*AddCellOrAnimResObjFromOpenNarc(void*,void*,int,int,int,int,int);extern void SpriteTransfer_CreateCharTransferTask(void*),SpriteTransfer_CreateExtPlttTransferTask(void*);
typedef struct{u8 pad[0xbf8];void*list;u8 renderer[0x128];void*manager[4];void*resource[12];}Ctx;
void ov72_02238194(u8*);
void ov72_02238194(u8*w){void*a=NARC_New(239,67),*b=NARC_New(238,67);int i;u8*cursor;NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,67);P(w,0xbf8)=G2dRenderer_Init(20,w+0xbfc,67);G2dRenderer_SetSubSurfaceCoords(w+0xbfc,0,256<<12);i=0;cursor=w;for(;i<4;i++)((Ctx*)w)->manager[i]=Create2DGfxResObjMan(3,i,67);
P(w,0xd34)=AddCharResObjFromOpenNarc(P(w,0xd24),b,1,0,0,2,67);P(w,0xd38)=AddPlttResObjFromOpenNarc(P(w,0xd28),b,0,0,0,2,3,67);P(w,0xd3c)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd2c),b,2,0,0,2,67);P(w,0xd40)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd30),b,3,0,0,3,67);
P(w,0xd44)=AddCharResObjFromOpenNarc(P(w,0xd24),b,5,0,1,2,67);P(w,0xd48)=AddPlttResObjFromOpenNarc(P(w,0xd28),b,4,0,1,2,3,67);P(w,0xd4c)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd2c),b,6,0,1,2,67);P(w,0xd50)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd30),b,7,0,1,3,67);
P(w,0xd54)=AddCharResObjFromOpenNarc(P(w,0xd24),a,12,0,2,2,67);P(w,0xd58)=AddPlttResObjFromOpenNarc(P(w,0xd28),a,11,0,2,2,1,67);P(w,0xd5c)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd2c),a,13,0,2,2,67);P(w,0xd60)=AddCellOrAnimResObjFromOpenNarc(P(w,0xd30),a,14,0,2,3,67);
SpriteTransfer_CreateCharTransferTask(P(w,0xd34));SpriteTransfer_CreateCharTransferTask(P(w,0xd44));SpriteTransfer_CreateCharTransferTask(P(w,0xd54));SpriteTransfer_CreateExtPlttTransferTask(P(w,0xd38));SpriteTransfer_CreateExtPlttTransferTask(P(w,0xd48));SpriteTransfer_CreateExtPlttTransferTask(P(w,0xd58));NARC_Delete(b);NARC_Delete(a);}
