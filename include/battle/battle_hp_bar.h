#ifndef POKEHEARTGOLD_BATTLE_HP_BAR_H
#define POKEHEARTGOLD_BATTLE_HP_BAR_H

#include "battle/battle.h"

void BattleHpBar_Draw(BattleHpBar *hpBar, u32 num, u32 flag);
void BattleHpBar_LoadResources(BattleHpBar *hpBar);
void BattleHpBar_FreeResources(BattleHpBar *hpBar);
void BattleHpBar_ToggleHpNumbers(BattleHpBar *hpBar);
void BattleHpBar_InitHpChange(BattleHpBar *hpBar, int hp);
void BattleHpBar_InitExpChange(BattleHpBar *hpBar, int exp);
int BattleHpBar_UpdateHpChange(BattleHpBar *hpBar);
int BattleHpBar_UpdateExpChange(BattleHpBar *hpBar);
void BattleHpBar_StartActiveEffect(BattleHpBar *hpBar);
void BattleHpBar_StopActiveEffect(BattleHpBar *hpBar);
void BattleHpBar_SetPriority(BattleHpBar *hpBar, int prio);
void BattleHpBar_SetEnabled(BattleHpBar *hpBar, BOOL enabled);
void BattleHpBar_StartSlide(BattleHpBar *hpBar, BOOL slideOut);
u8 BattleHpBar_Util_GetBarTypeFromBattlerSide(u8 bside, u32 battleType);
SysTask *BattleHpBar_BeginExpBarFullFlashEffect(BattleHpBar *hpBar, u8 *doneFlag);
void BattleHpBar_MenuOpenDummy(BattleHpBar *hpBar);
void BattleHpBar_MenuCloseDummy(BattleHpBar *hpBar);

#endif
