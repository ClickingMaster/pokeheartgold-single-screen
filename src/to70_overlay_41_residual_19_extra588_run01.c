#pragma force_active on
#include "global.h"
typedef struct{void *a,*b,*c,*d,*e,*f,*g;int h,i,j;void *k;} Config;
extern void ov41_02248488(void *,Config *);extern void ov41_022487F8(void *,int,int);extern int ov41_02248ED4(void *,int);extern void ov41_022485DC(void *,int,int);extern int ov41_02248EE8(void *,int);extern void ov41_02248724(void *);
typedef struct {void *a,*b;u8 p8[8];void *c;u8 p14[28];void *d;u8 p34[12];void *e;u8 p44[0x140];u8 storage[0x1d8];u32 data[2];void *f;u8 output[64];} Work;
void ov41_02247334(Work *);
void ov41_02247334(Work *w){Config c;int i,j,n;c.a=w->a;c.b=w->b;c.c=w->c;c.d=w->d;c.e=w->e;c.k=w->storage;c.f=w->f;c.g=w->data;c.h=14;c.i=2;c.j=1;ov41_02248488(w->output,&c);ov41_022487F8(w->output,0,0);for(i=0;i<100;i++){n=ov41_02248ED4(w->storage,i);for(j=0;j<n;j++)ov41_022485DC(w->output,0,i);}for(i=0;i<18;i++){n=ov41_02248EE8(w->storage,i);if(n<18)ov41_022485DC(w->output,1,n);}ov41_02248724(w->output);}
