#pragma force_active on
#include "global.h"
#pragma force_active on
void ov87_021E7590(u8*,int,int);
struct S{u8 p[0x16c];u8 list[0x344-0x16c];void*sprites[2];u8 p34C[0x39d-0x34c];u8 frame,tick,p39F[2],mode,ids[2];};struct XY{s16 x,y;};extern const struct XY ov87_021E82E4[];void ov87_021E8078(void*,int);void ov87_021E7FD4(void*,int);void ov87_021E7FEC(void*,int,int);void ov87_021E7F28(void*,int);void ov87_021E7698(struct S*);
void ov87_021E7698(struct S*s){int i;s->tick++;if(s->tick>=1){s->tick=0;s->frame++;if(s->frame>=8)s->frame=0;if(s->mode==2){for(i=0;i<2;i++){ov87_021E8078(s->sprites[i],3);ov87_021E7FD4(s->sprites[i],1);ov87_021E7FEC(s->sprites[i],ov87_021E82E4[s->ids[i]].x-36,ov87_021E82E4[s->ids[i]].y-10);}}ov87_021E7F28(s->list,s->frame);}}
