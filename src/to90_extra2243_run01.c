#pragma force_active on
#include "global.h"
#pragma force_active on
void NNS_G2dInitOamManagerModule(void);
void OamManager_Create(int,int,int,int,int,int,int,int,int);
void *G2dRenderer_Init(int,void *,int);
void G2dRenderer_SetSubSurfaceCoords(void *,int,int);
void *Create2DGfxResObjMan(int,int,int);
void *AddCharResObjFromOpenNarc(void *,void *,int,int,int,int,int);
void *AddPlttResObjFromOpenNarc(void *,void *,int,int,int,int,int,int);
void *AddCellOrAnimResObjFromOpenNarc(void *,void *,int,int,int,int,int);
void SpriteTransfer_CreateCharTransferTask(void *);
void SpriteTransfer_CreateExtPlttTransferTask(void *);
void ov52_021E870C(u8 *,void *);
#define P(o) (*(void **)(w+(o)))
void ov52_021E870C(u8 *w,void *narc){
 int i;u8 *cursor;
 NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,39);
 P(60)=G2dRenderer_Init(50,w+64,39);G2dRenderer_SetSubSurfaceCoords(w+64,0,1048576);
 i=0;cursor=w;do{*(void**)(cursor+360)=Create2DGfxResObjMan(2,i,39);cursor+=4;i++;}while(i<4);
 P(376)=AddCharResObjFromOpenNarc(P(360),narc,7,1,0,1,39);
 P(380)=AddPlttResObjFromOpenNarc(P(364),narc,1,0,0,1,3,39);
 P(384)=AddCellOrAnimResObjFromOpenNarc(P(368),narc,8,1,0,2,39);
 P(388)=AddCellOrAnimResObjFromOpenNarc(P(372),narc,9,1,0,3,39);
 P(392)=AddCharResObjFromOpenNarc(P(360),narc,7,1,1,2,39);
 P(396)=AddPlttResObjFromOpenNarc(P(364),narc,1,0,1,2,3,39);
 P(400)=AddCellOrAnimResObjFromOpenNarc(P(368),narc,8,1,1,2,39);
 P(404)=AddCellOrAnimResObjFromOpenNarc(P(372),narc,9,1,1,3,39);
 SpriteTransfer_CreateCharTransferTask(P(376));SpriteTransfer_CreateCharTransferTask(P(392));
 SpriteTransfer_CreateExtPlttTransferTask(P(380));SpriteTransfer_CreateExtPlttTransferTask(P(396));
}
