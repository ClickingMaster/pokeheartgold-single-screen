#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Buffer{s16 left[192],right[192];u8 pad[0x300];u32 window;} Buffer;
typedef struct State{u32 pad,count,engine;} State;
extern Buffer *sub_02010EE0(State *,int);
void sub_02010C38(State *);
static inline void Apply(Buffer *p,int scan,State *ctx){
 u32 engine=ctx->engine;s16 right=p->right[scan],left=p->left[scan];
 if(!p->window){
  if(!engine){if(*(volatile u16 *)0x04000004&2){*(volatile u16 *)0x04000040=((left<<8)&0xff00)|(u8)right;*(volatile u16 *)0x04000044=192;}}
  else {if(*(volatile u16 *)0x04000004&2){*(volatile u16 *)0x04001040=((left<<8)&0xff00)|(u8)right;*(volatile u16 *)0x04001044=192;}}
 }else{
  if(!engine){if(*(volatile u16 *)0x04000004&2){*(volatile u16 *)0x04000042=((left<<8)&0xff00)|(u8)right;*(volatile u16 *)0x04000046=192;}}
  else {if(*(volatile u16 *)0x04000004&2){*(volatile u16 *)0x04001042=((left<<8)&0xff00)|(u8)right;*(volatile u16 *)0x04001046=192;}}
 }
}
void sub_02010C38(State *p){
 int scan;if(!p)GF_AssertFail();scan=*(volatile u16 *)0x04000006;
 if(scan<192){scan++;if(scan>191)scan-=192;
 if(p->count==1)Apply(sub_02010EE0(p,0),scan,p);
 else{Apply(sub_02010EE0(p,0),scan,p);Apply(sub_02010EE0(p,1),scan,p);}
 }
}
