#ifndef POKEHEARTGOLD_FRONTIER_FIELD_SYSTEM_H
#define POKEHEARTGOLD_FRONTIER_FIELD_SYSTEM_H

#include "field_system.h"
#include "scrcmd_9.h"

BOOL PartyIsValidForFrontier(u16 numRequired, SaveData *saveData, BOOL checkDuplicateItems);
void ResetSystem(void);
void FrontierFieldSystem_ClearSavedChallenge(u32 record);
u32 FrontierFieldSystem_GetSavedChallenge(u32 a0);
void FrontierFieldSystem_SetReturnWarp(FieldSystem *fieldSystem);
void FrontierFieldSystem_ClearReturnWarpFlag(FieldSystem *fieldSystem);
u32 FrontierFieldSystem_GetWinStreak(SaveData *saveData, u32 a1);
void sub_0204A730(SaveData *saveData, u8 a1);
u16 sub_0204A750(SaveData *saveData);
u32 FrontierFieldSystem_ForfeitSavedChallenge(SaveData *saveData);
u16 Save_BattleRoomDownload_HasTrainers(SaveData *saveData);
void FrontierFieldSystem_AssertCleared(FrontierFieldSystem **frontierFsys);
FrontierFieldSystem *FrontierFieldSystem_New(SaveData *saveData, BOOL resumeFromPrevious, u32 a2);
void FrontierFieldSystem_Free(FrontierFieldSystem *frontierFsys);
void FrontierFieldSystem_StartPartySelect(FrontierFieldSystem *frontierFsys, TaskManager *taskManager, void *runningApp);
BOOL FrontierFieldSystem_ReadPartySelection(FrontierFieldSystem *frontierFsys, void **runningApp, SaveData *saveData);
u32 FrontierFieldSystem_PartyHasDuplicateSpeciesOrItems(FrontierFieldSystem *frontierFsys, SaveData *saveData);
void FrontierFieldSystem_SetRandomFrontierTrainers(FrontierFieldSystem *frontierFsys, SaveData *saveData);
u8 FrontierFieldSystem_GetFrontierBattleNumber(FrontierFieldSystem *frontierFsys);
BOOL FrontierFieldSystem_HasWonSet(FrontierFieldSystem *frontierFsys);
void FrontierFieldSystem_RecordSetLost(FrontierFieldSystem *frontierFsys, SaveData *saveData);
void FrontierFieldSystem_RecordSetWon(FrontierFieldSystem *frontierFsys, SaveData *saveData);
void FrontierFieldSystem_SaveChallenge(FrontierFieldSystem *frontierFsys);
void FrontierFieldSystem_GenerateStatTrainers(FrontierFieldSystem *frontierFsys);
u32 FrontierFieldSystem_GetTrainerOverworld(FrontierFieldSystem *frontierFsys, u32 index);
u32 FrontierFieldSystem_GetBattleTowerMode(FrontierFieldSystem *frontierFsys);
u32 FrontierFieldSystem_GetPalmerDefeated(FrontierFieldSystem *frontierFsys);
u32 FrontierFieldSystem_AwardTowerBattlePoints(FrontierFieldSystem *frontierFsys);
BOOL FrontierFieldSystem_0204B0E0(FrontierFieldSystem *frontierFsys, SaveData *saveData);
u32 FrontierFieldSystem_WifiRankAction(FrontierFieldSystem *frontierFsys, SaveData *saveData, u32 action);
BOOL FrontierFieldSystem_TryGivePalmerRibbons(FrontierFieldSystem *frontierFsys, SaveData *saveData);
BOOL FrontierFieldSystem_TryGiveOtherTowerRibbons(FrontierFieldSystem *frontierFsys, SaveData *saveData);
u16 FrontierFieldSystem_InitChallengeRandom(FrontierFieldSystem *frontierFsys, SaveData *saveData);
u8 GetFrontierTrainerIVs(u32 frontierTrainerIndex);
u16 FrontierFieldSystem_NextRandom(FrontierFieldSystem *frontierFsys);

#endif // POKEHEARTGOLD_FRONTIER_FIELD_SYSTEM_H
