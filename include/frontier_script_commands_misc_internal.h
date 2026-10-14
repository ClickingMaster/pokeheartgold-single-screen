#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_MISC_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_MISC_INTERNAL_H

#include "global.h"

u16 FrontierScriptContext_ReadHalfWord(void *ctx);
u32 FrontierScript_ReadVar(void *ctx);
u16 *FrontierScript_ReadVarPtr(void *ctx);
void FrontierScriptContext_Pause(void *ctx, BOOL (*callback)(void *));
void *Frontier_GetLaunchArgs(void *ctx);
void *Frontier_GetData(void *ctx);
u32 GF_GetCurrentPlayingBGM(void);
void StopBGM(u32 sequence, u32 fade);
void FrontierFieldSystem_Free(void *data);
u32 BattleTower_GetTrainerMessageBank(u32 mode);
void FrontierScript_PrintMessageEntryFromBank(void *ctx, void *data, u32 value);
void *BattleTower_NewBattleSetup(void *data, void *args);
void Sound_SetSceneAndPlayBGM(u32 scene, u32 sequence, u32 arg);
void Frontier_LaunchApplication(void *ctx, const void *template, void *args, u32 arg, u32 arg2);
extern const u8 gOverlayTemplate_Battle[];
u32 IsBattleResultWin(u32 result);
void BattleSetup_Delete(void *setup);
void BattleTowerSetup_SetLinkFlag(void *data, u32 value);
void GF_AssertFail(void);
u32 Comm_GetLocalNetId(void);
u32 LinkSync_SendPacket(u32 value, void *data);
void *LinkSync_GetPacket(u32 side);
u16 *FrontierScript_GetVarPtr(void *ctx, u32 value);
u32 BattleTowerSetup_IsLinkFlagSetByEither(void *data, void *remote);
extern const u16 ov80_0223C034[];
void ResetSystem(void);
u32 FrontierFieldSystem_AwardTowerBattlePoints(void *data);
void *Save_GameStats_Get(void *saveData);
void GameStats_Add(void *stats, u32 stat, u32 value);
u32 FrontierFieldSystem_GetFrontierBattleNumber(void *data);
u32 BattleTower_IncBattleNumber(void *data);
u32 FrontierFieldSystem_HasWonSet(void *data);
u32 ov80_022358E8(void *data);
u32 FrontierFieldSystem_GetBattleTowerMode(void *data);
void BattleTower_GenerateOpponents(void *data, void *saveData);
u32 BattleTower_GetTrainerSprite(void *data, u32 value);
void BattleTower_SetPalmerDefeated(void *data, u32 value);
void FrontierFieldSystem_RecordSetLost(void *data, void *saveData);
void FrontierFieldSystem_RecordSetWon(void *data, void *saveData);
void FrontierFieldSystem_SaveChallenge(void *data);

BOOL FrtCmd_StopBGM(void *ctx);
BOOL FrtCmd_TowerAction(void *ctx);
BOOL FrtCmd_TowerPrintTrainerMessage(void *ctx);
BOOL FrtCmd_TowerFreeData(void *ctx);
BOOL FrtCmd_TowerStartBattle(void *ctx);
BOOL FrtCmd_TowerEndBattle(void *ctx);
BOOL FrtCmd_TowerSendLinkMessage(void *ctx);
BOOL FrtCmd_TowerWaitLink(void *ctx);
u32 ov80_0223558C(void *data, u32 mode);
BOOL FrontierScriptNative_WaitPartnerPacket(void *ctx);
BOOL FrontierScript_ReadPartnerPacket(void *ctx, void *data, void *fieldSystem, u32 mode, u16 value);

#endif
