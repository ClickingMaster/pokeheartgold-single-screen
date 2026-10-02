#pragma force_active on
#include "global.h"
#pragma force_active on
extern u16 GetWindowBaseTile(void*);
extern void GF_CreateNewVramTransferTask(int,u32,void*,u32);

void ov48_0225A868(u8 *w,int index,int row,void *src);

typedef struct {u16 unused,kind;u32 pad;void *buffers[2];void *scrns[2];u16 count;u8 p[2];void *pal,*pltt;u8 flags,p1,slot,p2;}Entry;
typedef struct{u16 ids[2],count,palette,flags,slot;}Template;
typedef struct{u8 p[0x144];void*narc;}Work;
extern void *GfGfxLoader_GetScrnDataFromOpenNarc(void*,int,int,void**,int),*GfGfxLoader_GetPlttDataFromOpenNarc(void*,int,void**,int);
void ov48_0225A894(Entry*,Work*,Template*,int,int);
void ov48_0225A868(u8 *w,int index,int row,void *src){u16 tile=GetWindowBaseTile(w+0x168+index*16);tile+=row*27;GF_CreateNewVramTransferTask(21,tile*32,src,864);}
void ov48_0225A894(Entry*p,Work*w,Template*t,int kind,int heap){int i;memset(p,0,40);p->kind=kind;if(t->count){Template *cursor;void **out;Entry *dst;p->count=t->count;i=0;if(i<p->count){cursor=t;out=p->scrns;dst=p;do{dst->buffers[0]=GfGfxLoader_GetScrnDataFromOpenNarc(w->narc,cursor->ids[0],0,out,heap);out++;cursor=(Template*)((u8*)cursor+2);dst=(Entry*)((u8*)dst+4);i++;}while(i<p->count);}}if(t->flags){p->flags=t->flags;p->slot=t->slot;p->pal=GfGfxLoader_GetPlttDataFromOpenNarc(w->narc,t->palette,&p->pltt,heap);}}
