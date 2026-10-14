#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma force_active on
#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((u8 *)(p) + (o)))
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define P(p, o) (*(u8 **)((u8 *)(p) + (o)))

typedef union S {
    struct {
        u32 flag : 1;
        u32 cap : 31;
        u32 len;
        char *ptr;
    } l;

    struct {
        u8 flag : 1;
        u8 len : 7;
        char data[11];
    } s;
} S;

typedef struct Small {
    u8 x;
} Small;

typedef struct Vec {
    char *data;
    u32 len, cap;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

typedef struct Body {
    u64 a, b;
    u32 c, d;
} Body;

typedef struct Record {
    int type;
    u32 words[6];
} Record;

extern int DWC_CheckUserData(void *);
extern int DWC_CheckHasProfile(void *);
extern u8 *volatile gPPWLobbyWork;
extern u8 gPPWLobbyPlayersVtable[];
extern void *LobbyString_InitCopy(void *, void *);
extern void *LobbyCharVector_Data(Vec *);
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int DWC_Free();
extern int DWC_GetDateTime();
extern int PPWLobby_Initialize();
extern int PPWLobby_SendMessage();
extern int PPWLobby_SetChannelData();
extern int PPWLobbyPlayerSet_Destroy();
extern int LobbyTimerMap_Clear();
extern int LobbyCharVector_Grow();
extern int LobbyCharVector_Reallocate();
extern int LobbyCharVector_RecommendSize();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
extern int LobbyString_Destroy();
extern int PPWLobbySchedule_Copy();
extern int PPWLobbyPlayers_Clear();
extern int LobbyChat_ToErrorCode();
extern int LobbyChat_IsInChannel();
extern int LobbyChat_Shutdown();
u8 *PPWLobby_GetWork(void);
int PPW_LobbyInitializeAsync(void *a, void *b, void *c, void *d, u8 *user, void *e, void *g);
u8 *PPWLobbyPlayer_Destroy(u8 *p);
u8 *PPWLobbyPlayers_Destroy(u8 *p);
u8 *PPWLobbyWebRequest_Delete(u8 *p);
void *PPWLobby_GetChannelBaseName(u8 *p, u8 *q);
void *PPWLobbyChannel_GetName(u8 *p, u8 *q);
int PPW_LobbyGetTimeInfo(u64 *out);
int PPW_LobbyGetSchedule(void *p, u32 *size);
int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e);
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);
void LobbyCharVector_AssignRange(Vec *p, char *first, char *last);
void LobbyTimerMap_MakeEntry(Record *p, int type, Body *q);
void LobbyString_FormatN(u8 *p, int n, char *fmt, ...);
int LobbyTimerMap_EraseKey(u8 *p, u32 *key);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);
int PPW_LobbyGetLastError(void);
int PPW_LobbyToErrorCode(int x);
int ov45_02233220(int x);
int PPW_LobbyShutdownAsync(void);
int PPW_LobbyStopRecruit(void);
int PPW_LobbyGetSubChannelState(void);
void PPWLobby_GetChannelName(S *p, u8 *q, int x);
int PPW_LobbyUpdateRecruitInfo(u8 *p);

void PPWLobby_GetChannelName(S *p, u8 *q, int x) {
    volatile S copy;
    if (!x) {
        PPWLobbyChannel_GetName((u8 *)p, q);
        copy = *p;
        return;
    }
    PPWLobbyChannel_GetName((u8 *)p, q + 24);
    copy = *p;
}

int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d) {
    u8 *root = gPPWLobbyWork;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(gPPWLobbyWork, 0) != 8) {
            return 4;
        }
    }
    return PPWLobby_SendMessage(-1, a, 1, 1, b, c, d);
}

int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d) {
    u8 *root = gPPWLobbyWork;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(gPPWLobbyWork, 0) != 8) {
            return 4;
        }
    }
    return PPWLobby_SendMessage(a, 5, 1, 1, b, c, d);
}

int PPW_LobbyGetMyUserId(void) {
    u8 *p = gPPWLobbyWork;
    if (p) {
        return W(p, 76);
    }
    return -1;
}
