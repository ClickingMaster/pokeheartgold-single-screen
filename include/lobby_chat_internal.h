#ifndef POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H
#define POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H

// Lobby chat: the chat manager, its string, map and request types, and the functions shared
// between the chat source files.

#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_chat_manager.h"
#include "lobby_vector.h"

#include "../lib/NitroDWC/src/tree-iterator4.h"

void LobbyString_InitCopy(void *dst, const void *src);

static inline void StrInit(Str *s) {
    u32 i;
    u32 *p;
    for (i = 0, p = (u32 *)s; i < 3; i++) {
        p[i] = 0;
    }
}

static inline void PairInit(LobbyUserMapEntry *p, int *k, Str *v) {
    p->userId = *k;
    LobbyString_InitCopy(&p->nick, v);
}

void LobbyString_Substr(void *a, void *b, u32 len, int d);
void LobbyRequestMap_DeleteAll(LobbyRequestMap *map);
void LobbyString_Destroy(void *p);

typedef Iterator Iter;

int LobbyString_Compare(Str *a, Str *b);
void LobbyUserIdSet_Find(Iter *out, LobbyUserIdSet *set, int *key);

static inline BOOL StrLess(Str *a, Str *b) {
    return LobbyString_Compare(a, b) < 0;
}

u32 current_time(void);
void LobbyString_FormatN(Str *out, u32 width, const char *fmt, ...);
extern char sFmt_N_UnderscoreIntUnderscoreUint[];
extern char sStr___2[];
void LobbyString_InitFromCStr(Str *s, const char *cstr);
void LobbyChat_SplitString(LobbyStringVector *out, Str *s, Str *sep, int n);
void LobbyStringVector_Destroy(LobbyStringVector *v);
long atol(const char *s);
int LobbyChat_GetUserIdFromNick(Str *s);

LobbyUserMapEntry *LobbyUserMap_GetOrInsert(LobbyUserMap *map, int *key);

// Results of the channel and message commands.
enum LobbyChatResult {
    LOBBY_CHAT_RESULT_OK = 0,
    LOBBY_CHAT_RESULT_INVALID_PARAM = 3,
    LOBBY_CHAT_RESULT_NOT_READY = 4,
    LOBBY_CHAT_RESULT_ERROR = 7,
};

int LobbyChat_GetFullChannelName(Str *out, LobbyChatManager *manager, Str *name);

void *LobbyMem_Alloc(u32 size);
int LobbyString_CompareCStr(Str *a, const char *b);
void *LobbyChatManager_Destroy(LobbyChatManager *manager);

// Commands need a session that is logged in and has not failed.
#define CTX_CHECK(c)                                           \
    if ((c) == NULL) {                                         \
        return LOBBY_CHAT_RESULT_NOT_READY;                    \
    }                                                          \
    if ((c) != NULL && (c)->state == LOBBY_CHAT_STATE_ERROR) { \
        return LOBBY_CHAT_RESULT_ERROR;                        \
    }                                                          \
    if ((c) != NULL && CtxState() != LOBBY_CHAT_STATE_READY) { \
        return LOBBY_CHAT_RESULT_NOT_READY;                    \
    }

static inline void OpDelete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
}

static inline void TagTemp(void) {
    LobbyTag t;
    t = (LobbyTag) { 0 };
}

void LobbyU32Vector_PushBackSlow(LobbyU32Vector *v, u32 *val, LobbyTag t);
void *LobbyU32Vector_Data(LobbyU32Vector *vector);
Str *LobbyString_Concat(Str *out, Str *a, Str *b);

static inline BOOL StrEqC(Str *a, const char *b) {
    return LobbyString_CompareCStr(a, b) == 0;
}

// The session pointer, read from memory again at every call.
static inline LobbyChatManager *CurrentManager(void) {
    return *(LobbyChatManager *volatile *)&gLobbyChatManager;
}

static inline int CtxState(void) {
    return CurrentManager()->state;
}

typedef void (*VoidFunc)(void);

extern struct {
    LobbyChatManager *manager;
    void *buf;
} gLobbyMemReserve;

void LobbyChat_StripChannelPrefix(Str *out, LobbyChatManager *manager, Str *name);
void LobbyChat_StripChannelPrefixes(LobbyU32Vector *out, LobbyChatManager *manager, int n, u32 *arr);
void LobbyChat_FormatNick(Str *out, int userId);

extern VoidFunc gLobbyMemOutOfMemoryHandler;
extern u8 sLobbyRequestMapInsertFlags[];
extern char sStr_Empty_17[];
extern char sStr_normal[];
extern char sStr_voice[];
extern char sStr_ops[];
extern char sStr_VoiceOps[];
extern char sStr_invalid[];

LobbyChannelMapNode *LobbyChannelMap_LowerBound(LobbyChannelMap *map, Str *key, LobbyChannelMapNode *n, LobbyChannelMapNode *res);
void LobbyChannelMap_MakeEntry(LobbyChannelMapEntry *dst, Str *name, LobbyChannel *src);
void LobbyChatManager_GetGameKey(Str *out, LobbyChatManager *manager);
int LobbyChat_AreChannelUsersEnumerated(LobbyChatManager *manager, Str *channel);
int LobbyChat_SetChannelUsersEnumerated(LobbyChatManager *manager, Str *channel);
int LobbyChat_GetChannelUserLimit(LobbyChatManager *manager, Str *channel);
int LobbyChat_AddChannelUser(LobbyChatManager *manager, Str *nick, Str *channel);
LobbyChannelCallbacks *LobbyChat_GetChannelCallbacks(LobbyChatManager *manager, Str *key);
void LobbyChat_GetUserNick(Str *out, LobbyChatManager *manager, int userId);
int LobbyChat_LeaveChannel(const char *channel);
void LobbyChat_RemoveChannel(LobbyChatManager *manager, Str *channel);
BOOL LobbyChat_RemoveChannelUser(LobbyChatManager *manager, Str *nick, Str *channel);
BOOL LobbyChat_RemoveUserIfUnused(LobbyChatManager *manager, int id);
int LobbyChat_SendChannelMessage(const char *channel, const char *message);
int LobbyChat_SendUserMessage(int userId, const char *message);
int LobbyChat_SetChannelKey(const char *channel, int userId, const char *key, const char *value);
int LobbyChat_SetChannelMode(const char *channel, int mode);
int LobbyChat_Shutdown(void);
LobbyRequest *LobbyEnterChannelRequest_Delete(LobbyRequest *request);
void LobbyEnterChannelRequest_Destroy(void);
LobbyRequest *LobbyEnumChannelsRequest_Delete(LobbyRequest *request);
void LobbyEnumChannelsRequest_Destroy(void);
LobbyRequest *LobbyEnumUsersRequest_Delete(LobbyRequest *request);
void LobbyEnumUsersRequest_Destroy(void);
LobbyRequest *LobbyGetChannelKeysRequest_Delete(LobbyRequest *request);
void LobbyGetChannelKeysRequest_Destroy(void);
LobbyRequest *LobbyGetChannelModeRequest_Delete(LobbyRequest *request);
void LobbyGetChannelModeRequest_Destroy(void);
BOOL LobbyMem_InstallOutOfMemoryHandler(LobbyChatManager *manager);
void LobbyMem_OnOutOfMemory(void);
void LobbyMem_OnOutOfMemoryDisconnect(void);
void LobbyMem_ReleaseReserve(void);
BOOL LobbyRequestMap_DeleteKey(LobbyRequestMap *map, u32 id);
void LobbyRequestMap_Erase(Iter *out, LobbyRequestMap *map, Iter pos);
void LobbyRequestMap_Find(Iter *out, LobbyRequestMap *map, LobbyRequest **key);
LobbyRequestMapNode *LobbyRequestMap_FindInsertPos(LobbyRequestMap *map, LobbyRequest **key, LobbyRequestMapNode **pos, u8 *insertLeft, u8 *isLeftmost);
void LobbyRequestMap_Insert(LobbyRequestMapInsertResult *out, LobbyRequestMap *map, LobbyRequest **val);
LobbyRequest *LobbyRequest_Delete(LobbyRequest *request);
void LobbyRequest_Destroy(void);
u32 LobbyRequest_GetParam(LobbyRequest *request);
u32 LobbyRequest_GetId(LobbyRequest *request);
void LobbyString_Assign(Str *dst, Str *src);
void LobbyString_AssignCStr(Str *s, const char *cstr);
int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n);
void *LobbyTree_EraseRebalance(void *node, void *root);
LobbyUserIdSetNode *LobbyUserIdSet_LowerBound(LobbyUserIdSet *set, int *key, LobbyUserIdSetNode *node, LobbyUserIdSetNode *res);
BOOL LobbyUserMap_EraseKey(LobbyUserMap *map, int *key);
void LobbyUserMap_Find(Iter *out, LobbyUserMap *map, int *key);
LobbyUserMapNode *LobbyUserMap_FindInsertPos2(LobbyUserMap *map, int *key, LobbyUserMapNode **pos, u8 *insertLeft, u8 *isLeftmost);
LobbyUserMapNode *LobbyUserMap_LowerBound(LobbyUserMap *map, int *key, LobbyUserMapNode *node, LobbyUserMapNode *res);
void LobbyUserMap_MakeEntry(LobbyUserMapEntry *out, int id, const Str *nick);
void chatDisconnect(void *p);
int chatGetChannelNumUsersA(void *chat, const char *channel);
void chatRegisterUniqueNickA(void *chat, int accountSpace, const char *nick, const char *cdKey);
void chatRetryWithNickA(void *chat, const char *nick);
void chatLeaveChannelA(void *h, const char *name, int x);
void chatSendChannelMessageA(void *h, const char *name, const char *msg, int x);
void chatSendUserMessageA(void *h, const char *name, const char *msg, int x);
void chatSetChannelKeysA(void *h, const char *name, const char *who, int x, const char **key, const char **val);
void chatSetChannelModeA(void *h, const char *name, int arg);

void LobbyChatManager_GetGameName(Str *out, LobbyChatManager *manager);
void LobbyChatManager_GetNick(Str *out, LobbyChatManager *manager);
void LobbyChatManager_GetRealName(Str *out, LobbyChatManager *manager);
void LobbyChatManager_GetUser(Str *out, LobbyChatManager *manager);
int LobbyChat_FindUserIdByNick(LobbyChatManager *manager, const Str *nick);
int LobbyChat_GetLastError(void);
int LobbyChat_IsInChannel(char *channel);
void LobbyChat_OnEnumChannelsEach(void *chat, int success);
void LobbyChat_OnBroadcastKeyChanged(void *chat, char *channel, char *nick, void *key, void *value);
void LobbyChat_OnChannelMessage(void *chat, char *channel, char *nick, void *message, int type);
void LobbyChat_OnChannelModeChanged(void);
void LobbyChat_OnConnected(void *chat, int success);
void LobbyChat_OnDisconnected(void);
void LobbyChat_OnFillInUser(void *chat, void *param, char *user);
void LobbyChat_OnInvited(void);
void LobbyChat_OnNickError(void *chat, int type);
void LobbyChat_OnKicked(void *chat, char *channel);
void LobbyChat_OnRaw(void);
void LobbyChat_OnTopicChanged(void *chat, char *channel, void *topic);
void LobbyChat_OnUserChangedNick(void *chat, void *channel, char *oldNick, char *newNick);
void LobbyChat_OnUserListUpdated(void);
void LobbyChat_OnUserJoined(void *chat, const char *channel, const char *nick);
void LobbyChat_OnUserModeChanged(int chat, int channel, int nick, int mode);
void LobbyChat_OnUserParted(int chat, char *channel, char *nick);
int LobbyChat_RenameUser(LobbyChatManager *manager, const Str *oldNick, const Str *newNick);
int LobbyChat_TimerConnectTimeout(void);
void *LobbyChat_ToErrorCode(void *p);
void LobbyTimerMap_EraseKey(LobbyTimerMap *map, int *key, int unused);
BOOL chatInChannelA(void *chat, const char *channel);

#endif // POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H
