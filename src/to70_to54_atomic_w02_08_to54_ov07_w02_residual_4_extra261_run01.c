#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{s16 x,y;int steps;u32 a;int ra;u32 b;int rb;int da,db;};int ov07_0222204C(int,int,int);void ov07_0222207C(struct S*,int,int,int);void ov07_022220B8(struct S*,int,int,int);void ov07_0222212C(struct S*,u32,u32,u32,u16,int,int,u16);int ov07_02222180(struct S*);int ov07_02222218(struct S*,int,int,int);int ov07_02222240(struct S*,int,int,int);
static inline int Mul(int a,int b){return(int)(((s64)a*b+2048)>>12);}
void ov07_0222212C(struct S*s,u32 a,u32 end,u32 b,u16 bend,int ra,int rb,u16 delta){s16 step;GF_ASSERT(s!=NULL);if(a>end)delta=-delta;step=(s16)delta;s->steps=ov07_0222204C(a<<12,end<<12,step<<12);s->a=a;s->ra=ra;s->b=b;s->rb=rb;s->da=step;s->db=((int)bend-(int)b)/s->steps;}
int ov07_02222180(struct S*s){GF_ASSERT(s!=NULL);if(s->steps!=0){s->a+=s->da;s->b+=s->db;s->a=(u16)s->a;s->b=(u16)s->b;s->steps--;s->x=Mul(FX_SinCosTable_[((int)s->a>>4)*2],s->ra)>>12;s->y=Mul(FX_SinCosTable_[((int)s->b>>4)*2+1],s->rb)>>12;return 1;}return 0;}
int ov07_02222218(struct S*s,int a,int b,int c){if(ov07_02222180(s)){ov07_0222207C(s,c,a,b);return 1;}return 0;}
int ov07_02222240(struct S*s,int a,int b,int c){if(ov07_02222180(s)){ov07_022220B8(s,c,a,b);return 1;}return 0;}
