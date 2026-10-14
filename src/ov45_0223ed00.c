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

extern void *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];
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

void LobbyChat_OnRaw(void) {
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
}

void LobbyChat_OnDisconnected(void) {
    int zero;
    void *p;
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
    zero = 0;
    LobbyTimerMap_EraseKey((u8 *)ov45_022577DC + 0x9c, &zero, 0);
    p = ov45_022577DC;
    if (W(p, 4) == 4) {
        if (!p) {
            return;
        }
        if (p) {
            LobbyChatManager_Destroy(p);
            if (p) {
                DWC_Free(0, p, 0);
            }
        }
        ov45_022577DC = 0;
        return;
    }
    W(p, 0x58) = 3;
    W(p, 4) = 5;
    W(ov45_022577DC, 0x60) = 1;
}
