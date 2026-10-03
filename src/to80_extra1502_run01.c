#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void **)((u8 *)(p)+(o)))
#define U(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define B(p,o) (*((u8 *)(p)+(o)))
typedef struct {s32 frame;u32 x;void *data;} Anim; typedef struct {u32 x[2];Anim *anim;s32 frame;} Ctx;
void Field3dModelAnimation_FrameAdvanceAndLoop(Ctx *c,s32 speed);
void Field3dModelAnimation_FrameAdvanceAndLoop(Ctx *c,s32 speed){s32 max=(*(u16 *)((u8 *)c->anim->data+4))<<12;if(speed>0)c->frame=(c->frame+speed)%max;else{c->frame+=speed;if(c->frame<0)c->frame+=max;}c->anim->frame=c->frame;}
