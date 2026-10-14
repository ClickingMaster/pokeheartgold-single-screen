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

u8 *PPWLobbyWebRequest_Delete(u8 *p) {
    LobbyCharVector_Destroy(p + 40);
    LobbyString_Destroy(p + 24);
    LobbyString_Destroy(p + 12);
    if (p) {
        DWC_Free(0, p, 0);
    }
    return p;
}
