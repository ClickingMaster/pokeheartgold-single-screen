#pragma force_active on
#include "global.h"
#include <string.h>
static inline void zeroVec(VecFx32*p){p->x=0;p->y=0;p->z=0;}
void ov96_021F27B8(VecFx32*,VecFx32*,VecFx32*);
void ov96_021F27B8(VecFx32*a,VecFx32*b,VecFx32*out){VecFx32 normA,normB,zero;fx32 dot,mag;VEC_Normalize(a,&normA);VEC_Normalize(b,&normB);dot=VEC_DotProduct(&normA,&normB);mag=VEC_Mag(a);memset(&zero,0,sizeof(zero));VEC_MultAdd((fx32)(((s64)dot*mag+2048)>>12),&normB,&zero,out);}
