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

extern u8 *volatile gPPWLobbyWork;
extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
extern int PPWLobby_SendMessage();
void *PPWLobbyChannel_GetName(u8 *p, u8 *q);
int PPW_LobbySendChannelBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbySendPlayerBinaryMessage(int a, void *b, void *c, void *d);
int PPW_LobbyGetMyUserId(void);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);
void PPWLobby_GetChannelName(S *p, u8 *q, int x);

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
