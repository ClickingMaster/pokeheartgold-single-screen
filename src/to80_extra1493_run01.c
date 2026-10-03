#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
extern void *WaitingIcon_New(void *,int);extern void sub_0200F478(void *);
void ov74_02229E28(void *ctx,int a);
void ov74_02229E28(void *ctx,int a){struct {u8 pad[0x29f0];void *icon;} *c=ctx;if(a==1){if(c->icon==0)c->icon=WaitingIcon_New((u8 *)ctx+0x18,1);}else{if(c->icon)sub_0200F478(c->icon);c->icon=0;}}
