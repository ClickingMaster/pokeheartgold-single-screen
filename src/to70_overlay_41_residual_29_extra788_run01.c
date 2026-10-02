#pragma force_active on
#include "global.h"
typedef struct{u32 a,b,c,d,e,f,g,h;}Config;typedef struct{void*p;u8 dest[32];int count,limit;u32 b,*array,*c;u8*bytes;u8 gap[8];u32 first;}Work;extern void*ov41_02245EE0(Config*);extern void ov41_02246014(void*,int);extern void*ov41_022499F0(void*,void*,int);extern void ov41_02249A50(void*,void*);
int ov41_02248044(Work*w,int index,int x,int y,int arg);
int ov41_02248044(Work*w,int index,int x,int y,int arg){if(w->count<w->limit){Config c;void*p;c.a=w->first;c.b=w->b;c.c=w->array[index];c.d=*w->c;c.e=x;c.f=y;c.g=index;c.h=w->bytes[index];p=ov41_02245EE0(&c);ov41_02246014(p,arg);ov41_02249A50(ov41_022499F0(w->p,p,0),w->dest);w->count++;return 1;}return 0;}
