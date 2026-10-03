#pragma force_active on
#include "global.h"
typedef struct Work { char gap0[12]; s16 dx,dy,x,y; u16 a,b; u8 state; char gap19[3]; void *ctx,*p20,*sprites,*p28,*bg,*p30,*s0,*s1,*s2; u8 timer; } Work;
extern void ov07_02231524(Work*,int),ov07_0221C448(void*,void*),ov07_02232508(Work*),G2x_SetBlendAlpha_(u32,int,int,int,int),BgClearTilemapBufferAndCommit(void*,int),ManagedSprite_SetDrawFlag(void*,int),BgSetPosTextAndCommit(void*,int,int,int),ManagedSprite_TickFrame(void*),SpriteSystem_DrawSprites(void*);
void ov07_022315C0(void*,Work*);
void ov07_022315C0(void*arg,Work*w) {
 switch(w->state) {
 case 0: ov07_02231524(w,2); ++w->state; break;
 case 1: G2x_SetBlendAlpha_(0x4000050,4,0x39,w->a,w->b); ++w->state; break;
 case 2: if(w->timer>20) ++w->state; else ++w->timer; break;
 case 3: if(w->a) --w->a; if(w->b<16) ++w->b;
 if(!w->a && w->b==16) { BgClearTilemapBufferAndCommit(w->bg,2); ManagedSprite_SetDrawFlag(w->s0,0); ManagedSprite_SetDrawFlag(w->s1,0); ManagedSprite_SetDrawFlag(w->s2,0); ++w->state; }
 *(vu16*)0x4000052=w->a|(w->b<<8); break;
 default: *(vu16*)0x400004a=(*(vu16*)0x400004a&~0x3f)|0x1f; *(vu16*)0x400004a=(*(vu16*)0x400004a&~0x3f00)|0x1f00; BgSetPosTextAndCommit(w->bg,2,0,0); BgSetPosTextAndCommit(w->bg,2,3,0); *(vu32*)0x4000000&=0xffff1fff; ov07_0221C448(w->ctx,arg); ov07_02232508(w); return;
 }
 w->x+=w->dx; w->y+=w->dy; BgSetPosTextAndCommit(w->bg,2,0,w->x); BgSetPosTextAndCommit(w->bg,2,3,w->y); ManagedSprite_TickFrame(w->s0); ManagedSprite_TickFrame(w->s1); ManagedSprite_TickFrame(w->s2); SpriteSystem_DrawSprites(w->sprites);
}
