#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x78];int pos[3];u8 gap[0x1e0-0x84];int x,y,z,a,b,c,d,e,f;} Work;extern void sub_020181B0(void*,int*),sub_020182A8(void*,int,int,int),sub_020182C4(void*,int,int,int),sub_020182A0(void*,int);

void ov92_0225D9B4(Work*w,int mode);

void ov92_0225D9B4(Work*w,int mode){sub_020181B0(w,w->pos);sub_020182A8(w,0,-0x14000,0);if(mode==0)sub_020182C4(w,0x1000,0x1000,0x1000);else sub_020182C4(w,0x1800,0x1800,0x1800);sub_020182A0(w,1);w->x=0;w->y=-0x14000;w->z=0;w->e=0;w->f=0;w->a=0;w->b=1;}
