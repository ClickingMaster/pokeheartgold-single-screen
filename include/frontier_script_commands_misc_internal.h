#ifndef POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_MISC_INTERNAL_H
#define POKEHEARTGOLD_FRONTIER_SCRIPT_COMMANDS_MISC_INTERNAL_H

#include "global.h"

#include "frontier/frontier.h"
#include "frontier/frontier_script_context.h"

#include "launch_application.h"

u32 FrontierScript_ReadVar(FrontierScriptContext *ctx);
u16 *FrontierScript_ReadVarPtr(FrontierScriptContext *ctx);
u32 GF_GetCurrentPlayingBGM(void);
void StopBGM(u32 sequence, u32 fade);
void FrontierFieldSystem_Free(void *data);
u32 BattleTower_GetTrainerMessageBank(u32 mode);
void FrontierScript_PrintMessageEntryFromBank(FrontierScriptContext *ctx, void *data, u32 value);
void *BattleTower_NewBattleSetup(void *data, FrontierLaunchArgs *args);
void Sound_SetSceneAndPlayBGM(u32 scene, u32 sequence, u32 arg);
void BattleTowerSetup_SetLinkFlag(void *data, u32 value);
void GF_AssertFail(void);
u32 Comm_GetLocalNetId(void);
u32 LinkSync_SendPacket(u32 value, void *data);
void *LinkSync_GetPacket(u32 side);
u16 *FrontierScript_GetVarPtr(FrontierScriptContext *ctx, u32 value);
u32 BattleTowerSetup_IsLinkFlagSetByEither(void *data, void *remote);
extern const u16 ov80_0223C034[];
void ResetSystem(void);
u32 FrontierFieldSystem_AwardTowerBattlePoints(void *data);
u32 FrontierFieldSystem_GetFrontierBattleNumber(void *data);
u32 BattleTower_IncBattleNumber(void *data);
u32 FrontierFieldSystem_HasWonSet(void *data);
u32 Overlay80Counter_GetSumCapped(void *data);
u32 FrontierFieldSystem_GetBattleTowerMode(void *data);
void BattleTower_GenerateOpponents(void *data, void *saveData);
u32 BattleTower_GetTrainerSprite(void *data, u32 value);
void BattleTower_SetPalmerDefeated(void *data, u32 value);
void FrontierFieldSystem_RecordSetLost(void *data, void *saveData);
void FrontierFieldSystem_RecordSetWon(void *data, void *saveData);
void FrontierFieldSystem_SaveChallenge(void *data);

BOOL FrtCmd_StopBGM(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerAction(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerPrintTrainerMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerFreeData(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerStartBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerEndBattle(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerSendLinkMessage(FrontierScriptContext *ctx);
BOOL FrtCmd_TowerWaitLink(FrontierScriptContext *ctx);
u32 ov80_0223558C(void *data, u32 mode);
BOOL FrontierScriptNative_WaitPartnerPacket(FrontierScriptContext *ctx);
BOOL FrontierScript_ReadPartnerPacket(FrontierScriptContext *ctx, void *data, void *fieldSystem, u32 mode, u16 value);

#endif
