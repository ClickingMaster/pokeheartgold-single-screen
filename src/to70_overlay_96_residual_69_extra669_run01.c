#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int heap;u8 p[0x17c-4];void *renderer;u8 renderdata[0x2a8-0x180];void *man[6];void *res[4];} State;
extern void *G2dRenderer_Init(int,void *,int),*Create2DGfxResObjMan(int,int,int),*AddCharResObjFromNarc(void *,int,int,int,int,int,int),*AddPlttResObjFromNarc(void *,int,int,int,int,int,int,int),*AddCellOrAnimResObjFromNarc(void *,int,int,int,int,int,int);
extern void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *),SpriteTransfer_CreateExtPlttTransferTask(void *);
void ov96_021F8980(State *);
void ov96_021F8980(State *s){int i;s->renderer=G2dRenderer_Init(55,s->renderdata,s->heap);for(i=0;i<6;i++)s->man[i]=Create2DGfxResObjMan(1,i,s->heap);s->res[0]=AddCharResObjFromNarc(s->man[0],160,9,0,103,1,s->heap);s->res[1]=AddPlttResObjFromNarc(s->man[1],160,6,0,103,1,6,s->heap);s->res[2]=AddCellOrAnimResObjFromNarc(s->man[2],160,8,0,103,2,s->heap);s->res[3]=AddCellOrAnimResObjFromNarc(s->man[3],160,7,0,103,3,s->heap);SpriteTransfer_CreateCharTransferTask_AllocAtEnd(s->res[0]);SpriteTransfer_CreateExtPlttTransferTask(s->res[1]);}
