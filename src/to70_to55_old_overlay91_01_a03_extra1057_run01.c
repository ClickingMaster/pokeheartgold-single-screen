#pragma force_active on
#include "global.h"
typedef struct {void*list,*data;int x,y,z,sx,sy,sz;u16 animation;u16 pad;int priority,vram,heap;} Template;
typedef struct {u8 p[0x1c];void*list;u8 q[0x158-0x20];u8 data[4];} Work;
extern void ov91_0225D6B8(void*,Template*,Work*,int,int,int,int);
void ov91_0225D628(void*,Work*,int,int,int);
void ov91_0225D628(void*dst,Work*w,int a,int b,int heap){Template t;int i;t.list=w->list;t.data=w->data;t.sx=4096;t.sy=4096;t.sz=4096;t.animation=0;t.priority=0;t.vram=1;t.heap=heap;for(i=0;i<4;i++,dst=(u8*)dst+64)ov91_0225D6B8(dst,&t,w,a,i,b,heap);}
