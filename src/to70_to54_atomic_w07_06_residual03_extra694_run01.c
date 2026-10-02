#pragma force_active on
#include "global.h"
typedef struct {u32 a;u16 b,c;u32 d;u16 e,f;u32 g,h,i,j,k,l,gap[2],m,n,o;} Work;
extern u32 ov07_0221C4A8(void*,int);
extern void *ov07_0221C494(void*,u32);
extern u32 ov07_0221C478(void*),ov07_0221C468(void*),ov07_0221C470(void*);
void ov07_0222600C(void *p,Work *w);
void ov07_0222600C(void *p,Work *w){u32 v;w->a=ov07_0221C4A8(p,0);w->e=ov07_0221C4A8(p,1);w->f=ov07_0221C4A8(p,2);w->b=ov07_0221C4A8(p,3);w->c=ov07_0221C4A8(p,4);w->d=ov07_0221C4A8(p,5);w->g=ov07_0221C4A8(p,6);w->m=ov07_0221C4A8(p,8);v=ov07_0221C4A8(p,7);w->j=0;w->k=(v&0xffff0000)>>16;w->l=(u16)v;if(w->k==0)w->k=0;if(w->l==0)w->l=255;w->o=(u32)ov07_0221C494(p,w->a);w->n=ov07_0221C478(p);if(w->g==0){w->h=ov07_0221C468(p);w->i=ov07_0221C470(p);}else if(w->g==1){w->h=ov07_0221C470(p);w->i=ov07_0221C468(p);}if(w->o==0)GF_AssertFail();}
