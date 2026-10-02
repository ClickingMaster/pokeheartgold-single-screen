#pragma force_active on
#include "global.h"
extern s16 ov96_0221D6E4[],ov96_0221D6E6[];extern void ov96_021EB0A4(void*,int,int,int*,int*);typedef struct{int a,b,c;}Vec;typedef struct{u32 state:16,mode:4,index:2,other:5,flag:1,other2:1,flag2:1,flag3:1,last:1;}Flags;typedef struct{u32 a;void**obj;u8 gap[24];int x,y;u32 unused;int x2,y2;u8 gap2[16];Vec v;u8 gap3[16];Flags flags;}Work;
void ov96_02218A68(Work*w);
static inline void zeroVec(Vec*v,int x,int y,int z){v->a=x;v->b=y;v->c=z;}
void ov96_02218A68(Work*w){int x,y;ov96_021EB0A4(*w->obj,ov96_0221D6E4[w->flags.index*2],ov96_0221D6E6[w->flags.index*2],&x,&y);w->x2=x<<12;w->y2=y<<12;w->x=x<<12;w->y=y<<12;{Vec v={0,0,0};w->v=v;}w->flags.mode=2;w->flags.flag=0;w->flags.flag2=0;w->flags.flag3=0;}
