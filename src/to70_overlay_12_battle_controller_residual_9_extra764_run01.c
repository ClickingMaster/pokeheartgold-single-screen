#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x64];u32 battler;u8 q[0x219c-0x68];u8 forms[4];u8 r[0x3044-0x21a0];u32 move;}Context;
typedef struct {u8 cmd,kind;u16 value;u32 a,b;}Message;
extern void BattleBuffer_Clear(void*,int),ov12_02262240(void*,int,int,void*,int);
void BattleController_EmitDrawYesNoBox(void*,void*,int,int,int,u32,u32),BattleController_EmitPrintAttackMessage(void*,Context*);
void BattleController_EmitDrawYesNoBox(void*sys,void*ctx,int battler,int value,int kind,u32 a,u32 b){Message m;BattleBuffer_Clear(ctx,battler);m.cmd=19;m.value=value;m.kind=kind;m.a=a;m.b=b;ov12_02262240(sys,1,battler,&m,12);}
void BattleController_EmitPrintAttackMessage(void*sys,Context*ctx){struct {u8 cmd,kind;u16 move;}m;m.cmd=20;m.kind=ctx->forms[ctx->battler];m.move=ctx->move;ov12_02262240(sys,1,ctx->battler,&m,4);}
