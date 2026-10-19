#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_tree.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern int printf(const char *, ...);
extern void abort(void);

extern const char sStr_TreeInsertLengthError_4[];
extern LobbyUserIdSetNode *LobbyMem_Alloc(u32);
extern void LobbyTree_InsertRebalance(LobbyUserIdSetNode *, void *);

LobbyUserIdSetNode *LobbyUserIdSet_InsertNode(LobbyUserIdSet *set, LobbyUserIdSetNode *parent, int left, int last, const int *key) {
    struct {
        volatile u8 byte;
        u8 pad[3];
        LobbyUserIdSetNode *volatile a, *volatile b;
        void *volatile owner;
        LobbyUserIdSetNode *volatile cleanup;
    } f;

    LobbyUserIdSetNode *n;
    int *userId;
    u32 *link;
    if (set->count == -1) {
        printf(sStr_TreeInsertLengthError_4);
        abort();
    }
    f.byte = 0;
    n = LobbyMem_Alloc(16);
    f.owner = &set->root;
    userId = &n->userId;
    f.cleanup = n;
    f.a = n;
    if (userId) {
        *userId = *key;
    }
    n->right = 0;
    f.cleanup = 0;
    n->left = 0;
    link = &n->parent;
    if (link) {
        *link = (u32)parent;
    }
    if (left) {
        parent->left = n;
    } else {
        parent->right = n;
    }
    set->count++;
    f.b = n;
    LobbyTree_InsertRebalance(n, set->root);
    if (last) {
        set->first = n;
    }
    {
        LobbyUserIdSetNode *cleanup = f.cleanup;
        if (cleanup) {
            DWC_Free(0, cleanup, 0);
        }
    };
    return n;
}
