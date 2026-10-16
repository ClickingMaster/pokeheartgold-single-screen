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

extern void *LobbyUserIdSet_Destroy(void *p);
extern Node *LobbyUserMap_FindInsertPos(Tree *p, int *key, Node **prev, u8 *left, u8 *first);
extern void LobbyUserIdSet_DestroyNodes(void *p, void *node);

typedef struct {
    Node *node;
    u8 ok;
} InsertResult;

extern Node *LobbyUserMap_InsertNode(Tree *, Node *, int, int, int *);
extern u8 ov45_02255358[];
extern void LobbyUserMap_Insert(InsertResult *, Tree *, int *);

void *LobbyUserIdSet_Destroy(void *p) {
    if (P(p, 4)) {
        LobbyUserIdSet_DestroyNodes(p, P(p, 4));
    }
    return p;
}

void LobbyUserMap_Insert(InsertResult *out, Tree *p, int *key) {
    Node *prev;
    u8 left, first;
    Node *last;
    last = LobbyUserMap_FindInsertPos(p, key, &prev, &left, &first);
    if (!prev || prev->key < *key) {
        out->node = LobbyUserMap_InsertNode(p, last, left, first, key);
        out->ok = ov45_02255358[0];
    } else {
        out->node = prev;
        out->ok = ov45_02255358[1];
    }
}
