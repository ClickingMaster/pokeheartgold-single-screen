#pragma force_active on
#include "global.h"
extern u32 BattleAnimUtil_GetBattlerType(void*,int);int ov07_0221FF34(void*,int,int);
int ov07_0221FF34(void*w,int a,int b) {int result=1;u32 v=BattleAnimUtil_GetBattlerType(w,a);BattleAnimUtil_GetBattlerType(w,b);switch(v){case 0:break;case 2:break;case 4:break;case 1:result-=2;break;case 3:result-=2;break;case 5:result-=2;break;}return result;}
