#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct W31776 {u8 a[0xbf8];void *sprites;u8 renderer[0x128];void *man[4],*res[4];} W31776;
extern void *NARC_New(int,int),*G2dRenderer_Init(int,void *,int),*Create2DGfxResObjMan(int,int,int),*AddCharResObjFromOpenNarc(void *,void *,int,int,int,int,int),*AddPlttResObjFromOpenNarc(void *,void *,int,int,int,int,int,int),*AddCellOrAnimResObjFromOpenNarc(void *,void *,int,int,int,int,int);
extern void NARC_Delete(void *),NNS_G2dInitOamManagerModule(void),OamManager_Create(int,int,int,int,int,int,int,int,int),G2dRenderer_SetSubSurfaceCoords(void *,int,int),SpriteTransfer_CreateCharTransferTask(void *),SpriteTransfer_CreateExtPlttTransferTask(void *);
void ov73_021E8198(W31776 *);
void ov73_021E8198(W31776 *p){void *narc=NARC_New(100,150);int i;NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,150);p->sprites=G2dRenderer_Init(10,p->renderer,150);G2dRenderer_SetSubSurfaceCoords(p->renderer,0,1048576);for(i=0;i<4;i++)p->man[i]=Create2DGfxResObjMan(2,i,150);p->res[0]=AddCharResObjFromOpenNarc(p->man[0],narc,46,1,0,1,150);p->res[1]=AddPlttResObjFromOpenNarc(p->man[1],narc,10,0,0,1,3,150);p->res[2]=AddCellOrAnimResObjFromOpenNarc(p->man[2],narc,47,1,0,2,150);p->res[3]=AddCellOrAnimResObjFromOpenNarc(p->man[3],narc,48,1,0,3,150);SpriteTransfer_CreateCharTransferTask(p->res[0]);SpriteTransfer_CreateExtPlttTransferTask(p->res[1]);NARC_Delete(narc);}
