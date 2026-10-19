#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

extern LobbyUserIdSetNode *LobbyMem_Alloc(u32);

void LobbyUserIdSet_Assign(LobbyUserIdSet *set, LobbyUserIdSet *other) {
    struct {
        u8 pad0;
        volatile u8 byte;
        u8 pad[2];
        LobbyUserIdSetNode *volatile a, *volatile b;
        void *volatile owner;
        LobbyUserIdSetNode *volatile cleanup;
        LobbyUserIdSet tree;
    } f;

    LobbyUserIdSetNode *n;
    int *userId;
    u32 flag;
    if (!other->root) {
        return;
    }
    f.byte = 0;
    n = LobbyMem_Alloc(16);
    f.owner = &set->root;
    f.a = n;
    f.b = n;
    f.cleanup = n;
    userId = &n->userId;
    {
        LobbyUserIdSetNode *root = other->root;
        if (userId) {
            *userId = root->userId;
        }
    };
    f.tree.count = 0;
    f.tree.root = 0;
    f.tree.first = (LobbyUserIdSetNode *)&f.tree.root;
    n = f.cleanup;
    n->right = 0;
    n->left = 0;
    n->parent = (u32)&f.tree.root | (n->parent & 1);
    flag = (other->root->parent & 1) != 0;
    n = f.cleanup;
    if (flag) {
        n->parent |= 1;
    } else {
        n->parent &= ~1;
    }
    n = f.cleanup;
    f.tree.root = n;
    f.tree.count = 1;
    f.tree.first = n;
    {
        LobbyUserIdSetNode *root = other->root;
        f.cleanup = 0;
        if (root->right) {
            LobbyUserIdSet_CopyNodes(&f.tree, n, root, 0);
        }
    }
    if (other->root->left) {
        LobbyUserIdSet_CopyNodes(&f.tree, f.tree.root, other->root, 1);
    }
    LobbyUserIdSet_Swap(set, &f.tree);
    LobbyUserIdSet_Destroy2(&f.tree);
    {
        LobbyUserIdSetNode *cleanup = f.cleanup;
        if (cleanup) {
            DWC_Free(0, cleanup, 0);
        }
    };
}
