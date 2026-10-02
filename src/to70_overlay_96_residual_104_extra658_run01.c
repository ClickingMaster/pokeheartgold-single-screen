#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int a,heap;void *x,*y,*z,*q,*r;u8 flag;u8 pad[3];void *screen,*data;u8 windows[4][16];void *sprites[31];} State;
extern void *Heap_Alloc(int,u32);extern void Heap_Free(void *),ov96_02210538(State *),ov96_0221075C(State *),ov96_0221065C(State *),ov96_0221069C(State *),ov96_0221040C(State *,int),ov96_0221058C(State *,int),RemoveWindow(void *),Sprite_DeleteAndFreeResources(void *);
extern void *ov96_022104D8(void *,void *,int,int,int,int),*GfGfxLoader_GetScrnData(int,int,int,void *,int);
State *ov96_02210240(void *,void *,void *,void *,void *,u8,int,int);void ov96_022102D4(State *);
State *ov96_02210240(void *x,void *y,void *z,void *q,void *r,u8 flag,int heap,int a){State *s=Heap_Alloc(heap,232);MI_CpuFill8(s,0,232);s->x=x;s->y=y;s->flag=flag;s->heap=heap;s->z=z;s->q=q;s->r=r;s->a=a;ov96_02210538(s);s->sprites[0]=ov96_022104D8(s->x,s->y,210,76,8,0);ov96_0221075C(s);ov96_0221065C(s);ov96_0221069C(s);s->screen=GfGfxLoader_GetScrnData(237,14,1,&s->data,s->heap);ov96_0221040C(s,60);ov96_0221058C(s,0);return s;}
void ov96_022102D4(State *s){int i;GF_ASSERT(s);Heap_Free(s->screen);for(i=0;i<4;i++)RemoveWindow(s->windows[i]);for(i=0;i<31;i++){GF_ASSERT(s->sprites[i]);Sprite_DeleteAndFreeResources(s->sprites[i]);}Heap_Free(s);}
