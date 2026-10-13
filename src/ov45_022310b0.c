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
extern void *ov45_02232B9C(Vec *);
extern Node *ov45_02232E30(u8 *, u32 *, Node *, Node *);
extern void ov45_02232DDC(Node **, u8 *, u32 *);
extern int DWC_Free();
extern int DWC_GetDateTime();
extern int ov45_02231144();
extern int ov45_0223325C();
extern int ov45_02233434();
extern int ov45_02236B88();
extern int ov45_02236CB0();
extern int ov45_022377AC();
extern int ov45_02237820();
extern int ov45_02237890();
extern int ov45_022379D4();
extern int LobbyString_InitFromCStr();
extern int LobbyString_Destroy();
extern int ov45_02238B70();
extern int ov45_0223DF4C();
extern int ov45_02240A74();
extern int ov45_02240A80();
extern int ov45_02240F9C();
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
void ov45_02232AE8(Record *p, int type, Body *q);
void ov45_02232B24(u8 *p, int n, char *fmt, ...);
int ov45_02232D8C(u8 *p, u32 *key);
void ov45_02232DDC(Node **out, u8 *p, u32 *key);
Node *ov45_02232E30(u8 *p, u32 *key, Node *n, Node *best);
int PPW_LobbyGetLastError(void);
int PPW_LobbyToErrorCode(int x);
int ov45_02233220(int x);
int PPW_LobbyShutdownAsync(void);
int PPW_LobbyStopRecruit(void);
int PPW_LobbyGetSubChannelState(void);
void ov45_02232434(S *p, u8 *q, int x);
int PPW_LobbyUpdateRecruitInfo(u8 *p);

u8 *PPWLobby_GetWork(void) {
    return _022577C4;
}
