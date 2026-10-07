#pragma force_active on
#include "global.h"
#pragma force_active on
int ov96_021E872C(int,int,int,int,int,int *);
int ov96_021E872C(int x,int y,int tx,int ty,int range,int *out){VecFx32 a,b;a.x=x<<12;a.y=y<<12;a.z=0;b.x=tx<<12;b.y=ty<<12;b.z=0;VecFx32 v;VEC_Subtract(&a,&b,&v);{int mag=VEC_Mag(&v);**(int*volatile*)&out=mag;if(mag<(*(volatile int*)&range<<12))return 1;return 0;}}
