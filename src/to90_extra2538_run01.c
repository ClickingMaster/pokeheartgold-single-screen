#pragma force_active on
#include "global.h"
typedef struct{int x,y,z;} Vec;
typedef struct{u8 p[24];u8 kind;} Inner;
typedef struct{u8 p[4];Inner*inner;} Outer;
typedef struct{u32 unk;Outer*outer;u8 p0[12];u32 state;u32 kind;s16 destination[2];Vec current,target;u8 p2[12];Vec velocity;int height;u32 unk54;struct{s8 timer58,timer59,timer5a,timer5b,timer5c,timer5d,timer5e,pad;}timers;union {u32 word;struct{u32 pad:27,b27:1,b28:1,b29:1,b30:1,b31:1;} b;} flags;} Work;
static inline Vec makeZero(void){Vec v={0,0,0};return v;}
extern s16 ov96_0221D7D0[5][10];extern u8 ov96_0221D69C[8];
extern void ov96_02218F18(Work*),ov96_022187A8(Work*,void*),ov96_02219030(Work*),ov96_02218F58(Work*),ov96_02218DF8(Work*,const s16*),ov96_02218578(Work*,int),ov96_02218A50(void**),ov96_02218FD4(Work*),ov96_02218EB8(Work*),ov96_02218AF0(Work*);extern BOOL ov96_02218934(Work*);void ov96_02218B1C(Work*,void*);
#pragma force_active on
void ov96_02218B1C(Work*w,void*context){if(w->timers.timer5d>0){int timer=w->timers.timer5d-1;w->timers.timer5d=timer;if(w->timers.timer5d>0)goto done;}switch(w->state){case 6:ov96_02218F18(w);break;case 5:case 7:case 8:case 10:case 11:case 12:break;default:ov96_022187A8(w,context);break;}switch(w->state){case 4:if(w->flags.b.b27)GF_AssertFail();if(ov96_02218934(w))ov96_02219030(w);else{if(w->flags.b.b29)ov96_02218F58(w);else if(w->flags.b.b30)ov96_02218DF8(w,w->destination);else{w->timers.timer5e--;if(w->timers.timer5e<=0)ov96_02218578(w,1);}}break;
case 3:w->timers.timer5b--;if(w->timers.timer5b<=0){ov96_02218A50((void**)w);ov96_02218578(w,4);}break;
case 2:if(ov96_02218934(w))ov96_02219030(w);else{w->timers.timer58--;if(w->timers.timer58<=0)ov96_02218578(w,1);}break;
case 1:if(ov96_02218934(w))ov96_02219030(w);else if(!(w->flags.b.b27)){if(w->flags.b.b29)ov96_02218F58(w);else if(w->flags.b.b30)ov96_02218DF8(w,w->destination);}break;
case 8:w->timers.timer5a--;if(w->timers.timer5a<=0){w->timers.timer5a=0;ov96_02218578(w,9);}break;
case 9:w->height=(s32)((u32)(s32)ov96_0221D7D0[w->outer->inner->kind][w->timers.timer5a]<<12);w->timers.timer5a++;if(w->timers.timer5a>=10){w->timers.timer5a=10;ov96_02218578(w,10);}break;
case 10:w->timers.timer5a--;if(w->timers.timer5a<=0){w->timers.timer5a=0;ov96_02218578(w,11);}break;
case 11:if(w->height==0){ov96_02218FD4(w);if(ov96_02218934(w))ov96_02219030(w);}else{w->height=(s32)((u32)w->height-((u32)ov96_0221D69C[w->outer->inner->kind]<<12));if(w->height<=0)w->height=0;}break;
case 12:w->timers.timer5a--;if(w->timers.timer5a<=0)ov96_02218578(w,1);break;
case 5:w->timers.timer59--;if(w->timers.timer59<=0){w->timers.timer59=6;ov96_02218578(w,6);}break;
case 6:w->timers.timer59--;if(w->timers.timer59<=0){Vec zero;w->current=w->target;zero=makeZero();w->velocity=zero;ov96_02218578(w,7);}break;
case 7:w->timers.timer59--;if(w->timers.timer59<=0)ov96_02218EB8(w);break;
default:GF_AssertFail();break;}ov96_02218AF0(w);done:;}
