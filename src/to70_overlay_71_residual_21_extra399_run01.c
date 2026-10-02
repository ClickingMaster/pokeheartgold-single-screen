#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct C{u8 pad[192];void*bg;}C;typedef struct W{C*ctx;u8 pad[336];void*bg;int y2,y6,speed,position;void**task;void*pending;}W;
extern void*Heap_Alloc(int,u32);extern void Heap_Free(void*);
extern void*SysTask_CreateOnVBlankQueue(void(*)(void*,W*),W*);extern int Bg_GetYpos(void*,int);extern void BgSetPosTextAndCommit(void*,int,int,int),ov71_02248BBC(C*,void**,void*);void ov71_02248D0C(C*,void**),ov71_02248D80(void*,W*);
void ov71_02248D0C(C*c,void**task){W*w=Heap_Alloc(57,368);if(w){w->ctx=c;w->bg=c->bg;w->y2=Bg_GetYpos(w->bg,2)<<12;w->y6=Bg_GetYpos(w->bg,6)<<12;w->speed=0;w->position=0;w->task=task;w->pending=0;*task=SysTask_CreateOnVBlankQueue(ov71_02248D80,w);if(*task==0)Heap_Free(w);}}
void ov71_02248D80(void*task,W*w){if(w->position<2097152){w->speed+=288;w->position+=w->speed;BgSetPosTextAndCommit(w->bg,2,3,(w->y2-w->position)>>12);BgSetPosTextAndCommit(w->bg,6,3,(w->y6-w->position)>>12);if(w->position>=524288&&w->pending==0)ov71_02248BBC(w->ctx,&w->pending,(u8*)w+4);}}
