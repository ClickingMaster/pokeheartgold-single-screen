#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
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

extern u8 gPPWLobbyPlayersVtable[];
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int PPWLobbyPlayerSet_Destroy();
extern int LobbyCharVector_Destroy();
extern int PPWLobbyPlayers_Clear();
u8 *PPWLobbyPlayer_Destroy(u8 *p);
u8 *PPWLobbyPlayers_Destroy(u8 *p);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);

u8 *PPWLobbyPlayer_Destroy(u8 *p) {
    LobbyCharVector_Destroy(p + 24);
    return p;
}

u8 *PPWLobbyPlayers_Destroy(u8 *p) {
    P(p, 0) = gPPWLobbyPlayersVtable;
    PPWLobbyPlayers_Clear(p);
    LobbyCharVector_Destroy(p + 40);
    PPWLobbyPlayerSet_Destroy(p + 4);
    return p;
}
