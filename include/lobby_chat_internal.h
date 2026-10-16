#ifndef POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H
#define POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H

// Lobby chat (overlay 45): the chat manager, its string, map and request types, and the functions shared
// between the chat source files.

#include "global.h"

#include "dwc/net_common_internal.h"

#include "../lib/NitroDWC/src/tree-iterator4.h"

typedef union Str {
    struct {
        u32 isLong : 1;
        u32 pad : 31;
        u32 size;
        char *ptr;
    } l;

    struct {
        u8 isLong : 1;
        u8 len : 7;
        char buf[11];
    } s;
} Str;

typedef struct IntStrPair {
    int key;
    Str val;
} IntStrPair;

void LobbyString_InitCopy(void *dst, const void *src);

static inline void StrInit(Str *s) {
    u32 i;
    u32 *p;
    for (i = 0, p = (u32 *)s; i < 3; i++) {
        p[i] = 0;
    }
}

static inline void PairInit(IntStrPair *p, int *k, Str *v) {
    p->key = *k;
    LobbyString_InitCopy(&p->val, v);
}

static inline BOOL StrIsShort(Str *s) {
    return !s->l.isLong;
}

static inline u32 StrSize(Str *s) {
    return StrIsShort(s) ? s->s.len : s->l.size;
}

static inline const char *StrData(Str *s) {
    return StrIsShort(s) ? s->s.buf : s->l.ptr;
}

void LobbyString_Substr(void *a, void *b, u32 len, int d);
void LobbyTimerMap_Clear(void *p);
void LobbyTimerMap_Destroy(void *p);
struct Tree;
void LobbyRequestMap_DeleteAll(struct Tree *p);
void LobbyRequestMap_Destroy(void *p);
void LobbyChannelMap_Destroy(void *p);
void LobbyUserMap_Destroy(void *p);
void LobbyString_Destroy(void *p);

typedef struct Obj {
    void *vt;
    u32 id;
} Obj;

typedef struct Node {
    void *left;
    void *right;
    u32 parent;

    union {
        int i;
        Obj *obj;
        Str str;
    } key;
} Node;

typedef struct Tree {
    int count;
    Node head;
} Tree;

typedef Iterator Iter;

typedef struct InsertResult {
    Node *node;
    u8 inserted;
} InsertResult;

int LobbyString_Compare(Str *a, Str *b);
void LobbyUserIdSet_Find(Iter *out, Tree *t, int *key);

static inline BOOL StrLess(Str *a, Str *b) {
    return LobbyString_Compare(a, b) < 0;
}

typedef struct ObjKey {
    void *vt;
    u32 id;
    u32 param;
} ObjKey;

typedef struct VObj VObj;

typedef struct VTable {
    void (*f0)(VObj *);
    void (*dtor)(VObj *);
    void *(*f8)(VObj *);
} VTable;

struct VObj {
    VTable *vt;
};

typedef struct Vec {
    void *ptr;
    u32 size;
    u32 cap;
} Vec;

// What a channel reports to the code that entered it, with that code's parameter.
typedef struct LobbyChannelCallbacks {
    void (*onUserJoined)(const char *channel, int userId, void *param);
    void (*onUserParted)(const char *channel, int userId, void *param);
    void (*onChannelMessage)(const char *channel, int userId, void *message, void *param);
    void (*onBroadcastKeyChanged)(const char *channel, int userId, void *key, void *value, void *param);
    void (*onTopicChanged)(const char *channel, void *topic, void *param);
    void *param;
} LobbyChannelCallbacks;

typedef struct TreeHdr {
    int count;
    void *root;
    void *leftmost;
} TreeHdr;

typedef struct Entry {
    u32 a;
    TreeHdr t;
    u32 x10;
    LobbyChannelCallbacks callbacks;
} Entry;

typedef struct NamedEntry {
    Str name;
    Entry e;
} NamedEntry;

extern void *gLobbyRequestVtable;
u32 current_time(void);
void LobbyString_FormatN(char *buf, u32 size, const char *fmt, ...);
extern char sFmt_N_UnderscoreIntUnderscoreUint[];
extern char sStr___2[];
void LobbyString_InitFromCStr(Str *s, const char *cstr);
void LobbyChat_SplitString(Vec *out, Str *s, Str *sep, int n);
void LobbyStringVector_Destroy(Vec *v);
long atol(const char *s);
void LobbyUserIdSet_Assign(TreeHdr *dst, TreeHdr *src);
int LobbyChat_GetUserIdFromNick(Str *s);

typedef struct Tag {
    u8 x;
} Tag;

IntStrPair *LobbyUserMap_GetOrInsert(Tree *t, int *key);

// The chat session: the CHAT connection, the strings it logged in with and the user, channel, request and timer maps.
typedef struct LobbyChatManager {
    void *connection;
    int state;
    int userData;
    Str nick;
    Str user;
    Str realName;
    Str gameName;
    Str gameKey;
    Str channelPrefix;
    int unk54;
    int lastError;
    int connectedFrames;
    int disconnected;
    u8 unk64[0xC];
    u8 userMap[0x10];
    u8 channelMap[0x10];
    TreeHdr requestMap;
    u8 timerMap[0x10];
} LobbyChatManager;

int LobbyChat_GetFullChannelName(Str *out, LobbyChatManager *manager, Str *name);

void *LobbyMem_Alloc(u32 size);
int LobbyString_CompareCStr(Str *a, const char *b);
void *LobbyChatManager_Destroy(LobbyChatManager *manager);

#define CTX_CHECK(c)                      \
    if ((c) == NULL) {                    \
        return 4;                         \
    }                                     \
    if ((c) != NULL && (c)->state == 5) { \
        return 7;                         \
    }                                     \
    if ((c) != NULL && CtxState() != 3) { \
        return 4;                         \
    }

static inline void OpDelete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
}

static inline void TagTemp(void) {
    Tag t;
    t = (Tag) { 0 };
}

void LobbyU32Vector_PushBackSlow(Vec *v, u32 *val, Tag t);
Str *LobbyString_Concat(Str *out, Str *a, Str *b);

static inline BOOL StrEqC(Str *a, const char *b) {
    return LobbyString_CompareCStr(a, b) == 0;
}

extern LobbyChatManager *gLobbyChatManager;

static inline int CtxState(void) {
    return (*(LobbyChatManager *volatile *)&gLobbyChatManager)->state;
}

typedef void (*VoidFunc)(void);

extern struct {
    LobbyChatManager *manager;
    void *buf;
} ov45_022577E8;

void LobbyChat_StripChannelPrefix(Str *out, LobbyChatManager *manager, Str *name);
void LobbyChat_StripChannelPrefixes(Vec *out, LobbyChatManager *manager, int n, u32 *arr);
void LobbyChat_FormatNick(char *buf, int arg);

extern VoidFunc gLobbyMemOutOfMemoryHandler;
extern u8 ov45_02255228[];
extern char sStr_Empty_17[];
extern char sStr_normal[];
extern char sStr_voice[];
extern char sStr_ops[];
extern char sStr_VoiceOps[];
extern char sStr_invalid[];

void LobbyChannelMap_Find(Iter *out, Tree *t, Str *key);
Node *LobbyChannelMap_LowerBound(Tree *t, Str *key, Node *n, Node *res);
void LobbyChannelMap_MakeEntry(NamedEntry *dst, Str *name, Entry *src);
void LobbyChatManager_GetGameKey(Str *out, LobbyChatManager *manager);
int LobbyChat_AddChannelUser(LobbyChatManager *manager, Str *name, Str *key2);
void *LobbyChat_GetChannelCallbacks(LobbyChatManager *manager, Str *key);
void LobbyChat_GetUserNick(Str *out, LobbyChatManager *manager, int a);
int LobbyChat_LeaveChannel(const char *name);
void LobbyChat_RemoveChannel(LobbyChatManager *manager, Str *s);
BOOL LobbyChat_RemoveChannelUser(LobbyChatManager *manager, Str *name, Str *key2);
BOOL LobbyChat_RemoveUserIfUnused(LobbyChatManager *manager, int id);
int LobbyChat_SendChannelMessage(const char *name, const char *msg);
int LobbyChat_SendUserMessage(int a, const char *msg);
int LobbyChat_SetChannelKey(const char *name, int a, const char *key, const char *val);
int LobbyChat_SetChannelMode(const char *name, int arg);
int LobbyChat_Shutdown(void);
void *LobbyEnterChannelRequest_Delete(void *p);
void LobbyEnterChannelRequest_Destroy(void);
void *LobbyEnumChannelsRequest_Delete(void *p);
void LobbyEnumChannelsRequest_Destroy(void);
void *LobbyEnumUsersRequest_Delete(void *p);
void LobbyEnumUsersRequest_Destroy(void);
void *LobbyGetChannelKeysRequest_Delete(void *p);
void LobbyGetChannelKeysRequest_Destroy(void);
void *LobbyGetChannelModeRequest_Delete(void *p);
void LobbyGetChannelModeRequest_Destroy(void);
BOOL LobbyMem_InstallOutOfMemoryHandler(LobbyChatManager *manager);
void LobbyMem_OnOutOfMemory(void);
void LobbyMem_OnOutOfMemoryDisconnect(void);
void LobbyMem_ReleaseReserve(void);
BOOL LobbyRequestMap_DeleteKey(Tree *t, u32 id);
void LobbyRequestMap_Erase(Iter *out, Tree *t, Iter pos);
void LobbyRequestMap_Find(Iter *out, Tree *t, void *key);
Node *LobbyRequestMap_FindInsertPos(Tree *t, Obj **key, Node **pos, u8 *a, u8 *b);
void LobbyRequestMap_Insert(InsertResult *out, Tree *t, Obj **val);
Node *LobbyRequestMap_InsertNode(Tree *t, Node *parent, u8 a, u8 b, Obj **val);
void *LobbyRequest_Delete(void *p);
int LobbyRequest_GetId(u8 *a);
void LobbyString_Assign(Str *dst, Str *src);
void LobbyString_AssignCStr(Str *s, const char *cstr);
int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n);
Node *LobbyTree_EraseRebalance(Node *n, void *root);
BOOL LobbyUserIdSet_EraseKey(Tree *t, int *id);
void LobbyUserIdSet_Insert(InsertResult *out, Tree *t, int *key);
Node *LobbyUserIdSet_LowerBound(Tree *t, int *key, Node *node, Node *res);
void LobbyUserMap_Erase(Iter *out, Tree *t, Iter it);
BOOL LobbyUserMap_EraseKey(Tree *t, int *key);
void LobbyUserMap_Find(Iter *out, Tree *t, int *key);
Node *LobbyUserMap_FindInsertPos2(Tree *t, int *key, Node **pos, u8 *a, u8 *b);
void LobbyUserMap_Insert(InsertResult *out, Tree *t, const IntStrPair *pair);
Node *LobbyUserMap_InsertNode(Tree *t, Node *parent, u8 a, u8 b, IntStrPair *val);
Node *LobbyUserMap_LowerBound(Tree *t, int *key, Node *node, Node *res);
void LobbyUserMap_MakeEntry(IntStrPair *out, int id, const Str *nick);
void chatDisconnect(void *p);
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
int LobbyChat_IsInChannel(char *s);
void LobbyChat_OnBroadcastKeyChanged(void *a, char *s, char *name, void *arg, void *extra);
void LobbyChat_OnChannelMessage(void *a, char *s, char *name, void *arg, int mode);
void LobbyChat_OnChannelModeChanged(void);
void LobbyChat_OnConnected(void *a, int n);
void LobbyChat_OnDisconnected(void);
void LobbyChat_OnFillInUser(void *a, void *b, char *out);
void LobbyChat_OnInvited(void);
void LobbyChat_OnKicked(void *a, char *s);
void LobbyChat_OnRaw(void);
void LobbyChat_OnTopicChanged(void *a, char *s, void *b);
void LobbyChat_OnUserChangedNick(void *a, void *b, char *c, char *d);
void LobbyChat_OnUserListUpdated(void);
void LobbyChat_OnUserModeChanged(int a, int b, int c, int mode);
void LobbyChat_OnUserParted(int unused, char *name, char *value);
int LobbyChat_RenameUser(LobbyChatManager *manager, const Str *oldNick, const Str *newNick);
int LobbyChat_TimerConnectTimeout(void);
void *LobbyChat_ToErrorCode(void *p);
void LobbyTimerMap_EraseKey(void *map, int *key, int unused);
BOOL chatInChannelA(void *chat, const char *channel);

#endif // POKEHEARTGOLD_LOBBY_CHAT_INTERNAL_H
