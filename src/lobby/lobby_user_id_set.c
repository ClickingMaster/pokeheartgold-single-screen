#include "global.h"

#include "dwc/net_common_internal.h"
#include "lobby_chat_manager.h"
#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_mem.h"

extern u8 ov45_02255358[];
extern LobbyUserIdSetNode *LobbyTree_EraseRebalance(LobbyUserIdSetNode *, void *);

void *LobbyChannelMap_Destroy(LobbyChannelMap *map) {
    if (map->root) {
        LobbyChannelMap_DestroyNodes(map, map->root);
    }
    return map;
}

void LobbyUserIdSet_Insert(LobbyUserIdSetInsertResult *out, LobbyUserIdSet *set, int *key) {
    LobbyUserIdSetNode *prev;
    u8 left, first;
    LobbyUserIdSetNode *last;
    last = LobbyUserIdSet_FindInsertPos(set, key, &prev, &left, &first);
    if (!prev || prev->userId < *key) {
        out->node = LobbyUserIdSet_InsertNode(set, last, left, first, key);
        out->inserted = ov45_02255358[3];
    } else {
        out->node = prev;
        out->inserted = ov45_02255358[7];
    }
}

void LobbyUserIdSet_Erase(Iterator *out, LobbyUserIdSet *set, Iterator node) {
    LobbyUserIdSetNode *old = node.current;
    if (node.current == set->first) {
        LobbyTreeIterator_Advance(&node);
        set->first = node.current;
    }
    out->current = LobbyTree_EraseRebalance(old, set->root);
    if (old) {
        DWC_Free(0, old, 0);
    }
    set->count--;
}

void LobbyUserIdSet_Assign(LobbyUserIdSet *set, LobbyUserIdSet *other) {
    struct {
        u8 pad0;
        u8 byte;
        u8 pad[2];
        LobbyUserIdSetNode *a, *b;
        void *owner;
        LobbyUserIdSetNode *cleanup;
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
