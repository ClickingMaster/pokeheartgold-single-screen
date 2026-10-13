#ifndef POKEHEARTGOLD_UNK_02058AEC_H
#define POKEHEARTGOLD_UNK_02058AEC_H

#include "script.h"

void Colosseum_ShowOpponentTrainerCard(FieldSystem *fieldSystem);
struct UnionRoom *UnionRoom_Start(FieldSystem *fieldSystem);
void UnionRoom_Stop(FieldSystem *fieldSystem);
int UnionRoom_GetPlayerActivityType(struct UnionRoom *fieldSystem_unk80, int objId);
int UnionRoom_RequestJoinPlayer(struct UnionRoom *fieldSystem_unk80, int objId, u16 a2);
int UnionRoom_GetJoinStatus(struct UnionRoom *fieldSystem_unk80);
u32 UnionRoom_GetTalkChoice(struct UnionRoom *fieldSystem_unk80);
void UnionRoom_SendTalkResponse(struct UnionRoom *fieldSystem_unk80, int a1, int a2);
int UnionRoom_GetTalkReply(struct UnionRoom *fieldSystem_unk80);
int UnionRoom_TrySendTalkCancel(struct UnionRoom *fieldSystem_unk80, int a1);
void UnionRoom_RecvUnusedProfile(void);
void UnionRoom_RecvTalkReply(void);
void UnionRoom_RecvUnused(void);
void UnionRoom_RecvRestartSearch(void);
void Colosseum_RecvMixBattleMons(void);
u32 Colosseum_GetMixBattleMonsSize(void);
void Colosseum_GetMixBattleMonsBuffer(void);
void Colosseum_RecvMixBattleChoice(void);

#endif // POKEHEARTGOLD_UNK_02058AEC_H
