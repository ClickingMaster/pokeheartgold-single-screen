#pragma force_active on
#include "global.h"
#include "sys_task.h"
#pragma force_active on
extern int BattleSystem_GetMaxBattlers(void*),BattleSystem_GetCriticalHpMusicFlag(void*),BattleSystem_GetBattleType(void*),BattleSystem_GetBattleSpecial(void*),BattleSystem_GetFieldSide(void*,int),ov12_02261264(void*),CalculateHpBarColor(u16,u16,int),IsSEPlaying(u32);
extern void *BattleSystem_GetOpponentData(void*,int),*OpponentData_GetHpBar(void*);
extern void BattleSystem_SetCriticalHpMusicFlag(void*,int),BattleSystem_SetCriticalHpMusicDelay(void*,u8),StopSE(u16,int),PlaySE(u16);
extern u8 BattleSystem_GetCriticalHpMusicDelay(void*);
extern u32 MaskOfFlagNo(int);
void ov12_02239854(SysTask*,void*);
void ov12_02239854(SysTask*unused,void*b){int count=BattleSystem_GetMaxBattlers(b);u32 mask=0;int flag=BattleSystem_GetCriticalHpMusicFlag(b),i;if(BattleSystem_GetBattleType(b)&0x220)return;if(flag&2){if(flag&1){StopSE(0x704,0);BattleSystem_SetCriticalHpMusicFlag(b,2);}return;}for(i=0;i<count;i++){void*opponent=BattleSystem_GetOpponentData(b,i);if((!ov12_02261264(opponent)&&!(BattleSystem_GetBattleSpecial(b)&16))||(!BattleSystem_GetFieldSide(b,i)&&(BattleSystem_GetBattleSpecial(b)&16))){u8*bar=OpponentData_GetHpBar(opponent);if(bar&&CalculateHpBarColor((u16)*(u32*)(bar+40),(u16)*(u32*)(bar+44),48)==1)mask|=MaskOfFlagNo(i);}}
if(mask&&!BattleSystem_GetCriticalHpMusicFlag(b)){PlaySE(0x704);BattleSystem_SetCriticalHpMusicFlag(b,1);BattleSystem_SetCriticalHpMusicDelay(b,4);}else if(!mask&&BattleSystem_GetCriticalHpMusicFlag(b)){StopSE(0x704,0);BattleSystem_SetCriticalHpMusicFlag(b,0);}if(BattleSystem_GetCriticalHpMusicFlag(b)){int delay=BattleSystem_GetCriticalHpMusicDelay(b);if(!IsSEPlaying(0x704)){if(--delay==0){PlaySE(0x704);BattleSystem_SetCriticalHpMusicDelay(b,4);}else BattleSystem_SetCriticalHpMusicDelay(b,(u8)delay);}}}
