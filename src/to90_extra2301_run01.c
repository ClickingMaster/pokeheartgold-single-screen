#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 state,count;u16 pad;int x,y,unkc,unk10,unk14,limit,bg,activeX,activeY;void*config,*task;u8 pad2[0xd4-0x30];s16 currentX,currentY;} Work;
extern void ov07_022227A8(void*,s16,s16,int,int);extern int ov07_022227D8(void*);extern void BgSetPosTextAndCommit(void*,u8,int,int),ov07_0221C448(void*,void*),Heap_Free(void*);
void ov07_02225DC4(void*,Work*);
void ov07_02225DC4(void *task,Work*w){switch(w->state){case 0:ov07_022227A8(&w->currentX,(s16)w->x,(s16)w->y,w->unkc,w->unk10);w->state++;case 1:if(!ov07_022227D8(&w->currentX)){if(w->count>=w->limit)w->state++;else w->state--;if(w->activeX)BgSetPosTextAndCommit(w->config,(u8)w->bg,0,0);if(w->activeY)BgSetPosTextAndCommit(w->config,(u8)w->bg,3,0);w->count++;}else{if(w->currentX){w->activeX=1;BgSetPosTextAndCommit(w->config,(u8)w->bg,0,w->currentX);}if(w->currentY){w->activeY=1;BgSetPosTextAndCommit(w->config,(u8)w->bg,3,w->currentY);}}break;default:ov07_0221C448(w->task,task);Heap_Free(w);break;}}
