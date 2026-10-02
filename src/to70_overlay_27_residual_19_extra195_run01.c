#pragma force_active on
#include "global.h"
#pragma force_active on
void *G2dRenderer_Init(int,void *,int);
void G2dRenderer_SetSubSurfaceCoords(void *,int,int);
void *Create2DGfxResObjMan(int,int,int);
void *AddCharResObjFromNarc(void *,int,int,int,int,int,int);
void *AddPlttResObjFromNarc(void *,int,int,int,int,int,int,int);
void *AddCellOrAnimResObjFromNarc(void *,int,int,int,int,int,int);
void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *),SpriteTransfer_CreatePlttTransferTask(void *);
void ov27_0225C72C(u8 *);
#define P(p,o) (*(void **)((p)+(o)))
void ov27_0225C72C(u8 *w){
 int i;u8 *p;
 P(w,0x218)=G2dRenderer_Init(21,w+0x21c,8);
 G2dRenderer_SetSubSurfaceCoords(w+0x21c,0,0x100000);
 for(i=0,p=w;i<4;i++,p+=4)P(p,0x344)=Create2DGfxResObjMan(10,i,8);
 P(w,0x354)=AddCharResObjFromNarc(P(w,0x344),239,12,0,10,2,8);
 P(w,0x358)=AddPlttResObjFromNarc(P(w,0x348),239,11,0,10,2,1,8);
 P(w,0x35c)=AddCellOrAnimResObjFromNarc(P(w,0x34c),239,13,0,10,2,8);
 P(w,0x360)=AddCellOrAnimResObjFromNarc(P(w,0x350),239,14,0,10,3,8);
 SpriteTransfer_CreateCharTransferTask_AllocAtEnd(P(w,0x354));SpriteTransfer_CreatePlttTransferTask(P(w,0x358));
}
