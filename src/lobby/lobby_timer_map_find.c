#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
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

extern Node *LobbyTimerMap_LowerBound(u8 *, u32 *, Node *, Node *);
extern void LobbyTimerMap_Find(Node **, u8 *, u32 *);
void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key);
Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best);

void LobbyTimerMap_Find(Node **out, u8 *p, u32 *key) {
    Node *n = LobbyTimerMap_LowerBound(p, key, *(Node **)(p + 4), (Node *)(p + 4));
    if (n == (Node *)(p + 4) || *key < n->key) {
        *out = (Node *)(p + 4);
    } else {
        *out = n;
    }
}

Node *LobbyTimerMap_LowerBound(u8 *p, u32 *key, Node *n, Node *best) {
    if (n) {
        u32 x = *key;
        do {
            if (n->key >= x) {
                best = n;
                n = n->left;
            } else {
                n = n->right;
            }
        } while (n);
    }
    return best;
}
