#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 pad[0x14];VecFx32 pos,normal;u8 pad2[0x40-0x2c];u8 scale;} Work;void ov96_022134D4(void*,Work*,const VecFx32*);
void ov96_022134D4(void*unused,Work*w,const VecFx32*v){VecFx32 zero={0,0,0};VEC_Normalize(v,&w->normal);w->pos=w->normal;VEC_MultAdd(w->scale<<12,&w->pos,&zero,&w->pos);}
