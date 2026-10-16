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

extern void *LobbyUserMap_Destroy(void *p);
extern void *LobbyRequestMap_Destroy(void *p);
extern void LobbyUserMap_DestroyNodes(void *p, void *node);
extern void LobbyRequestMap_DestroyNodes(void *p, void *node);

typedef struct {
    Node *node;
    u8 ok;
} InsertResult;

void *LobbyUserMap_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyUserMap_DestroyNodes(p, P(p, 4));
    }
    return p;
}

void *LobbyRequestMap_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyRequestMap_DestroyNodes(p, P(p, 4));
    }
    return p;
}
