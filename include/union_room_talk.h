#ifndef POKEHEARTGOLD_UNION_ROOM_TALK_H
#define POKEHEARTGOLD_UNION_ROOM_TALK_H

#include "easy_chat.h"
#include "script.h"

// Union Room talk: talking to another player in the Union Room.

struct UnionRoom;
struct TrainerCardData;

enum UnionRoomActivityKind {
    UNION_ACTIVITY_NONE,
    UNION_ACTIVITY_DRAWING_RECRUIT,
    UNION_ACTIVITY_RECORD_MIX_RECRUIT,
    UNION_ACTIVITY_BUSY = 4,
    UNION_ACTIVITY_TRAINER_CARD,
    UNION_ACTIVITY_BATTLE,
    UNION_ACTIVITY_TRADE,
    UNION_ACTIVITY_DRAWING,
    UNION_ACTIVITY_RECORD_MIX,
    UNION_ACTIVITY_LINKED = 11,
    UNION_ACTIVITY_SPIN_TRADE,
    UNION_ACTIVITY_SPIN_TRADE_RECRUIT,
};

// message ids per gender of the talking player, and the Union Room's member slots per group
extern const int gUnionRoomTalkGreetingMsgs[2];
extern const int gUnionRoomTalkComeBackMsgs[2];
extern const int gUnionRoomTalkAnythingElseMsgs[2];
extern const int gUnionRoomTalkLeaveMsgs[2];
extern const int gUnionRoomTalkUntradeableEggMsgs[2];
extern const int gUnionRoomActivityBusyMsgs[2];
extern const int gUnionRoomActivityTradeMsgs[2][2];
extern const int gUnionRoomActivityTrainerCardMsgs[2][2];
extern const u16 gUnionRoomGroupFirstSlot[10];
extern const int gUnionRoomTalkJoinGroupMsgs[3][2];
extern const int gUnionRoomTalkJoinAcceptedMsgs[3][2];
extern const int gUnionRoomTalkJoinDeclinedMsgs[3][2];
extern const int gUnionRoomTalkRequirementMsgs[3][2];
extern const int gUnionRoomLanguageGreetings[7];
extern const int gUnionRoomActivityDrawingMsgs[4][2];
extern const int gUnionRoomActivityBattleMsgs[4][2];
extern const int gUnionRoomActivityRecordMixMsgs[4][2];
extern const int gUnionRoomActivitySpinTradeMsgs[4][2];
extern const int gUnionRoomTalkDeclineRequestMsgs[5][2];
extern const int gUnionRoomTalkAcceptRequestMsgs[8][2];

// script messages
BOOL UnionRoom_PartnerCancelled(struct UnionRoom *unionRoom);
int UnionRoom_GetBattleCancel(struct UnionRoom *unionRoom);
int UnionRoom_GetPartnerHometownMsg(MessageFormat *msgFmt);
int UnionRoom_GetTalkMsg(struct UnionRoom *unionRoom, int objId, int kind, MessageFormat *msgFmt);
u32 UnionRoom_GetPartnerGameVersion(void);
void UnionRoom_SetBeaconActivity(int activity);
int UnionRoom_GetOwnChatMessageMsg(struct UnionRoom *unionRoom, MessageFormat *msgFmt);
void UnionRoom_BufferPartnerLanguage(MessageFormat *msgFmt, u16 mode, u16 objId, PlayerProfile *profile, SaveEasyChat *easyChat);
void UnionRoom_ResetTalkReplies(struct UnionRoom *unionRoom);
void UnionRoom_SendBattleChoice(int choice);

// the player's own chat message
void UnionRoom_BroadcastChatMessage(MailMessage *message);
void UnionRoom_SetOwnChatMessage(struct UnionRoom *unionRoom, MailMessage *message);
MailMessage *UnionRoom_TakeOwnChatMessage(struct UnionRoom *);

// link command handlers (in the field link command table): (netId, size, data, fieldSystem)
void UnionRoom_RecvTalkChoice(int, int, u8 *, FieldSystem *);
void UnionRoom_RecvTalkCancel(int, int, u8 *, FieldSystem *);
void UnionRoom_RecvTrainerCard(int, int, void *);
struct TrainerCardData *UnionRoom_GetTrainerCardBuffer(int, FieldSystem *);
void UnionRoom_RecvBattleChoice(int, int, u8 *, FieldSystem *);

// trainer card exchange
struct TrainerCardData *UnionRoom_AllocTrainerCards(struct UnionRoom *);
void UnionRoom_FreeTrainerCards(struct UnionRoom *);
BOOL UnionRoom_SendTrainerCard(struct UnionRoom *);

// The profile in a player's beacon record (communication layer), NULL if absent.
PlayerProfile *CommWireless_GetParentProfile(int);

#endif // POKEHEARTGOLD_UNION_ROOM_TALK_H
