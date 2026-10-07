#include "global.h"
#pragma force_active on

typedef struct {s32 x,y;} Point;
extern int sub_02020EB0(const void*,const void*,const void*,const void*);extern void VEC_MultAdd(fx32,const VecFx32*,const VecFx32*,VecFx32*);
int ov96_02213D2C(const VecFx32*,const VecFx32*,const VecFx32*,const VecFx32*,VecFx32*);
// @fn ov96_02213D2C
int ov96_02213D2C(const VecFx32*a,const VecFx32*b,const VecFx32*c,const VecFx32*d,VecFx32*out){VecFx32 p={0,0,0};VecFx32 dir;Point pa,pb,pc,pd;s32 n,m;pa.x=a->x/4096;pa.y=a->y/4096;pb.x=b->x/4096;pb.y=b->y/4096;pc.x=c->x/4096;pc.y=c->y/4096;pd.x=d->x/4096;pd.y=d->y/4096;if(!sub_02020EB0(&pa,&pb,&pc,&pd))return 0;VEC_Subtract(b,a,&dir);VEC_Normalize(&dir,&dir);if(a->y==b->y){n=(s32)((u32)a->y-(u32)c->y);if(n<0){u32 minus=0;minus=~minus;n=(s32)((u32)n*minus);};m=(s32)((u32)a->y-(u32)d->y);if(m<0){u32 minus=0;minus=~minus;m=(s32)((u32)m*minus);};if(!m)return 0;fx32 x=c->x;p.y=a->y;p.x=x;VEC_MultAdd(FX_Div(n,(s32)((u32)n+(u32)m)),&dir,&p,out);return 1;}if(a->x==b->x){n=(s32)((u32)a->x-(u32)c->x);if(n<0){u32 minus=0;minus=~minus;n=(s32)((u32)n*minus);};m=(s32)((u32)a->x-(u32)d->x);if(m<0){u32 minus=0;minus=~minus;m=(s32)((u32)m*minus);};if(!m)return 0;p.x=a->x;p.y=c->y;VEC_MultAdd(FX_Div(n,(s32)((u32)n+(u32)m)),&dir,&p,out);return 1;}GF_ASSERT(0);return 0;}
