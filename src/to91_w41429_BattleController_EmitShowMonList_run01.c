#include "global.h"
#include "battle/battle_controller.h"
#include "battle/overlay_12_0224E4FC.h"
#pragma force_active on
typedef struct {u8 cmd,battler,type,option;u8 flags[4];u8 mons[4][6];u32 data;u8 value;u8 reserved[3];} Packet;
void ov12_02262240(BattleSystem*,int,int,const void*,u8);
void BattleController_EmitShowMonList(BattleSystem *system,BattleContext *ctx,int battler,u32 type,u32 data,u32 option){Packet p;int i,j;BattleBuffer_Clear(ctx,battler);p.cmd=18;p.battler=battler;p.type=type;p.data=data;p.option=option;p.value=ctx->switchInFlag;for(i=0;i<4;i++){p.flags[i]=ctx->selectedMonIndex[i];for(j=0;j<6;j++)p.mons[i][j]=ctx->unk_312C[i][j];}ov12_02262240(system,1,battler,&p,40);}
