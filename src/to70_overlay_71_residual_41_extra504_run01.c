#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,int);extern void Heap_Free(void*);
extern void *SysTask_CreateOnVBlankQueue(void (*)(void*,void*),void*,int);
extern void BgSetPosTextAndCommit(void*,int,int,int);
extern void ov71_0224AFB8(void*);
extern void ov71_0224AF58(void*,void*);
typedef struct {void *parent,*bg;int count,x,y,speed,unk;void **out;int rest;} Job;

void ov71_0224AF08(void *p,void **out);

void ov71_0224AF08(void *p,void **out){Job*j=Heap_Alloc(57,36);if(j){j->parent=p;j->bg=*(void**)((u8*)p+12);j->x=-0x180000;j->y=-0xc0000;j->speed=0x9999;j->count=40;j->out=out;*out=SysTask_CreateOnVBlankQueue(ov71_0224AF58,j,0);if(*out==0)Heap_Free(j);}}
void ov71_0224AF58(void*t,void*p){Job*j=p;if(j->count){j->x+=j->speed;j->y+=j->speed;BgSetPosTextAndCommit(j->bg,2,3,j->x>>12);BgSetPosTextAndCommit(j->bg,6,3,j->y>>12);j->count--;}else{BgSetPosTextAndCommit(j->bg,2,3,0);BgSetPosTextAndCommit(j->bg,6,3,192);ov71_0224AFB8(t);}}
