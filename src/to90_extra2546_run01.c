#pragma force_active on
#include "global.h"
#include "battle/battle_system.h"
#include "sprite_system.h"
#include "pokepic.h"
#include "pokepic_animation_internal.h"
#include "heap.h"
#include "sys_task_api.h"
#include <string.h>
typedef struct {BattleSystem *battle;OpponentData *other;Pokepic *pic;UnkBattleSystemSub17C *sprite;u8 a10,who,state,side;s16 target;u16 value;u32 a18,kind;int timer;u32 a24,flag;u8 a2c;} Env;
typedef struct BattleSourceR1 {u16 unk0,unk2;u8 pad04[0x14];u16 values18[4];u8 values20[4],values24[4],values28[4];u32 values2C[4],values3C[4],mode4C,value50;}BattleSourceR1;
typedef struct {BattleSourceR1 source;u32 terrain;}EffectPacket;
typedef char check_env48[sizeof(Env)==48?1:-1];
typedef char check_source84[sizeof(BattleSourceR1)==84?1:-1];
typedef char check_packet88[sizeof(EffectPacket)==88?1:-1];
extern void ov12_02261F38(void*,int,int,void*,void*,int,int,int,int);
extern void BattleController_InitMoveAnimData(BattleSystem*,BattleContext*,void*,int,int,int,int,int);
extern void ov12_02261B80(BattleSystem*,u8*,void*,BattleSourceR1*);
extern int ov07_0221C394(u8*);
extern BOOL ov07_0221C3B0(void*),ov07_0221C3C0(void*);
extern void sub_02005B58(u8);
extern void BattleController_EmitCommandComplete(BattleSystem*,int,int);
void ov12_0225B494(SysTask*,void*);

#pragma force_active on
void ov12_0225B494(SysTask *task,void *data){Env *e=data;u32 *raw=ov12_0223A8DC(e->battle);u8 state=e->state;u8 *p=(u8*)raw;s16 xy[2];EffectPacket block;int target;
 switch(state){case 0:e->timer=28;e->state++;case 1:if(--e->timer)break;e->state++;case 2:
 if(e->side==2){ManagedSprite_GetPositionXY(e->sprite->unk0,&xy[1],&xy[0]);if(e->kind==1||e->kind==3){if(xy[1]<192)ManagedSprite_OffsetPositionXY(e->sprite->unk0,8,0);else ManagedSprite_SetPositionXY(e->sprite->unk0,192,88);}ManagedSprite_GetPositionXY(e->sprite->unk0,&xy[1],&xy[0]);if(e->kind==1)Pokepic_SetAttr(e->pic,0,xy[1]);else if(e->kind==3){xy[1]=Pokepic_GetAttr(e->pic,0)-xy[1];xy[1]-=24;Pokepic_AddAttr(e->pic,0,-xy[1]);}else if(e->kind==5){xy[1]=xy[1]-Pokepic_GetAttr(e->pic,0);xy[1]-=16;Pokepic_AddAttr(e->pic,0,xy[1]);}if(Pokepic_GetAttr(e->pic,0)<e->target)break;Pokepic_SetAttr(e->pic,44,0);Pokepic_SetAttr(e->pic,45,0);Pokepic_SetAttr(e->pic,0,e->target);ov12_02261F38(e->battle,e->who,e->kind,e->pic,e->other->unk1A4,e->value,e->a2c,e->side,e->a18);if(e->kind==1||e->kind==3)ManagedSprite_SetPositionXY(e->sprite->unk0,192,88);Pokepic_StartPaletteFade(e->pic,8,0,0,0);e->state++;return;}
 else {ManagedSprite_GetPositionXY(e->sprite->unk0,&xy[1],&xy[0]);if(e->kind==0||e->kind==2){if(xy[1]>64)ManagedSprite_OffsetPositionXY(e->sprite->unk0,-8,0);else ManagedSprite_SetPositionXY(e->sprite->unk0,64,136);}ManagedSprite_GetPositionXY(e->sprite->unk0,&xy[1],&xy[0]);if(e->kind==0)Pokepic_SetAttr(e->pic,0,xy[1]);else if(e->kind==2){xy[1]=xy[1]-Pokepic_GetAttr(e->pic,0);xy[1]-=24;Pokepic_AddAttr(e->pic,0,xy[1]);}else if(e->kind==4){xy[1]=Pokepic_GetAttr(e->pic,0)-xy[1];xy[1]-=16;Pokepic_AddAttr(e->pic,0,-xy[1]);}target=e->target;if(Pokepic_GetAttr(e->pic,0)>target)break;Pokepic_SetAttr(e->pic,0,target);ov12_02261F38(e->battle,e->who,e->kind,e->pic,e->other->unk1A4,e->value,e->a2c,e->side,e->a18);if(e->kind==0||e->kind==2)ManagedSprite_SetPositionXY(e->sprite->unk0,64,136);e->state++;return;}
 case 3:if(sub_02017068(ov12_0223B750(e->battle),e->who)==1&&!Pokepic_IsAnimFinished(e->pic)){if(e->flag){BattleController_InitMoveAnimData(e->battle,0,&block,1,11,e->who,e->who,0);ov12_02261B80(e->battle,(u8*)e->other,p,&block.source);e->state=4;}else e->state=255;}break;
 case 4:ov07_0221C394((u8*)p);if(!ov07_0221C3B0(p)){ov07_0221C3C0(p);e->state=255;}break;
 default:sub_02005B58(0);BattleController_EmitCommandComplete(e->battle,e->who,e->a10);Heap_Free(e);SysTask_Destroy(task);break;}}
