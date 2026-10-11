#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "overlay07_obj_window_work_private.h"
typedef Ov07ObjWindowWork Work;
void ov07_02231524(u8 *,int);
void BattleAnimSystem_DestroyTask(void *,void *);
void BattleAnimUtil_FreeWork(void *);
void ov07_022315C0(SysTask *arg,void *data) {
 Work *w=data;
 switch(w->prefix.state[0]) {
 case 0: ov07_02231524((u8 *)w,2); ++w->prefix.state[0]; break;
 case 1: G2x_SetBlendAlpha_(0x4000050,4,0x39,w->init.alphaA,w->init.alphaB); ++w->prefix.state[0]; break;
 case 2: if(w->timer>20) ++w->prefix.state[0]; else ++w->timer; break;
 case 3: if(w->init.alphaA) --w->init.alphaA; if(w->init.alphaB<16) ++w->init.alphaB;
 if(!w->init.alphaA && w->init.alphaB==16) { BgClearTilemapBufferAndCommit(w->prefix.field14,2); ManagedSprite_SetDrawFlag(w->sprites[0],0); ManagedSprite_SetDrawFlag(w->sprites[1],0); ManagedSprite_SetDrawFlag(w->sprites[2],0); ++w->prefix.state[0]; }
 *(vu16*)0x4000052=w->init.alphaA|(w->init.alphaB<<8); break;
 default: *(vu16*)0x400004a=(*(vu16*)0x400004a&~0x3f)|0x1f; *(vu16*)0x400004a=(*(vu16*)0x400004a&~0x3f00)|0x1f00; BgSetPosTextAndCommit(w->prefix.field14,2,BG_POS_OP_SET_X,0); BgSetPosTextAndCommit(w->prefix.field14,2,BG_POS_OP_SET_Y,0); *(vu32*)0x4000000&=0xffff1fff; BattleAnimSystem_DestroyTask(w->prefix.owner,arg); BattleAnimUtil_FreeWork(w); return;
 }
 w->init.x+=w->init.xVel; w->init.y+=w->init.yVel; BgSetPosTextAndCommit(w->prefix.field14,2,BG_POS_OP_SET_X,w->init.x); BgSetPosTextAndCommit(w->prefix.field14,2,BG_POS_OP_SET_Y,w->init.y); ManagedSprite_TickFrame(w->sprites[0]); ManagedSprite_TickFrame(w->sprites[1]); ManagedSprite_TickFrame(w->sprites[2]); SpriteSystem_DrawSprites(w->prefix.field0C);
}
