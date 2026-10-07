#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[0x14];VecFx32 a,b;u8 flags[12];u8 f38,f39,f3a,f3b,f3c,f3d,f3e,f3f,f40,f41,f42,f43,f44,f45;u16 f46;} Work;
void ov96_02213444(Work*);
void ov96_02213444(Work *w){VecFx32 zero={0,0,0};u8 i;w->a=zero;w->b=zero;w->f38=0;w->f39=3;w->f3a=0;w->f3b=12;w->f3c=0;w->f3d=0;w->f3e=0;w->f40=0;w->f41=0;w->f43=0;w->f45=0;w->f46=0;for(i=0;i<12;i++)w->flags[i]=0;}
