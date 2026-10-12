#ifndef POKEHEARTGOLD_UNION_ROOM_TALK_H
#define POKEHEARTGOLD_UNION_ROOM_TALK_H

#include "easy_chat.h"
#include "script.h"

// Union Room talk (src/union_room_talk.c): talking to another player in the Union Room.
// Note: MWCC numbers function-local statics by the named parameters declared before them, so the declarations
// added after the matched includers were written leave their parameters unnamed.

struct UnkStruct_02059E1C;
struct TrainerCardData;

// script messages
BOOL UnionRoom_PartnerCancelled(struct UnkStruct_02059E1C *unionRoom);
int UnionRoom_GetBattleCancel(struct UnkStruct_02059E1C *unionRoom);
int UnionRoom_GetPartnerHometownMsg(MessageFormat *msgFmt);
int UnionRoom_GetTalkMsg(struct UnkStruct_02059E1C *unionRoom, int objId, int kind, MessageFormat *msgFmt);
u32 UnionRoom_GetPartnerGameVersion(void);
void UnionRoom_SetBeaconActivity(int activity);
int UnionRoom_GetOwnChatMessageMsg(struct UnkStruct_02059E1C *unionRoom, MessageFormat *msgFmt);
void UnionRoom_BufferPartnerLanguage(MessageFormat *msgFmt, u16 mode, u16 objId, PlayerProfile *profile, SaveEasyChat *easyChat);
void UnionRoom_ResetTalkReplies(struct UnkStruct_02059E1C *unionRoom);
void UnionRoom_SendBattleChoice(int choice);

// the player's own chat message
void UnionRoom_BroadcastChatMessage(MailMessage *message);
void UnionRoom_SetOwnChatMessage(struct UnkStruct_02059E1C *unionRoom, MailMessage *message);
MailMessage *UnionRoom_TakeOwnChatMessage(struct UnkStruct_02059E1C *);

// link command handlers (command table in unk_02091564.c): (netId, size, data, fieldSystem)
void UnionRoom_RecvTalkChoice(int, int, u8 *, FieldSystem *);
void UnionRoom_RecvTalkCancel(int, int, u8 *, FieldSystem *);
void UnionRoom_RecvTrainerCard(int, int, void *);
struct TrainerCardData *UnionRoom_GetTrainerCardBuffer(int, FieldSystem *);
void UnionRoom_RecvBattleChoice(int, int, u8 *, FieldSystem *);

// trainer card exchange
struct TrainerCardData *UnionRoom_AllocTrainerCards(struct UnkStruct_02059E1C *);
void UnionRoom_FreeTrainerCards(struct UnkStruct_02059E1C *);
BOOL UnionRoom_SendTrainerCard(struct UnkStruct_02059E1C *);

// The profile in a player's beacon record (communication layer, wireless_connection_state_8.c), NULL if absent.
PlayerProfile *CommWireless_GetParentProfile(int);

#endif // POKEHEARTGOLD_UNION_ROOM_TALK_H
