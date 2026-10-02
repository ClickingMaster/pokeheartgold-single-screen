#pragma force_active on
#include "global.h"
typedef struct{void*bg;u32 file,a,x,y;int p,q,r,s,t,u;}Config;
typedef struct{Config*c;int alt,delay,ticks,flag,heap;}Work;
extern void*GfGfxLoader_GetScrnData(u32,int,int,void**,int);extern void ov41_02249F7C(void*,int,void*,int,int,int,int,int,int),Heap_Free(void*);
void ov41_02249CF8(Work*,int);
void ov41_02249CF8(Work*w,int delta){w->ticks+=delta;if(w->ticks>w->delay){int file;void*data,*owner;Config*c;w->ticks=0;if(w->flag==0){file=w->alt;w->flag=1;}else{file=w->c->a;w->flag=0;}owner=GfGfxLoader_GetScrnData(w->c->file,file,0,&data,w->heap);c=w->c;ov41_02249F7C(c->bg,c->r,data,c->p,c->q,c->x,c->y,c->u,c->s);Heap_Free(owner);}}
