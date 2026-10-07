#pragma force_active on
#include "global.h"
#include "battle/battle.h"
#include "battle/battle_system.h"
#include "battle/battle_controller.h"
#pragma force_active on
typedef struct Copy {u8 command;u8 index:4,mimicked:4;s16 hp;u32 status,side;u16 item,moves[4];u8 pp[4],pad1a[2];u32 status2;u16 form,pad22;u32 ability;u16 switched,healed;} Copy;
extern void ov12_02262240(BattleSystem*,int,int,const void*,int);
void BattleController_EmitBattleMonToPartyMonCopy(BattleSystem*bs,BattleContext*ctx,int id){Copy buf;buf.command=39;buf.index=ctx->selectedMonIndex[id];buf.mimicked=(u8)ctx->battleMons[id].unk88.mimicedMoveIndex;buf.hp=ctx->battleMons[id].hp;buf.item=ctx->battleMons[id].item;buf.side=ctx->fieldSideConditionData[BattleSystem_GetFieldSide(bs,id)].battlerBitKnockedOffItem;buf.form=ctx->battleMons[id].form;buf.ability=ctx->battleMons[id].ability;for(int i=0;i<4;i++){buf.moves[i]=ctx->battleMons[id].moves[i];buf.pp[i]=ctx->battleMons[id].movePPCur[i];}if(buf.hp){buf.status=ctx->battleMons[id].status&0xfffff0ff;buf.status2=ctx->battleMons[id].status2;}else{buf.status=0;buf.status2=ctx->battleMons[id].status2;}if(ctx->battleStatus2&0x4000000){buf.healed=1;ctx->battleStatus2&=~0x4000000;}else buf.healed=0;if(ctx->battleStatus2&0x8000000){buf.switched=1;buf.healed=1;ctx->battleStatus2&=~0x8000000;}else buf.switched=0;ov12_02262240(bs,1,id,&buf,44);}
