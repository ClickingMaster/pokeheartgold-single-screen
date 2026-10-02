#pragma force_active on
#include "global.h"
#pragma force_active on
struct Inner{u8 gap[0x14];int*pc;int x;int plane1,start,plane2,x2,target,total;};struct S{u8 gap[0x40c];int result,next,frame;struct Inner*inner;};int abs(int);void G2x_SetBlendAlpha_(u32,int,int,int,int);extern int(*ov106_021E7044[])(struct S*,int*);extern int ov106_021E7090[];int ov106_021E68F4(struct S*);int ov106_021E6988(struct S*);
int ov106_021E6988(struct S*s){struct Inner*i=s->inner;while(1){int*pc=i->pc;int state=*pc;int r;if(state==19)return 1;r=ov106_021E7044[state](s,pc);if(r==0)break;if(r==1){i->pc+=ov106_021E7090[*i->pc];continue;}if(r==2){i->pc+=ov106_021E7090[*i->pc];break;}}return s->result;}
