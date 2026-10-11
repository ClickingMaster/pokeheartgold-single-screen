#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "battle_controller_opponent_r2_exact_private.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "pokepic_anim_internal.h"
#include "heap.h"
#include "sys_task_api.h"
#pragma force_active on
typedef struct {BattleSystem *battle;OpponentData *ctx;Pokepic *pic;UnkBattleSystemSub17C *sprite;u8 id0,id,state,flag;s16 target;u16 attr;u32 x18;int side,timer;u32 x24;int enabled;u8 x2c;}Work;
typedef char WorkSize30[sizeof(Work)==48?1:-1];
extern void ov12_02261F38(BattleSystem*,int,int,Pokepic*,u32*,int,int,int,u32);
extern void BattleController_InitMoveAnimData(BattleSystem*,BattleContext*,void*,int,int,int,int,int);
extern int ov07_0221C394(u8*);
extern BOOL ov07_0221C3B0(void*),ov07_0221C3C0(void*);
extern void sub_02005B58(u8);
extern void BattleController_EmitCommandComplete(BattleSystem*,int,int);
void ov12_0225B7B8(SysTask*,void*);
void ov12_0225B7B8(SysTask *task,void *data){Work *w=data;BattleSourceR1 params;s16 pos[2];u32 *p=ov12_0223A8DC(w->battle);switch((int)w->state){
case 0:w->timer=28;w->state++;
case 1:if(--w->timer)break;w->state++;
case 2:ManagedSprite_GetPositionXY(w->sprite->unk0,&pos[1],&pos[0]);if(w->side==1||w->side==3){if(pos[1]<192)ManagedSprite_OffsetPositionXY(w->sprite->unk0,8,0);else ManagedSprite_SetPositionXY(w->sprite->unk0,192,88);}ManagedSprite_GetPositionXY(w->sprite->unk0,&pos[1],&pos[0]);Pokepic_AddAttr(w->pic,1,4);if(Pokepic_GetAttr(w->pic,1)<w->target)break;Pokepic_SetAttr(w->pic,44,0);Pokepic_SetAttr(w->pic,45,0);Pokepic_SetAttr(w->pic,1,w->target);ov12_02261F38(w->battle,w->id,w->side,w->pic,w->ctx->unk1A4,w->attr,w->x2c,w->flag,w->x18);ManagedSprite_SetPositionXY(w->sprite->unk0,192,88);Pokepic_StartPaletteFade(w->pic,8,0,0,0);w->state++;break;
case 3:if(PokepicAnimManager_GetAnimState((PokepicAnimManager*)ov12_0223B750(w->battle),w->id)!=1)break;if(Pokepic_IsAnimFinished(w->pic))break;if(w->enabled){BattleController_InitMoveAnimData(w->battle,0,&params,1,11,w->id,w->id,0);ov12_02261B80(w->battle,(u8*)w->ctx,p,&params);w->state=4;}else w->state=255;break;
case 4:ov07_0221C394((u8*)p);if(ov07_0221C3B0(p))break;ov07_0221C3C0(p);w->state=255;break;
default:sub_02005B58(0);BattleController_EmitCommandComplete(w->battle,w->id,w->id0);Heap_Free(w);SysTask_Destroy(task);break;
}}
