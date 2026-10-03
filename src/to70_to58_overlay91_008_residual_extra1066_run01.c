#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x10];u8 count;u8 q[0x48-0x11];u32 num;void*data;u32 a[4],b[4];} Work;
extern void*GfGfxLoader_LoadFromNarc_GetSizeOut(int,int,int,int,int,u32*);extern u32 ov90_022588CC(void*,int),ov90_022588A4(void*,int);
void ov91_0225CBE4(Work*,int);
void ov91_0225CBE4(Work*w,int heap){u32 size;int i;Work*p;void*src;w->data=GfGfxLoader_LoadFromNarc_GetSizeOut(212,0,0,heap,0,&size);w->num=size/24;i=0;if(i<w->count){src=w;p=w;src=(u8*)src+8;do{p->a[0]=ov90_022588CC(src,i);p->b[0]=ov90_022588A4(src,i);i++;p=(Work*)((u8*)p+4);}while(i<w->count);}}
