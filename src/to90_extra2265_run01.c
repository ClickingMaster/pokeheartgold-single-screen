#pragma force_active on
#include "global.h"
#include "sys_task.h"
#pragma force_active on
SysTask*SysTask_CreateOnMainQueue(void(*)(SysTask*,void*),void*,u32);
typedef struct{u8 one,two;s16 hp;u16 max;u8 a;union{u8 raw;struct{u8 low:5,top:2,bit:1;}b;}flags;u32 f8,fc,f10;u8 last;} Input;
u32 BattleSystem_GetBattleType(void*);u8 BattleHpBar_Util_GetBarTypeFromBattlerSide(u8,u32);void BattleHpBar_SetEnabled(void*,BOOL);void ov12_0226498C(void*,u32,u32);void ov12_0225DA18(SysTask*,void*);void ov12_0225A414(void*,u8*,Input*);
#define B(p,o) (*(u8*)((p)+(o)))
#define W(p,o) (*(u32*)((p)+(o)))
void ov12_0225A414(void*system,u8*c,Input*d){u8*bar=c+0x28;MIi_CpuClearFast(0,(u32*)bar,1);W(bar,12)=(u32)system;B(bar,0x24)=B(c,0x194);B(bar,0x25)=BattleHpBar_Util_GetBarTypeFromBattlerSide(B(c,0x195),BattleSystem_GetBattleType(system));B(bar,0x4c)=d->one;W(bar,0x28)=d->hp;W(bar,0x2c)=d->max;B(bar,0x48)=d->two;B(bar,0x49)=d->flags.b.top;W(bar,0x30)=0;W(bar,0x38)=d->f8;W(bar,0x3c)=d->fc;B(bar,0x26)=d->a;B(bar,0x4a)=d->flags.b.low;B(bar,0x4b)=d->flags.b.bit;B(bar,0x4d)=d->last;B(bar,0x27)=d->f10;BattleHpBar_SetEnabled(bar,0);ov12_0226498C(bar,W(bar,0x28),-1);W(bar,16)=(u32)SysTask_CreateOnMainQueue(ov12_0225DA18,bar,1000);}
