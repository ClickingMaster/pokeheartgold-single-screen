#pragma force_active on
#include "global.h"
extern void ov12_02262240(void*,int,int,void*,int);
typedef struct {u8 pad[0x2e4c];u32 damage,max;u8 q[0x3122-0x2e54];s16 hp;} Context;
typedef struct {u8 cmd,kind;u16 value;} Message;
void BattleController_EmitPrintReturnMessage(void*sys,Context*ctx,int battler,int kind);
void BattleController_EmitPrintSendOutMessage(void*sys,Context*ctx,int battler,int kind);
void BattleController_EmitPrintReturnMessage(void*sys,Context*ctx,int battler,int kind){Message m;m.cmd=0x20;m.kind=kind;m.value=(100*(ctx->hp-(s32)ctx->damage))/ctx->hp;ov12_02262240(sys,1,battler,&m,4);}
void BattleController_EmitPrintSendOutMessage(void*sys,Context*ctx,int battler,int kind){Message m;m.cmd=0x21;m.kind=kind;if(ctx->damage==0)m.value=1000;else m.value=1000*ctx->damage/ctx->max;ov12_02262240(sys,1,battler,&m,4);}
