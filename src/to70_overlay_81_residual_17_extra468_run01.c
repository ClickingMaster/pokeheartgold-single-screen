#pragma force_active on
#include "global.h"
typedef struct {void *a,*b,*sprite;} W;
extern void *Heap_Alloc(int,u32),*ov81_02242A8C(void *,int,int,int,int,int);
extern void ov81_02242D94(W *,void *,void *),ov81_02242DAC(W *,void *,void *);
W *ov81_02242CBC(void *,int,int,int,int,void *,void *);W *ov81_02242D18(void *,int,int,int,int,void *,void *);
W *ov81_02242CBC(void *a,int b,int c,int d,int e,void *f,void *g){W *w=Heap_Alloc(100,12);((u8*)w)[0]=0;((u8*)w)[1]=0;((u8*)w)[2]=0;((u8*)w)[3]=0;((u8*)w)[4]=0;((u8*)w)[5]=0;((u8*)w)[6]=0;((u8*)w)[7]=0;((u8*)w)[8]=0;((u8*)w)[9]=0;((u8*)w)[10]=0;((u8*)w)[11]=0;w->a=f;w->b=g;w->sprite=ov81_02242A8C(a,b,c,e,d,0);ov81_02242D94(w,f,g);return w;}
W *ov81_02242D18(void *a,int b,int c,int d,int e,void *f,void *g){W *w=Heap_Alloc(100,12);((u8*)w)[0]=0;((u8*)w)[1]=0;((u8*)w)[2]=0;((u8*)w)[3]=0;((u8*)w)[4]=0;((u8*)w)[5]=0;((u8*)w)[6]=0;((u8*)w)[7]=0;((u8*)w)[8]=0;((u8*)w)[9]=0;((u8*)w)[10]=0;((u8*)w)[11]=0;w->a=f;w->b=g;w->sprite=ov81_02242A8C(a,b,c,e,d,1);ov81_02242DAC(w,f,g);return w;}
