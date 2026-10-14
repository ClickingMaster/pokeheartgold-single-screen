#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct Node {
    void *left, *right;
    u32 parent, value;
} Node;

typedef struct {
    Node *unused;
    void *root;
} Work;

typedef struct {
    u32 count;
    void *root;
    void *last;
} Tree;

extern Node *LobbyMem_Alloc(u32);
extern void LobbyUserIdSet_CopyNodes(Tree *, Node *, Node *, int);
extern void LobbyUserIdSet_Swap(void *, Tree *);
extern void LobbyUserIdSet_Destroy2(Tree *);
extern void DWC_Free(int, void *, int);
void LobbyUserIdSet_Assign(u8 *, Work *);

void LobbyUserIdSet_Assign(u8 *owner, Work *w) {
    struct {
        u8 pad0;
        volatile u8 byte;
        u8 pad[2];
        Node *volatile a, *volatile b;
        void *volatile owner;
        Node *volatile cleanup;
        Tree tree;
    } f;

    Node *n;
    u32 target;
    u32 flag;
    if (!w->root) {
        return;
    }
    f.byte = 0;
    n = LobbyMem_Alloc(16);
    f.owner = owner + 4;
    f.a = n;
    f.b = n;
    f.cleanup = n;
    target = (u32)n + 12;
    {
        Node *root = w->root;
        if (target) {
            *(u32 *)target = root->value;
        }
    };
    f.tree.count = 0;
    f.tree.root = 0;
    f.tree.last = (Node *)&f.tree.root;
    n = f.cleanup;
    n->right = 0;
    n->left = 0;
    n->parent = (u32)&f.tree.root | (n->parent & 1);
    flag = (((Node *)w->root)->parent & 1) != 0;
    n = f.cleanup;
    if (flag) {
        n->parent |= 1;
    } else {
        n->parent &= ~1;
    }
    n = f.cleanup;
    f.tree.root = n;
    f.tree.count = 1;
    f.tree.last = n;
    {
        Node *root = w->root;
        f.cleanup = 0;
        if (root->right) {
            LobbyUserIdSet_CopyNodes(&f.tree, n, root, 0);
        }
    }
    if (((Node *)w->root)->left) {
        LobbyUserIdSet_CopyNodes(&f.tree, f.tree.root, w->root, 1);
    }
    LobbyUserIdSet_Swap(owner, &f.tree);
    LobbyUserIdSet_Destroy2(&f.tree);
    {
        Node *cleanup = f.cleanup;
        if (cleanup) {
            DWC_Free(0, cleanup, 0);
        }
    };
}
