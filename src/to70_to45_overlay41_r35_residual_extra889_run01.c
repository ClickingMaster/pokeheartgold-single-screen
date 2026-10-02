#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x40];int index;u8 pad2[0x10];void*resource;u8 pad3[4];u8 manager[1];}Work;
typedef struct{void*res;int a,b,c,d,e,f,g,h,i,j,k;}Template;
extern void ov41_02249DB4(void*,Template*,int,int,int,int);
void ov41_022488D8(Work*,int,u32,int,int);
void ov41_022488D8(Work*w,int index,u32 flags,int x,int y){Template t;int a,b;t.res=w->resource;t.a=26;t.b=index*2+129;t.c=133;t.d=index*2+130;t.e=8;t.f=129;t.g=3;t.h=1;t.i=2;t.j=0;t.k=14;a=0;if(flags&1)a=112;if(flags&2)b=129;else b=0;ov41_02249DB4(w->manager,&t,a,b,x,y);w->index=index;}
