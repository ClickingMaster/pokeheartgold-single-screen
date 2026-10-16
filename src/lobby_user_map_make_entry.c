#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))

typedef struct Node {
    void *left, *right;
    u32 parent;
    int key;
} Node;

typedef struct {
    int count;
    void *root, *last;
} Tree;

extern void LobbyString_InitCopy(void *, void *);
extern void LobbyChannelMap_Find(void **, void *, void *);
extern void LobbyUserMap_MakeEntry(void *p, int a, void *b);
extern int LobbyChat_AreChannelUsersEnumerated(void *p, void *key);
extern int LobbyChat_SetChannelUsersEnumerated(void *p, void *key);
extern int LobbyChat_GetChannelUserLimit(void *p, void *key);

typedef struct {
    Node *node;
    u8 ok;
} InsertResult;

void LobbyUserMap_MakeEntry(void *p, int a, void *b) {
    W(p, 0) = a;
    LobbyString_InitCopy(A(p, 4), b);
}

int LobbyChat_AreChannelUsersEnumerated(void *p, void *key) {
    void *n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n == A(p, 0x84)) {
        return 0;
    }
    return W(n, 24);
}

int LobbyChat_SetChannelUsersEnumerated(void *p, void *key) {
    void *n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n == A(p, 0x84)) {
        return 0;
    }
    W(n, 24) = 1;
    return 1;
}

int LobbyChat_GetChannelUserLimit(void *p, void *key) {
    void *n;
    LobbyChannelMap_Find(&n, A(p, 0x80), key);
    if (n == A(p, 0x84)) {
        return -1;
    }
    return W(n, 40);
}
