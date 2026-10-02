#pragma force_active on
#include "global.h"
extern void Sprite_SetMatrix(void *,const VecFx32 *);
extern void *ov70_02246944;
void ov70_02238D8C(void *,int,int);void *ov70_02238DF8(void *,u32,int);void ov70_02238E20(void *,void *);
void ov70_02238D8C(void *sprite,int x,int y){VecFx32 v;float f;if(x>0)f=0.5f+(float)(x<<12);else f=(float)(x<<12)-0.5f;v.x=(int)f;if(y>0)f=0.5f+(float)(y<<12);else f=(float)(y<<12)-0.5f;v.y=(int)f;v.z=0;Sprite_SetMatrix(sprite,&v);}
void *ov70_02238DF8(void *unused,u32 size,int align){OSIntrMode state=OS_DisableInterrupts();void *p=NNS_FndAllocFromExpHeapEx(ov70_02246944,size,align);OS_RestoreInterrupts(state);return p;}
void ov70_02238E20(void *unused,void *p){if(p){OSIntrMode state=OS_DisableInterrupts();NNS_FndFreeToExpHeap(ov70_02246944,p);OS_RestoreInterrupts(state);}}
