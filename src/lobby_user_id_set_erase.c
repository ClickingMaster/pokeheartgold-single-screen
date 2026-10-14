#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "../lib/NitroDWC/src/tree-iterator4.h"

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 data;
} Node;

typedef struct {
    int count;
    void *root, *last;
} Tree;

extern void ov45_02236E58(Iterator *);
extern Node *LobbyTree_EraseRebalance(Node *, void *);
extern void DWC_Free(int, void *, int);
extern Node *LobbyMem_Alloc(u32);

typedef struct {
    Node *first, *second;
    Node **root;
    Node *volatile cleanup;
} Guard;

void LobbyUserIdSet_CopyNodes(Tree *, Node *, Node **, int);
void LobbyUserIdSet_Erase(Iterator *, Tree *, Iterator);

void LobbyUserIdSet_Erase(Iterator *out, Tree *tree, Iterator node) {
    Node *old = node.current;
    if (node.current == tree->last) {
        ov45_02236E58(&node);
        tree->last = node.current;
    }
    out->current = LobbyTree_EraseRebalance(old, tree->root);
    if (old) {
        DWC_Free(0, old, 0);
    }
    tree->count--;
}
