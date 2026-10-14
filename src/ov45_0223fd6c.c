#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef union {
    struct {
        u32 flag : 1;
        u32 pad : 31;
        u32 len;
        char *data;
    } b;

    struct {
        char head;
        char inlinebuf[11];
    } s;
} Str;

extern void *gLobbyChatManager;
extern char sStr_normal[];
extern char sStr_voice[];
extern char sStr_ops[];
extern char sStr_VoiceOps[];
extern char sStr_invalid[];
void LobbyString_InitFromCStr(Str *, char *);
void LobbyString_Destroy(Str *);
void LobbyString_InitCopy(void *, void *);
void LobbyU32Vector_Destroy(void *);
void LobbyTimerMap_EraseKey(void *, int *, int);
void LobbyChatManager_Destroy(void *);
void DWC_Free(int, void *, int);
void LobbyChat_RenameUser(void *, Str *, Str *);
void LobbyChat_RemoveChannel(void *, Str *);
void LobbyString_AssignCStr(Str *, char *);
void *LobbyChat_GetChannelCallbacks(void *, Str *);
void LobbyChat_StripChannelPrefix(Str *, void *, Str *);
void LobbyChat_GetFullChannelName(Str *, void *, Str *);
int chatInChannelA(void *, char *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void *LobbyMem_Alloc(unsigned);
void *LobbyU32Vector_RecommendSize(void *, unsigned, char *);
void LobbyU32Vector_Reallocate(void *, unsigned, void *);
char *strncpy(char *, const char *, size_t);
int LobbyChat_FindUserIdByNick(void *, Str *);
void *LobbyU32VectorBuffer_Destroy(void *p);
void LobbyU32VectorBuffer_Allocate(void *p, unsigned n);
void *LobbyU32VectorBuffer_Deallocate(void *p);
void LobbyChat_OnRaw(void);
void LobbyChat_OnInvited(void);
void LobbyChat_OnChannelModeChanged(void);
void LobbyChat_OnUserListUpdated(void);
void LobbyChat_OnDisconnected(void);
void LobbyChat_OnKicked(void *a, char *s);
void LobbyChat_OnUserChangedNick(void *a, void *b, char *c, char *d);
void LobbyChat_OnUserModeChanged(void *a, void *b, void *c, unsigned mode);
void LobbyChat_OnTopicChanged(void *a, char *s, void *b);
void LobbyChat_OnConnected(void *a, int n);
void LobbyChat_OnFillInUser(void *a, void *b, char *out);
void LobbyChatManager_GetUser(void *out, void *p);
void LobbyChatManager_GetNick(void *out, void *p);
void LobbyChatManager_GetGameName(void *out, void *p);
void LobbyChatManager_GetRealName(void *out, void *p);
int LobbyRequest_GetParam(void *p);
void *LobbyU32Vector_Data(void *p);
void LobbyRequest_Destroy(void);
void *LobbyRequestMap_LowerBound(void *c, void *key, void *node, void *best);
void LobbyRequestMap_Find(void **out, void *tree, void *key);
void LobbyChat_OnEnumChannelsEach(void *a, int n);
void LobbyU32Vector_AssignRange(void *p, u32 *first, u32 *last);
int LobbyChat_TimerConnectTimeout(void);
int LobbyChat_GetLastError(void);
void *LobbyChat_ToErrorCode(void *p);
int LobbyChat_IsInChannel(char *s);
void LobbyChat_OnChannelMessage(void *a, char *s, char *name, void *arg, int mode);
void LobbyChat_OnBroadcastKeyChanged(void *a, char *s, char *name, void *arg, void *extra);

int LobbyRequest_GetParam(void *p) {
    return W(p, 8);
}

void *LobbyU32Vector_Data(void *p) {
    if (W(p, 4) == 0) {
        return 0;
    }
    return P(p, 0);
}

void LobbyRequest_Destroy(void) {
}

void LobbyRequestMap_Find(void **out, void *tree, void *key) {
    void *p = LobbyRequestMap_LowerBound(tree, key, P(tree, 4), (u8 *)tree + 4);
    if (p == (u8 *)tree + 4 || U(P(key, 0), 4) < U(P(p, 12), 4)) {
        *out = (u8 *)tree + 4;
    } else {
        *out = p;
    }
}

void *LobbyRequestMap_LowerBound(void *c, void *key, void *node, void *best) {
    if (node) {
        unsigned n = U(P(key, 0), 4);
        do {
            if (U(P(node, 12), 4) >= n) {
                best = node;
                node = P(node, 0);
            } else {
                node = P(node, 4);
            }
        } while (node);
    }
    return best;
}

void LobbyChat_OnEnumChannelsEach(void *a, int n) {
    if (n) {
        return;
    }
}
