#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct CanvasWork {u8 canvas[24];struct CanvasWork *self;void *context;u32 a,b;void *allocation,*task;} CanvasWork;
typedef struct State {u8 pad[0x610];CanvasWork canvas[2];} State;
extern State *ov13_0224F5E0;
extern const u16 ov13_02245950[],ov13_02245952[],ov13_0224594C[];
extern void *ov13_0224128C(u32,u32);
extern void NNS_G2dCharCanvasInitForBG(void *,void *,u32,u32,int);
extern void NNS_G2dMapScrToCharText(void *,u32,u32,u32,u32,u32,u32,u32);
extern void ov13_02240D80(CanvasWork *,int);
extern void ov13_02240A18(void *,void *);
extern void *ov13_02242528(int,void (*)(void *,void *),void *,int);
CanvasWork *ov13_022408A0(int,int);
CanvasWork *ov13_022408A0(int idx,int other){
 int h=ov13_02245952[idx*2],w=ov13_02245950[idx*2];
 CanvasWork *p=&ov13_0224F5E0->canvas[idx];
 void *screen;
 p->allocation=ov13_0224128C(w*h*32,32);
 if(idx==1){*(volatile u16 *)0x04001008 &=~0x40;*(volatile u16 *)0x04001008=(*(volatile u16 *)0x04001008 &0x43)|0xc00;}
 else{*(volatile u16 *)0x04000008 &=~0x40;*(volatile u16 *)0x04000008=(*(volatile u16 *)0x04000008 &0x43)|0xc00;}
 NNS_G2dCharCanvasInitForBG(p,p->allocation,w,h,4);
 { State *state=ov13_0224F5E0; int sub=idx==1; p->self=p;p->context=(u8 *)state+other*8;p->a=1;p->b=1;
 if(sub)screen=G2S_GetBG0ScrPtr();else screen=G2_GetBG0ScrPtr(); }
 NNS_G2dMapScrToCharText(screen,w,h,0,0,32,ov13_0224594C[idx],15);
 ov13_02240D80(p,0);
 p->task=ov13_02242528(1,ov13_02240A18,(u8 *)ov13_0224F5E0+0x67c+idx,200);
 return p;
}
