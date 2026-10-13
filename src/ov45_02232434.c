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
extern u8 *volatile _022577C4;
extern u8 ov45_02254FA0[];
extern void *LobbyString_InitCopy(void *, void *);
extern void *LobbyCharVector_Data(Vec *);
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int DWC_Free();
extern int DWC_GetDateTime();
extern int ov45_02231144();
extern int PPWLobby_SendMessage();
extern int PPWLobby_SetChannelData();
extern int ov45_02236B88();
extern int LobbyTimerMap_Clear();
extern int LobbyCharVector_Grow();
extern int ov45_02237820();
extern int ov45_02237890();
extern int LobbyCharVector_Destroy();
extern int LobbyString_InitFromCStr();
extern int LobbyString_Destroy();
extern int ov45_02238B70();
extern int ov45_0223DF4C();
extern int LobbyChat_ToErrorCode();
extern int LobbyChat_IsInChannel();
extern int LobbyChat_Shutdown();
u8 *PPWLobby_GetWork(void);
int PPW_LobbyInitializeAsync(void *a, void *b, void *c, void *d, u8 *user, void *e, void *g);
u8 *ov45_02231448(u8 *p);
u8 *ov45_02231460(u8 *p);
u8 *ov45_02231C38(u8 *p);
void *ov45_02231EB0(u8 *p, u8 *q);
void *ov45_022320B4(u8 *p, u8 *q);
int PPW_LobbyGetTimeInfo(u64 *out);
int PPW_LobbyGetSchedule(void *p, u32 *size);
int PPW_LobbySetChannelData(void *a, void *b, void *c, void *d, void *e);
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);
void ov45_02232760(Vec *p, char *first, char *last);
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
void ov45_02232434(S *p, u8 *q, int x);
int PPW_LobbyUpdateRecruitInfo(u8 *p);

void ov45_02232434(S *p, u8 *q, int x) {
    volatile S copy;
    if (!x) {
        ov45_022320B4((u8 *)p, q);
        copy = *p;
        return;
    }
    ov45_022320B4((u8 *)p, q + 24);
    copy = *p;
}

int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d) {
    u8 *root = _022577C4;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(_022577C4, 0) != 8) {
            return 4;
        }
    }
    return PPWLobby_SendMessage(-1, a, 1, 1, b, c, d);
}

int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d) {
    u8 *root = _022577C4;
    if (!root) {
        return 4;
    }
    if (root) {
        if (W(root, 0) == 10) {
            return 7;
        }
    }
    if (root) {
        if (W(_022577C4, 0) != 8) {
            return 4;
        }
    }
    return PPWLobby_SendMessage(a, 5, 1, 1, b, c, d);
}

int PPW_LobbyGetMyUserId(void) {
    u8 *p = _022577C4;
    if (p) {
        return W(p, 76);
    }
    return -1;
}
