#pragma force_active on
#include "global.h"
typedef struct {u8 p[12];u32 pending;u8 q[0x24-16];void*bg;void*renderer;}Work;
extern void ov71_022474CC(void*),RequestSwap3DBuffers(int,int),BgSetPosTextAndCommit(void*,int,int,int),ov71_0224781C(void*,u16*);
void ov71_0224A8B0(Work*);
void ov71_0224A8B0(Work*w){u16 v[3];if(w->renderer){G3X_Reset();ov71_022474CC(w->renderer);RequestSwap3DBuffers(0,0);}if(w->pending){v[0]=0xeeb0;v[1]=0;v[2]=0;BgSetPosTextAndCommit(w->bg,3,3,256);BgSetPosTextAndCommit(w->bg,7,3,0);ov71_0224781C(w->renderer,v);*(volatile u16*)0x4000304&=0xffff7fff;w->pending=0;}}
