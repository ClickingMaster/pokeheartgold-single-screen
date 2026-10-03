#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct State {u8 pad[0x38];void *task;u8 p3c[4];u16 total;u8 p42[15];u8 phase;u8 p52;u8 divisor;u8 p54;u8 delay;u8 p56;u8 active,notice;} State;
typedef struct Root {u16 p0,value;State *state;} Root;
extern Root ov13_0224F538;
extern int ov13_02239564(void);
extern void ov13_022272E4(void);
extern void ov13_02227D18(void);
extern int ov13_02239550(void);
extern void ov13_022307AC(void);
extern void ov13_022272CC(void);
extern void ov13_0223ABB8(int);
extern void ov13_02230CC4(void);
extern void ov13_02230B14(void *);
extern void ov13_02230BD0(void *);
extern void *ov13_02242528(int,void (*)(void *),int,int);
extern int FX_DivS32(int,int);
void ov13_022301A4(void);
void ov13_022301A4(void) {
 if(ov13_0224F538.state->task)return;
 if(ov13_0224F538.state->delay)ov13_0224F538.state->delay--;
 switch(ov13_02239564()) {
 case 0:break;
 case 1:ov13_0224F538.state->active=1;ov13_022272E4();break;
 case 2:
 if(ov13_0224F538.state->delay)break;
 ov13_02227D18();
 ov13_0224F538.value=FX_DivS32(ov13_0224F538.state->total*ov13_02239550(),ov13_0224F538.state->divisor);
 ov13_022307AC();ov13_0224F538.state->delay=4;break;
 case 3: {
 int mod;
 ov13_0224F538.state->active=0;ov13_022272CC();
 ov13_0224F538.value=FX_DivS32(ov13_0224F538.state->total*ov13_02239550(),ov13_0224F538.state->divisor);
 ov13_0223ABB8(19);ov13_022307AC();
 mod=FX_ModS32(ov13_0224F538.value,28);
 if(mod==0)ov13_02230CC4();
 else if(mod<14)ov13_0224F538.state->task=ov13_02242528(0,ov13_02230B14,0,120);
 else ov13_0224F538.state->task=ov13_02242528(0,ov13_02230BD0,0,120);
 break; }
 case 4:
 if(ov13_0224F538.value==0) {
 if(ov13_0224F538.state->notice==0){ov13_0223ABB8(9);ov13_0224F538.state->notice=1;}
 } else {ov13_0223ABB8(19);ov13_0224F538.state->task=ov13_02242528(0,ov13_02230B14,0,120);}break;
 case 6:
 if(ov13_0224F538.state->phase<=4 || ov13_0224F538.value==ov13_0224F538.state->total) {
 if(ov13_0224F538.state->notice==0){ov13_0223ABB8(9);ov13_0224F538.state->notice=1;}
 } else {ov13_0223ABB8(19);ov13_0224F538.state->task=ov13_02242528(0,ov13_02230BD0,0,120);}break;
 case 5:case 7:ov13_0224F538.state->notice=0;break;
 }
}
