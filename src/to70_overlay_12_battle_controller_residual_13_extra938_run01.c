#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x219c];u8 ids[4];}Context;
extern int BattleSystem_GetMaxBattlers(void*);
extern void ov12_02262240(void*,int,int,void*,int);

void BattleController_EmitPrintFirstSendOutMessage(void*sys,Context*c,int who);

void BattleController_EmitPrintFirstSendOutMessage(void*sys,Context*c,int who){struct{u8 cmd,pad[3],ids[4];}msg;int i;msg.cmd=35;for(i=0;i<BattleSystem_GetMaxBattlers(sys);i++)msg.ids[i]=c->ids[i];ov12_02262240(sys,1,who,&msg,8);}
